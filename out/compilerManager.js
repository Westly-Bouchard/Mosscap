"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || (function () {
    var ownKeys = function(o) {
        ownKeys = Object.getOwnPropertyNames || function (o) {
            var ar = [];
            for (var k in o) if (Object.prototype.hasOwnProperty.call(o, k)) ar[ar.length] = k;
            return ar;
        };
        return ownKeys(o);
    };
    return function (mod) {
        if (mod && mod.__esModule) return mod;
        var result = {};
        if (mod != null) for (var k = ownKeys(mod), i = 0; i < k.length; i++) if (k[i] !== "default") __createBinding(result, mod, k[i]);
        __setModuleDefault(result, mod);
        return result;
    };
})();
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.ensureCompilerAvailable = ensureCompilerAvailable;
const vscode = __importStar(require("vscode"));
const fs = __importStar(require("fs"));
const path = __importStar(require("path"));
const axios_1 = __importDefault(require("axios"));
const extract_zip_1 = __importDefault(require("extract-zip"));
const DOWNLOAD_URLS = {
    'win32-x64': 'http://localhost:8000/emsdk-test.zip',
    'darwin-x64': 'http://localhost:8000/emsdk-test.zip',
    'darwin-arm64': 'http://localhost:8000/emsdk-test.zip',
    'linux-x64': 'http://localhost:8000/emsdk-test.zip'
};
async function ensureCompilerAvailable(context) {
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
        const response = await (0, axios_1.default)({
            url: downloadUrl,
            method: 'GET',
            responseType: 'stream'
        });
        const writer = fs.createWriteStream(zipPath);
        response.data.pipe(writer);
        await new Promise((resolve, reject) => {
            writer.on('finish', () => resolve());
            writer.on('error', (err) => reject(err));
        });
        progress.report({ message: "Extracting..." });
        await (0, extract_zip_1.default)(zipPath, { dir: compilerDir });
        // Cleanup the zip file
        fs.unlinkSync(zipPath);
    });
    return emccPath;
}
//# sourceMappingURL=compilerManager.js.map