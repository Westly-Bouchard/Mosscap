import * as vscode from 'vscode';
import * as fs from 'fs';
import * as path from 'path';
import axios from 'axios';
import extract from 'extract-zip';

const DOWNLOAD_URLS: Record<string, string> = {
    'Windows': 'https://github.com/Westly-Bouchard/Mosscap/releases/download/toolchains-v1/emsdk-Windows.zip',
    'macOS-Intel': 'https://github.com/Westly-Bouchard/Mosscap/releases/download/toolchains-v1/emsdk-macOS-Intel.zip',
    'macOS-AppleSilicon': 'https://github.com/Westly-Bouchard/Mosscap/releases/download/toolchains-v1/emsdk-macOS-AppleSilicon.zip',
    'Linux': 'https://github.com/Westly-Bouchard/Mosscap/releases/download/toolchains-v1/emsdk-Linux.zip'
    // 'linux-x64': 'http://localhost:8000/emsdk-Linux.zip'
};

export async function ensureCompilerAvailable(context: vscode.ExtensionContext): Promise<string> {
    // Determine user's operating system / platform
    let platform = "";
    switch(`${process.platform}-${process.arch}`) {
        case 'win32-x64':
            platform = "Windows";
            break;

        case 'darwin-x64':
            platform = "macOS-Intel"
            break;

        case 'darwin-arm64':
            platform = "macOS-AppleSilicon";
            break;

        case 'linux-x64':
            platform = "Linux";
            break;
        
        default:
            throw new Error(`Unsupported operating system or architecture: ${process.platform}-${process.arch}`);
    }

    // Build full path to compiler to check if it exists
    
    // Extension global storage path
    const storageURI = context.globalStorageUri;
    // For debugging, so we can delete stuff if we need to
    console.log("STORAGE PATH: ", storageURI.fsPath);

    // Path to sdk within global storage
    const emsdkDir = path.join(storageURI.fsPath, 'emsdk');

    // On windows emcc is provided as a .bat file
    const exeName = platform === 'Windows' ? 'emcc.bat' : 'emcc';

    // Full emcc path
    const emccPath = path.join(emsdkDir, 'emsdk', 'upstream', 'emscripten', exeName);

    // If it exists, return it and we're done
    if (fs.existsSync(emccPath)) {
        return emccPath;
    }

    // If not, we have to download it
    // Get associated download URL for platform
    const downloadUrl = DOWNLOAD_URLS[platform];

    await vscode.window.withProgress({
        location: vscode.ProgressLocation.Notification,
        title: "Downloading Simulation Compiler",
        cancellable: false
    }, async (progress) => {
        // Ensure that the global storage directory has been created
        fs.mkdirSync(storageURI.fsPath, { recursive: true });

        // Path to zip that will be downloaded
        const zipPath = path.join(storageURI.fsPath, `emsdk-${platform}.zip`);

        // Some weird js stuff, I don't really know.
        // This was mostly gemini tbh
        progress.report({ message: "Downloading..." });
        const response = await axios({
            url: downloadUrl,
            method: 'GET',
            responseType: 'stream'
        });

        const writer = fs.createWriteStream(zipPath);

        await new Promise<void>((resolve, reject) => {
            response.data.pipe(writer);

            writer.on('error', (err) => {
                writer.close();
                reject(err);
            });

            // Wait for the OS to release the file handle, NOT just finish piping
            writer.on('close', () => resolve());
        });

        // Now extract the sdk from the downloaded archive
        progress.report({ message: "Extracting..." });
        await extract(zipPath, { dir: emsdkDir });

        // Cleanup the zip file
        fs.unlinkSync(zipPath);
    });

    return emccPath;
}