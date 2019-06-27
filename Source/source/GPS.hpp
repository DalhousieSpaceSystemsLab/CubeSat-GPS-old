#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "include/minmea.h"

//Gathered from https://github.com/AndrewWay/cubesat/tree/develop on June 25th, 2019
#include "Message.h"
#include "MessageBuilder.h"
#include "MessageSerializer.h"
#include "MessageSenderInterface.h"
#include "GPS_Data_Types.h"
#include <fstream>
#define INDENT_SPACES "  "

using namespace std;
struct gps_data {
    string time_stamp;
    float latitude;
    float longitude;
    float altitudeMeters;
    float speedKnots;
};
bool gpsOn;

int main();
bool init_gps();
string poll();
gps_data decode(string raw);
string get_message();
bool send_message(gps_data decoded_data);
vector<string> read_nmea_from_file();
string toStringTime(struct minmea_time *time);
