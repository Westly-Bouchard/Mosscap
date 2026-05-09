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
Object.defineProperty(exports, "__esModule", { value: true });
exports.activate = activate;
exports.deactivate = deactivate;
const vscode = __importStar(require("vscode"));
const http = __importStar(require("http"));
const fs = __importStar(require("fs"));
const path = __importStar(require("path"));
let simulatorServer;
function startSimulatorServer(workspaceRoot) {
    return new Promise((resolve, reject) => {
        if (simulatorServer) {
            simulatorServer.close();
        }
        simulatorServer = http.createServer((req, res) => {
            // Inject the headers for WebAssembly Threads (SharedArrayBuffer)
            res.setHeader("Cross-Origin-Opener-Policy", "same-origin");
            res.setHeader("Cross-Origin-Embedder-Policy", "require-corp");
            // Look for the files in the 'build' folder of the open workspace
            const requestUrl = req.url === '/' ? '/ArduSim.html' : req.url;
            const filePath = path.join(workspaceRoot, 'cmake-build-debug-emscripten', requestUrl);
            fs.readFile(filePath, (err, data) => {
                if (err) {
                    res.writeHead(404);
                    res.end("File not found");
                    return;
                }
                const ext = path.extname(filePath);
                let contentType = 'text/html';
                if (ext === '.js' || ext === '.worker.js')
                    contentType = 'application/javascript';
                else if (ext === '.wasm')
                    contentType = 'application/wasm';
                res.writeHead(200, { 'Content-Type': contentType });
                res.end(data);
            });
        });
        // Start on a random port
        simulatorServer.listen(0, '127.0.0.1', () => {
            const address = simulatorServer?.address();
            if (address && typeof address === 'object') {
                resolve(address.port);
            }
            else {
                reject(new Error("Failed to get port"));
            }
        });
    });
}
function activate(context) {
    let disposable = vscode.commands.registerCommand('ardusim.start', async () => {
        const workspaceFolders = vscode.workspace.workspaceFolders;
        if (!workspaceFolders) {
            vscode.window.showErrorMessage("Please open a folder containing your simulator build.");
            return;
        }
        const rootPath = workspaceFolders[0].uri.fsPath;
        try {
            // 1. Start the secure server
            const port = await startSimulatorServer(rootPath);
            // 2. Open the user's default web browser
            const localUrl = vscode.Uri.parse(`http://127.0.0.1:${port}/ArduSim.html`);
            await vscode.env.openExternal(localUrl);
            vscode.window.showInformationMessage(`Simulator running in browser on port ${port}`);
        }
        catch (error) {
            vscode.window.showErrorMessage(`Failed to start simulator: ${error}`);
        }
    });
    context.subscriptions.push(disposable);
}
function deactivate() {
    if (simulatorServer) {
        simulatorServer.close();
    }
}
//# sourceMappingURL=extension.js.map