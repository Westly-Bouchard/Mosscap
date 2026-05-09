import * as vscode from 'vscode';
import * as http from 'http';
import * as fs from 'fs';
import * as path from 'path';

let simulatorServer: http.Server | undefined;

function startSimulatorServer(workspaceRoot: string): Promise<number> {
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
            const filePath = path.join(workspaceRoot, 'cmake-build-debug-emscripten', requestUrl!);

            fs.readFile(filePath, (err, data) => {
                if (err) {
                    res.writeHead(404);
                    res.end("File not found");
                    return;
                }

                const ext = path.extname(filePath);
                let contentType = 'text/html';
                if (ext === '.js' || ext === '.worker.js') contentType = 'application/javascript';
                else if (ext === '.wasm') contentType = 'application/wasm';

                res.writeHead(200, { 'Content-Type': contentType });
                res.end(data);
            });
        });

        // Start on a random port
        simulatorServer.listen(0, '127.0.0.1', () => {
            const address = simulatorServer?.address();
            if (address && typeof address === 'object') {
                resolve(address.port);
            } else {
                reject(new Error("Failed to get port"));
            }
        });
    });
}

export function activate(context: vscode.ExtensionContext) {
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

        } catch (error) {
            vscode.window.showErrorMessage(`Failed to start simulator: ${error}`);
        }
    });

    context.subscriptions.push(disposable);
}

export function deactivate() {
    if (simulatorServer) {
        simulatorServer.close();
    }
}