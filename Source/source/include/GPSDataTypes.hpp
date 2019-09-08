#ifndef LORIS_GPS_DATA_TYPES
#define LORIS_GPS_DATA_TYPES
#include <string>

struct GPS_Data_Types {
    int time = 0;
    int latitude = 1;
    int longitude = 2;
    int height = 3;
    int altitude = 4;
};

struct status_codes {
    int request = 200;
    int shutdown = 400;
    int standby = 300;
};

struct gps_data {
    float latitude;
    float longitude;
    float altitude;
    float height;
    unsigned int time;
    float fix_quality;
};

struct gps_time {
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;
    unsigned int microseconds;
};

#endif
