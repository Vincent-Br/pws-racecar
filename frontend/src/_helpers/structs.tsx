export type Car = {
    Id: string;
    sensors: Sensors;
    position: {
        x: number;
        y: number;
        z: number;
    };
};

export type Sensors = {
    rotation: {
        x: number;
        y: number;
        z: number;
    };
    distance: {
        left: number;
        center: number;
        right: number;
    };
};

export type MessageType = 'INIT' | 'SENSORS' | 'CONTROLS';

export type WSPayload = [number, object];
