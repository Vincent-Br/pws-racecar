const fs = require('fs');
const https = require('https');
const WebSocketServer = require('ws').Server;

const server = new https.createServer({
    cert: fs.readFileSync(''),
    key: fs.readFileSync(''),
});

var wss = new WebSocketServer({ server });

wss.on('connection', (ws) => {
    ws.on('message', (msg) => {
        wss.clients.forEach((client) => {
            if (client != ws) {
                client.send(msg);
            }
        });
    });
});

server.listen(8100);
