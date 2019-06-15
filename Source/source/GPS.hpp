#include <iostream>
#include <string>
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

int main();
bool init_gps();
string poll();
gps_data decode();
string get_message();
bool send_message(gps_data decoded_data);
