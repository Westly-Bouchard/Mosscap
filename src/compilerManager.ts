import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';
import axios from 'axios';
import extract from 'extract-zip';

const DOWNLOAD_URLS: Record<string, string> = {
    'win32-x64': 'https://github.com/Westly-Bouchard/Mosscap/releases/download/toolchains-v1/emsdk-Windows.zip',
    'darwin-x64': 'https://github.com/Westly-Bouchard/Mosscap/releases/download/toolchains-v1/emsdk-macOS-Intel.zip',
    'darwin-arm64': 'https://github.com/Westly-Bouchard/Mosscap/releases/download/toolchains-v1/emsdk-macOS-AppleSilicon.zip',
    // 'linux-x64': 'https://github.com/Westly-Bouchard/Mosscap/releases/download/toolchains-v1/emsdk-Linux.zip'
    'linux-x64': 'http://localhost:8000/emsdk-Linux.zip'
};

export async function ensureCompilerAvailable(context: vscode.ExtensionContext): Promise<string> {
    // Determine os (and in the case of macOS, architecture)
    const platformKey = `${process.platform}-${process.arch}`;
    let platform = "";
    if (platformKey === "win32-x64") {
        platform = "Windows";
    } else if (platformKey === "darwin-x64") {
        platform = "macOS-Intel";
    } else if (platformKey === "darwin-arm64") {
        platform = "macOS-AppleSilicon";
    } else if (platformKey === "linux-x64") {
        platform = "Linux";
    }

    const downloadUrl = DOWNLOAD_URLS[platformKey];

    if (!downloadUrl) {
        throw new Error(`Unsupported operating system or architecture: ${platformKey}`);
    }

    // Put compiler in global storage
    const storageUri = context.globalStorageUri;
    console.log("STORAGE PATH:", storageUri.fsPath);
    const compilerDir = path.join(storageUri.fsPath, 'emsdk');
    // const compilerDir = storageUri.fsPath;
    
    // Windows requires .bat, Mac/Linux use standard binaries
    const executableName = process.platform === 'win32' ? 'emcc.bat' : 'emcc';

    const emccPath = path.join(compilerDir, 'emsdk', 'emsdk-' + platform, 'upstream', 'emscripten', executableName);

    // If it's already been downloaded, we can return early
    if (fs.existsSync(emccPath)) {
        return emccPath;
    }

    // If it doesn't exist, then download and extract it
    await vscode.window.withProgress({
        location: vscode.ProgressLocation.Notification,
        title: "Downloading Simulator Compiler...",
        cancellable: false
    }, async (progress) => {
        fs.mkdirSync(compilerDir, { recursive: true });
        // const zipPathFull = path.join(storageUri.fsPath, "emsdk-" + platform + ".zip");
        const zipPathFull = path.join(compilerDir, "emsdk-" + platform + ".zip");

        progress.report({ message: "Downloading..." });
        const response = await axios({
            url: downloadUrl,
            method: 'GET',
            responseType: 'stream'
        });

        const writer = fs.createWriteStream(zipPathFull);

        await new Promise<void>((resolve, reject) => {
            response.data.pipe(writer);

            writer.on('error', (err) => {
                writer.close();
                reject(err);
            });

            // Wait for the OS to release the file handle, NOT just finish piping
            writer.on('close', () => resolve());
        });

        progress.report({ message: "Extracting..." });
        await extract(zipPathFull, { dir: compilerDir });

        // Cleanup the zip file
        fs.unlinkSync(zipPathFull);
    });

    return emccPath;
}

export async function ensureCompilerAvailable(context: vscode.ExtensionContext): Promise<string> {
    // Determine user's operating system
    const platformKey = `${process.platform}-${process.arch}`;

    // Determine platform string
    let platform = "";
    if (platformKey === "win32-x64") {
        platform = "Windows";
    } else if (platformKey === "darwin-x64") {
        platform = "macOS-Intel";
    } else if (platformKey === "darwin-arm64") {
        platform = "macOS-AppleSilicon";
    } else if (platformKey === "linux-x64") {
        platform = "Linux";
    }

    // Get associated download URL for platform
    const downloadUrl = DOWNLOAD_URLS[platformKey];

    // If we didn't get a download URL, this is an unsupoorted platform
    if (!downloadUrl) {
        throw new Error(`Unsupported operating system or architecture: ${platformKey}`);
    }

    // Zip will have unzipped to 
}