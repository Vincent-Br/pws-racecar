// (c) 2024 Vincent Brokx, https://vincentbrokx.nl

#include "structs.h"
#include <map>
// #include <WiFi.h>
// #include <RDV_GY521.h> // https://github.com/TheSpaceDragon/rdv-gy521/

Car car;   // define car variable
Conn conn; // wifi connection

TaskHandle_t Core0; // CORE0 - Sensors
TaskHandle_t Core1; // CORE1 - Networking / drive control

void setPinMode()
{
    //! DOESN'T MAP car.pins.<type>
    std::map<std::string, int> inputPins;
    std::map<std::string, int> outputPins;

    for (const auto &[key, value] : inputPins)
    {
        pinMode(value, INPUT);
    }

    for (const auto &[key, value] : outputPins)
    {
        pinMode(value, OUTPUT);
    }
}

void setup()
{
    // assign pins
    car.pins.output.distanceLeftTrig = 0;
    car.pins.input.distanceLeftEcho = 0;
    car.pins.output.distanceCenterTrig = 0;
    car.pins.input.distanceCenterEcho = 0;
    car.pins.output.distanceRightTrig = 0;
    car.pins.input.distanceRightEcho = 0;

    // set car id
    car.Id = "PWS-Racecar-1";

    // set wifi credentials
    conn.ssid = "hometech2";
    conn.password = "NULL"; //! Replace password

    // WiFi.setHostname(car.Id.c_str());
    // WiFi.mode(WIFI_STA);
    // WiFi.begin(conn.ssid, conn.password);

    setPinMode();

    // device.sensor_init(); // init gyro sensor

    xTaskCreatePinnedToCore(
        Task1Code,
        "Task1",
        10000,
        NULL,
        1,
        &Core0,
        0);
    delay(500);
    xTaskCreatePinnedToCore(
        Task2Code,
        "Task2",
        10000,
        NULL,
        1,
        &Core1,
        1);
    delay(500);
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

void Task1Code(void *parameter)
{
    for (;;)
    {
        Sensors sensors;
        Position position;

        int time = xTaskGetTickCount(); //* get ticks since boot (tick = 1ms)

        // distance
        sensors.distance.left = getDistance(car.pins.output.distanceLeftTrig, car.pins.input.distanceLeftEcho);
        sensors.distance.center = getDistance(car.pins.output.distanceCenterTrig, car.pins.input.distanceCenterEcho);
        sensors.distance.right = getDistance(car.pins.output.distanceRightTrig, car.pins.input.distanceRightEcho);

        // // acceleration
        // sensors.acceleration.x = device.get_accel_x();
        // sensors.acceleration.y = device.get_accel_y();
        // sensors.acceleration.z = device.get_accel_z();

        // // rotation
        // sensors.rotation.x = device.get_gyro_x();
        // sensors.rotation.y = device.get_gyro_y();
        // sensors.rotation.z = device.get_gyro_z();

        car.sensors = sensors; // update global sensor states

        // car.connection.rssi = WiFi.RSSI(); // get wifi strength
    }
}

void Task2Code(void *parameter)
{
    for (;;)
    {
        //TODO: HTTP server
    }
}

void loop() {}