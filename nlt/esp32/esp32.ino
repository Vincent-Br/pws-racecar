#include "structs.h"
#include <WiFi.h>
#include <ArduinoWebsockets.h>

Car car;   // define car variable
Conn conn; // wifi connection

const char* websockets_server_host = "ws.auto.sillyfrog.nl"; //Enter server adress
const uint16_t websockets_server_port = 8100; // Enter server port

using namespace websockets;

WebsocketsClient client;

int minCenterDist = 20;
int minSideDist = 10;

float leftSpeedMultiplier = 1.0;
float rightSpeedMultiplier = 1.0;

void setup()
{
    // assign pins
    car.pins.output.distanceLeftTrig = 32;
    car.pins.input.distanceLeftEcho = 39;
    car.pins.output.distanceCenterTrig = 33;
    car.pins.input.distanceCenterEcho = 34;
    car.pins.output.distanceRightTrig = 25;
    car.pins.input.distanceRightEcho = 35;

    car.pins.output.leftForward = 26;
    car.pins.output.leftBackward = 27;
    car.pins.output.rightForward = 14;
    car.pins.output.rightBackward = 12;

    pinMode(car.pins.output.distanceLeftTrig, OUTPUT);
    pinMode(car.pins.input.distanceLeftEcho, INPUT);
    pinMode(car.pins.output.distanceCenterTrig, OUTPUT);
    pinMode(car.pins.input.distanceCenterEcho, INPUT);
    pinMode(car.pins.output.distanceRightTrig, OUTPUT);
    pinMode(car.pins.input.distanceRightEcho, INPUT);

    pinMode(car.pins.output.leftForward, OUTPUT);
    pinMode(car.pins.output.leftBackward, OUTPUT);
    pinMode(car.pins.output.rightForward, OUTPUT);
    pinMode(car.pins.output.rightBackward, OUTPUT);

    // set car id
    car.Id = "Auto-1";

    // set wifi credentials
    conn.ssid = "Vinnie";
    conn.password = "WaggoeW00rd"; //! Replace password

    WiFi.setHostname(car.Id);
    WiFi.mode(WIFI_STA);
    WiFi.begin(conn.ssid, conn.password);

    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    } // wait for wifi connection

    client.connect(websockets_server_host, websockets_server_port, "/"); // connect to websocket

    delay(1000); // delay main loop

}

long getDistance(int trigPin, int echoPin)
{
    long duration;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    return duration * 0.034 / 2; // calculate distance from duration
}

void querySensors()
{
    Distance distance;

    distance.left = getDistance(car.pins.output.distanceLeftTrig, car.pins.input.distanceLeftEcho);
    distance.center = getDistance(car.pins.output.distanceCenterTrig, car.pins.input.distanceCenterEcho);
    distance.right = getDistance(car.pins.output.distanceRightTrig, car.pins.input.distanceRightEcho);

    car.distance = distance;

    return;
}

void sendMessage() {
    char data[64];

    sprintf(data, "%.2f,%.2f,%.2f", car.distance.left, car.distance.center, car.distance.right);

    client.send(data);

    return;
}

void setControls(float left, float right)
{
    float leftSpeed = left * leftSpeedMultiplier * 255;
    float rightSpeed = right * rightSpeedMultiplier * 255;

    if (leftSpeed >= 0.0)
    {
        analogWrite(car.pins.output.leftForward, abs(leftSpeed));
        analogWrite(car.pins.output.leftBackward, 0);
    }
    else
    {
        analogWrite(car.pins.output.leftBackward, abs(leftSpeed));
        analogWrite(car.pins.output.leftForward, 0);
    }

    if (rightSpeed >= 0.0)
    {
        analogWrite(car.pins.output.rightForward, abs(rightSpeed));
        analogWrite(car.pins.output.rightBackward, 0);
    }
    else
    {
        analogWrite(car.pins.output.rightBackward, abs(rightSpeed));
        analogWrite(car.pins.output.rightForward, 0);
    }

    return;
}

void loop()
{
    querySensors();

    if ((car.distance.center < minCenterDist) || (car.distance.left < minSideDist) || (car.distance.right < minSideDist))
    {
        if (car.distance.left > minCenterDist)
        {
            setControls(-1.0, 1.0); // turn left
        }
        else if (car.distance.right > minCenterDist)
        {
            setControls(1.0, -1.0); // turn right
        }
        else
        {
            setControls(-1.0, -1.0); // ride backwards
        }
    }
    else if (car.distance.center < (minCenterDist * 2))
    {
        setControls(0.5, 0.5); // ride slowly forwards
    }
    else
    {
        setControls(1.0, 1.0); // ride forwards
    }

    sendMessage();   
}