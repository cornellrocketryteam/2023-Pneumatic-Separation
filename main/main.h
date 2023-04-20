#ifndef MAIN_h
#define MAIN_h

struct SYSTEMS
{
    struct GPS
    {
        long LAT;
        long LONG;
        long ALT;
        byte SIV;
        unsigned long POLL_LASTTIME;
        int POLL_DELAY;
        bool INIT;
    } GPS;
    struct ALTIMETER
    {
        double REF_PRESURE;
        double TEMP;
        double PRESSURE;
        double ABS_ALTITUDE;
        double REL_ALTITUDE;
        bool INIT;
    } ALTIMETER;
    struct ACCEL
    {
        double X;
        double Y;
        double Z;
        bool INIT;
    } ACCEL;
    struct GYRO
    {
        double X;
        double Y;
        double Z;
        double PITCH;
        double ROLL;
        double HEADING;
        bool INIT;
    } GYRO;
    struct MAG
    {
        double X;
        double Y;
        double Z;
        bool INIT;
    } MAG;
    struct SD
    {
        unsigned long WRITE_LASTTIME;
        long WRITE_DELAY;
        bool INIT;
    } SD;
    struct RF
    {
        int TRANSMISSION_STATE;
        bool INIT;
    } RF;
};

#endif
