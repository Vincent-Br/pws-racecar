import { MessageType, WSPayload } from './structs';

var ws = new WebSocket('ws://ws.carcontrol.vincentbrokx.nl');

export function sendMessage(messageType: Array<MessageType>, data: object) {
    var payload: WSPayload = [0x00, {}];

    payload[0] = getMessageTypeHash(messageType);
    payload[1] = data;

    ws.send(JSON.stringify(payload));
    return;
}

function getMessageTypeHash(messageType: Array<MessageType>) {
    /*  Types
        1: Init
        2: Getsensordata
        3: Setcontrols
    */

    var hash = 0x00;

    messageType.forEach((type) => {
        if (type === 'INIT') hash = hash + 0x01;
        if (type === 'SENSORS') hash = hash + 0x02;
        if (type === 'CONTROLS') hash = hash + 0x03;
        // if (type === '') hash = hash + 0x04;
    });

    return hash;
}
