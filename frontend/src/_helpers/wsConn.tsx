import { Car, MessageType, WSPayload } from './structs';

var ws: WebSocket;
export var car: Car = {
    Id: 'null',
    sensors: {
        rotation: {
            x: 0,
            y: 0,
            z: 0,
        },
        distance: {
            left: 0,
            center: 0,
            right: 0,
        },
    },
    position: {
        x: 0,
        y: 0,
        z: 0,
    },
};

export function openWebsocket() {
    ws = new WebSocket('ws://ws.auto.sillyfrog.nl:8100');

    ws.addEventListener('message', (e) => {
        // var reader = new FileReader();
        // reader.onload = () => {
        //     console.log(reader.result);
        // };
        // reader.readAsArrayBuffer(e.data);

        var data: WSPayload = e.data.text();

        car.Id = '12323rv sg' + Math.random();

        console.log(data[0]);
    });
}

export function sendMessage(messageType: Array<MessageType>, data: object) {
    var payload: WSPayload = [0x00, {}];

    payload[0] = getMessageTypeHash(messageType);
    payload[1] = data;

    ws.send(JSON.stringify(payload));
    return;
}

function getMessageTypeHash(messageType: Array<MessageType>): number {
    /*  Types
        1: Handshake
        2: Init
        3: Heartbeat
        4: Getsensordata
        5: Setsensordata
        6: Getcontrols
        7: Setcontrols
    */

    var hash = 0x00;

    messageType.forEach((type) => {
        if (type === 'HANDSHAKE') hash = hash + 0x01; // 1
        if (type === 'INIT') hash = hash + 0x02; // 2
        if (type === 'HEARTBEAT') hash = hash + 0x04; // 3
        if (type === 'GETSENSORS') hash = hash + 0x08; // 4
        if (type === 'SETCONTROLS') hash = hash + 0x20; // 6
    });

    return hash;
}
