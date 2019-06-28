#include <iostream>
#include <string>
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
    int type;
    int time_stamp;
    float latitude;
    float longitude;
    int num_sats;
    float horizontal_dilution;
    float altitude;
};

bool gpsOn;

int main();
bool init_gps();
bool poll(string *message);
gps_data decode(string raw);
bool send_message(gps_data decoded_data);

// testing-only declarations follow, to be removed once we have GPS hardware interface
ifstream *nmea_datafile = NULL;
string nmea_filename = "nmea01.txt";

bool read_nmea_from_file(string *data);
void close_nmea_file();
void open_nmea_file(string fn);
