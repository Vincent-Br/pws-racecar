struct Sensors
{
    struct Rotation
    {
        float x;
        float y;
        float z;
    };
    Rotation rotation;

    struct Acceleration
    {
        float x;
        float y;
        float z;
    };
    Acceleration acceleration;

    struct Distance
    {
        float left;
        float center;
        float right;
    };
    Distance distance;

    int lastUpdate;
};

struct Position
{
    float x;
    float y;
    float z;
};

struct Pins
{
    struct Input
    {
        int distanceLeftEcho;
        int distanceCenterEcho;
        int distanceRightEcho;
    };
    Input input;

    struct Output
    {
        int distanceLeftTrig;
        int distanceCenterTrig;
        int distanceRightTrig;
    };
    Output output;
};

struct Car
{
    struct Connection {
        IPAddress ip;
        long rssi;
    };
    Connection connection;
    Pins pins;
    Sensors sensors;
    Position position;
    String Id;
};

struct Conn
{
    String ssid;
    String password;
};