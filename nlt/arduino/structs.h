struct Distance
{
    float left;
    float center;
    float right;
};

struct Pins
{
    struct Input
    {
        int distanceLeftEcho;
        int distanceCenterEcho;
        int distanceRightEcho;
        int leftForward;
        int leftBackward;
        int rightForward;
        int rightBackward;
    };
    Input input;

    struct Output
    {
        int distanceLeftTrig;
        int distanceCenterTrig;
        int distanceRightTrig;
        int leftForward;
        int leftBackward;
        int rightForward;
        int rightBackward;
    };
    Output output;
};

struct Car
{
    struct Connection
    {
        char* ip;
        long rssi;
    };
    Connection connection;
    Pins pins;
    Distance distance;
    char* Id;
};

struct Conn
{
    char* ssid;
    char* password;
};