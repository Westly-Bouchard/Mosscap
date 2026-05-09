import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';
import axios from 'axios';
import extract from 'extract-zip';

const DOWNLOAD_URLS: Record<string, string> = {
    'win32-x64': 'https://github.com/Westly-Bouchard/Mosscap/releases/download/toolchains-v1/emsdk-Windows.zip',
    'darwin-x64': 'https://github.com/Westly-Bouchard/Mosscap/releases/download/toolchains-v1/emsdk-macOS-Intel.zip',
    'darwin-arm64': 'https://github.com/Westly-Bouchard/Mosscap/releases/download/toolchains-v1/emsdk-macOS-AppleSilicon.zip',
    'linux-x64': 'https://github.com/Westly-Bouchard/Mosscap/releases/download/toolchains-v1/emsdk-Linux.zip'
};

export async function ensureCompilerAvailable(context: vscode.ExtensionContext): Promise<string> {
    // Determine os (and in the case of macOS, architecture)
    const platformKey = `${process.platform}-${process.arch}`;
    const downloadUrl = DOWNLOAD_URLS[platformKey];

    if (!downloadUrl) {
        throw new Error(`Unsupported operating system or architecture: ${platformKey}`);
    }

    // Put compiler in global storage
    const storageUri = context.globalStorageUri;
    console.log("STORAGE PATH:", storageUri.fsPath);
    const compilerDir = path.join(storageUri.fsPath, 'emsdk');
    
    // Windows requires .bat, Mac/Linux use standard binaries
    const executableName = process.platform === 'win32' ? 'emcc.bat' : 'emcc';
    const emccPath = path.join(compilerDir, 'emsdk', 'upstream', 'emscripten', executableName);

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
        fs.mkdirSync(storageUri.fsPath, { recursive: true });
        const zipPath = path.join(storageUri.fsPath, 'emsdk.zip');

        progress.report({ message: "Downloading..." });
        const response = await axios({
            url: downloadUrl,
            method: 'GET',
            responseType: 'stream'
        });

        const writer = fs.createWriteStream(zipPath);
        response.data.pipe(writer);

        await new Promise<void>((resolve, reject) => {
            writer.on('finish', () => resolve());
            writer.on('error', (err) => reject(err));
        });

        progress.report({ message: "Extracting..." });
        await extract(zipPath, { dir: compilerDir });

        // Cleanup the zip file
        fs.unlinkSync(zipPath);
    });

    return emccPath;
}