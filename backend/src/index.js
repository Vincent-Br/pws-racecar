const WebSocketServer = require('ws').Server;

var wss = new WebSocketServer({ port: 8100 });

wss.on('connection', (ws) => {
    ws.on('message', (msg) => {
        wss.clients.forEach((client) => {
            if (client != ws) {
                client.send(msg);
            }
        });
    });
});
