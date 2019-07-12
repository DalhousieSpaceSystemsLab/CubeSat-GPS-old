#ifndef LORIS_GPS_DATA_TYPES
#define LORIS_GPS_DATA_TYPES
#include <string>

struct GPS_Data_Types {
    int time_stamp = 0;
    int latitude = 1;
    int longitude = 2;
    int height = 3;
    int altitude = 4;
};

struct gps_data {
    std::string time_stamp;
    float latitude;
    float longitude;
    float altitude;
    float height;
    float time;
};
#endif
