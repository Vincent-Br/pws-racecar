#include "structs.h"

Car car; // define car variable

int minCenterDist = 20;
int minSideDist = 10;

float leftSpeedMultiplier = 1.0;
float rightSpeedMultiplier = 1.0;

void setup()
{
    // assign pins
    car.pins.output.distanceLeftTrig = 2;
    car.pins.input.distanceLeftEcho = 3;
    car.pins.output.distanceCenterTrig = 4;
    car.pins.input.distanceCenterEcho = 5;
    car.pins.output.distanceRightTrig = 7;
    car.pins.input.distanceRightEcho = 8;

    car.pins.output.leftForward = 6;
    car.pins.output.leftBackward = 9;
    car.pins.output.rightForward = 10;
    car.pins.output.rightBackward = 11;

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

    Serial.begin(9600); // TODO: REMOVE
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

    // Serial.print("Left: ");
    // Serial.println(car.distance.left);
    // Serial.print("Center: ");
    // Serial.println(car.distance.center);
    // Serial.print("Right: ");
    // Serial.println(car.distance.right);
}