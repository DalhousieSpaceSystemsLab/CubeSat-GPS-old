#include "GPS.hpp"
#include "include/minmea.h"
#define INDENT_SPACES "  "

//checks if GPS is turned on, if not, turn on. 
//returns TRUE if successful, FALSE otherwise
bool init_gps() {
	if (!gpsOn) {
		//if can turn on gps
		gpsOn = true;
		return true;
		//if cant turn on gps, return false
	}
	return true;
}

//polls the GPS, gets data if turned on
string poll() {
	if (gpsOn) {
		return get_message();
	}
	return NULL;
}

//send in a NMEA string, get organized gps_data
gps_data decode(string raw) {
	minmea_sentence_gga frame;
	if (minmea_parse_gga(&frame, raw.c_str())) {
		cout << "true" << endl;
		printf(INDENT_SPACES "$xxGGA: raw coordinates : (%d/%d,%d/%d)\n",
                            frame.latitude.value, frame.latitude.scale,
                            frame.longitude.value, frame.longitude.scale);
        printf(INDENT_SPACES "$xxGGA fixed-point coordinates scaled to three decimal places: (%d,%d)\n",
                            minmea_rescale(&frame.latitude, 1000),
                            minmea_rescale(&frame.longitude, 1000));
        printf(INDENT_SPACES "$xxGGA floating point degree coordinates: (%f,%f)\n",
                            minmea_tocoord(&frame.latitude),
                            minmea_tocoord(&frame.longitude));
	}
	else {
		cout << "false" << endl;
	}
	gps_data data;
	return data;
}

string get_message() {
	string message; // set message
	return message;
}

bool send_message(gps_data decoded_data) {
	return true;
}

//scheduler class to send messages
int main() {
	decode("abc"); //should be false
	decode("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47"); //should be true
}




