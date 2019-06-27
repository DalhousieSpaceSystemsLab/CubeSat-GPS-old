//Make sure to update cubesat/core/src/message/identifiers/GPSKeys.h with modifications

struct GPS_Data_Types {
    int type = 0;
    int time_stamp = 1;
    int latitude = 2;
    int longitude = 3;
    int num_sats = 4;
    int horizontal_dilution = 5;
    int altitude = 6;
};
