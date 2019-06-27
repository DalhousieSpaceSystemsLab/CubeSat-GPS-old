#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "minmea.h"

using namespace std;
struct gps_data {
    static string time_stamp;
    static float latitude;
    static float longitude;
    static float altitudeMeters;
	static float speedKnots;
};
string gps_data::time_stamp = "";
float gps_data::latitude = 0;
float gps_data::longitude = 0;
float gps_data::altitudeMeters = 0;
float gps_data::speedKnots = 0;

bool gpsOn;

int main();
bool init_gps();
string poll();
void decode(string raw);
void resetData();
string get_message();
bool send_message(gps_data decoded_data);
vector<string> read_nmea_from_file();
string toStringTime(struct minmea_time *time);
