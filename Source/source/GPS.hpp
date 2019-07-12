#ifndef GPS_H
#define GPS_H

#include <iostream>
#include <string>
#include <cstring>
#include <vector>

#include "minmea.h"
#include "SanityCheck.h"
#include "Testing.hpp"
//#include "include/minmea.h"

//Gathered from https://github.com/AndrewWay/cubesat/tree/develop on July 4th, 2019
#include "Message.h"
#include "MessageBuilder.h"
//#include "MessageSenderInterface.h"
#include "GPS_Data_Types.h"
#include <fstream>
#include <sstream>
#define INDENT_SPACES "  "

static GPS_Data_Types data_types;

using namespace std;
struct gps_data {
    string time_stamp;
    float latitude;
    float longitude;
    float altitude;
    float height;
    int time;
};
static bool gpsOn;

bool init_gps();
bool poll(string *message);
gps_data decode(string raw);
bool send_message(gps_data decoded_data);
string toStringTime(struct minmea_time *time);

void build_message(gps_data decoded_data, Message *message);
int main();
#endif
