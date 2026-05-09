import http.server
import socketserver

class SecureHandler(http.server.SimpleHTTPRequestHandler):
    def end_headers(self):
        # Inject the magic security headers for SharedArrayBuffer
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")
        super().end_headers()

PORT = 8080
with socketserver.TCPServer(("", PORT), SecureHandler) as httpd:
    print(f"Serving securely at http://localhost:{PORT}")
    httpd.serve_forever()