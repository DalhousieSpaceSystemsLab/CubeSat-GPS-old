#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "minmea.h"

//Gathered from https://github.com/AndrewWay/cubesat/tree/develop on June 25th, 2019
#include "Message.h"
#include "MessageBuilder.h"
#include "MessageSerializer.h"
//#include "MessageSenderInterface.h"
#include "GPS_Data_Types.h"
#include <fstream>
#define INDENT_SPACES "  "

#include "SanityCheck.h" // Only needed for testing

using namespace std;
struct gps_data {
    string time_stamp;
    float latitude;
    float longitude;
    float altitude;
    float height;
};
bool gpsOn;

int main();
bool init_gps();
bool poll(string *message);
gps_data decode(string raw);
bool send_message(gps_data decoded_data);
string toStringTime(struct minmea_time *time);
