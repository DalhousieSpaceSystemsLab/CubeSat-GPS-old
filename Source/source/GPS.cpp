#include "GPS.hpp"
#include "minmea.h"
#include "minmea.c"
#include <string>
#include <cstring>
#include <vector>

//scheduler class to send messages
int main() {
	decode("abc"); //should be false
	decode ("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47"); //should be true

}
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
	const char* d = raw.c_str();
	if (minmea_check(d, false)) {
		cout << "true" << endl;
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





