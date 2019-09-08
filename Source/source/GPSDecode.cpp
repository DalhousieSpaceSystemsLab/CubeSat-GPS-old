#include "GPSDecode.hpp"
gps_time decode_time_from_int(unsigned int time_int) {
    unsigned int hours, minutes, seconds;
    unsigned int BITS = 32;
    unsigned int H_BITS = 5;
    unsigned int M_BITS = 6;
    unsigned int S_BITS = 6;
    struct gps_time time = {};
    
    time.seconds = time_int << (BITS - S_BITS) >> (BITS - S_BITS);
    time.minutes = time_int << (BITS - M_BITS - S_BITS) >> (BITS - M_BITS);
    time.hours = time_int << (BITS - H_BITS - M_BITS - S_BITS) >> (BITS - H_BITS);
    
    return time;
}