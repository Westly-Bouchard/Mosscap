import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';
import axios from 'axios';
import extract from 'extract-zip';

const DOWNLOAD_URLS: Record<string, string> = {
    'win32-x64': 'http://localhost:8000/emsdk-test.zip',
    'darwin-x64': 'http://localhost:8000/emsdk-test.zip',
    'darwin-arm64': 'http://localhost:8000/emsdk-test.zip',
    'linux-x64': 'http://localhost:8000/emsdk-test.zip'
};

export async function ensureCompilerAvailable(context: vscode.ExtensionContext): Promise<string> {
    // 1. Determine OS and Architecture
    const platformKey = `${process.platform}-${process.arch}`;
    const downloadUrl = DOWNLOAD_URLS[platformKey];

    if (!downloadUrl) {
        throw new Error(`Unsupported operating system or architecture: ${platformKey}`);
    }

    // 2. Define where the compiler should live (VSCode's global storage for your extension)
    const storageUri = context.globalStorageUri;
    console.log("STORAGE PATH:", storageUri.fsPath);
    const compilerDir = path.join(storageUri.fsPath, 'emsdk');
    
    // Windows requires .bat, Mac/Linux use standard binaries
    const executableName = process.platform === 'win32' ? 'emcc.bat' : 'emcc';
    const emccPath = path.join(compilerDir, 'emsdk', 'upstream', 'emscripten', executableName);

    // 3. If it already exists, we are done!
    if (fs.existsSync(emccPath)) {
        return emccPath;
    }

    // 4. If it doesn't exist, download and extract it
    await vscode.window.withProgress({
        location: vscode.ProgressLocation.Notification,
        title: "Downloading Simulator Compiler (First Run Only)...",
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