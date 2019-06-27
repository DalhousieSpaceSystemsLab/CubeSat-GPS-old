#include <iostream>
#include <string>
#include <string>
#include <cstring>
#include <vector>

using namespace std;
struct gps_data {
    string type;
    int time_stamp;
    string latitude;
    string longitude;
    int num_sats;
    float horizontal_dilution;
    float altitude;
};

bool gpsOn;
ifstream *nmea_datafile = NULL;

int main();
bool init_gps();
bool poll(string *message);
gps_data decode(string raw);
string get_message();
bool send_message(gps_data decoded_data);

bool read_nmea_from_file(string fn, string *data);
void close_nmea_file();
void open_nmea_file(string fn);
