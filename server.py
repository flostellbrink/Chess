import http.server
from http.server import HTTPServer, BaseHTTPRequestHandler
import socketserver

PORT = 8000
print("Serving on port", PORT)

Handler = http.server.SimpleHTTPRequestHandler
Handler.extensions_map = {
    '.manifest': 'text/cache-manifest',
    '.html': 'text/html',
    '.png': 'image/png',
    '.jpg': 'image/jpg',
    '.svg': 'image/svg+xml',
    '.css': 'text/css',
    '.js': 'application/x-javascript',
    '.wasm': "application/wasm",
    '': 'application/octet-stream'
}

httpd = socketserver.TCPServer(("", PORT), Handler)
httpd.serve_forever()
