#include "GPS.hpp"
#include "include/minmea.h"
#include <fstream>
#include <sstream>
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
void decode(string raw) {
	unique_ptr<gps_data> data;
	minmea_sentence_gga ggaFrame;
	minmea_sentence_rmc rmcFrame;
	minmea_sentence_gll gllFrame;
	minmea_sentence_gst gstFrame;
	minmea_sentence_vtg vtgFrame;
	minmea_sentence_zda zdaFrame;

	const char *parse = raw.c_str();
	stringstream ss(parse);
	string to;
	if (parse != NULL) {
		while (getline(ss, to, '\n')) {
			string sentence = to;
			if (minmea_parse_gga(&ggaFrame, sentence.c_str())) {
				data->altitudeMeters = minmea_tofloat(&ggaFrame.altitude);
				data->latitude = minmea_tocoord(&ggaFrame.latitude);
				data->longitude = minmea_tocoord(&ggaFrame.longitude);
				data->time_stamp = toStringTime(&ggaFrame.time);
			}
			else if (minmea_parse_rmc(&rmcFrame, sentence.c_str())) {
				data->latitude = minmea_tocoord(&rmcFrame.latitude);
				data->longitude = minmea_tocoord(&rmcFrame.longitude);
				data->time_stamp = toStringTime(&rmcFrame.time);
				data->speedKnots =  minmea_tofloat(&rmcFrame.speed);
			}
			else if (minmea_parse_gll(&gllFrame, sentence.c_str())) {
				data->latitude = minmea_tocoord(&gllFrame.latitude);
				data->longitude = minmea_tocoord(&gllFrame.longitude);
				data->time_stamp = toStringTime(&gllFrame.time);
			}
			else if (minmea_parse_gst(&gstFrame, sentence.c_str())) {
				data->time_stamp = toStringTime(&gstFrame.time);
			}
			else if (minmea_parse_vtg(&vtgFrame, sentence.c_str())) {
				data->speedKnots = minmea_tofloat(&vtgFrame.speed_knots);
			}
			else if (minmea_parse_zda(&zdaFrame, sentence.c_str())) {
				data->time_stamp = toStringTime(&zdaFrame.time);
			}
		}
	}
	printf("time: %s\naltitude: %f\n(%f, %f)\nspeed: %f\n\n",
		data->time_stamp.c_str(), data->altitudeMeters, data->latitude, data->longitude, data->speedKnots);
}

void resetData() {
	unique_ptr<gps_data> data;
	data->time_stamp = "";
	data->altitudeMeters = 0;
	data->latitude = 0;
	data->longitude = 0;
	data->speedKnots = 0;
}

string toStringTime(struct minmea_time *time) {
	stringstream timeString;
	timeString << time->hours << ":" << time->minutes << ":" << time->seconds << ":" << time->microseconds;
	return timeString.str();
}

string get_message() {
	string message; // set message
	return message;
}

bool send_message(gps_data decoded_data) {
	return true;
}

vector<string> read_nmea_from_file() {
    vector<string> nmea_data;
    string line;
    ifstream nmea_file("nmea01.txt");
    
    if(nmea_file.is_open()) {
        while(getline(nmea_file, line)) {
            nmea_data.push_back(line);
        }
        nmea_file.close();
    } else {
        cout << "Unable to open file";
    }
    
    return nmea_data;
}

//scheduler class to send messages
int main() {
	vector<string> nmea_data;
    
	decode("abc"); //should be false
	resetData();
	decode("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47"); //should be true
	resetData();
	nmea_data = read_nmea_from_file();
	cout << "\n" << "Read data from file: " << nmea_data[0] << "\n";
	decode(nmea_data[0]);
	decode("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47\n$GPZDA,201530.00,04,07,2002,00,00*60");
	resetData();
	decode("$GPZDA,201530.00,04,07,2002,00,00*60");
	resetData();
	stringstream paragraph;
	paragraph << "$GPRMC,002454,A,3553.5295,N,13938.6570,E,0.0,43.1,180700,7.1,W,A*3F\n";
	paragraph << "$GPGGA,023042,3907.3837,N,12102.4684,W,1,04,2.3,507.3,M,-24.1,M,,*75";
	paragraph << "$GPGGA, 002454, 3553.5295, N, 13938.6570, E, 1, 05, 2.2, 18.3, M, 39.0, M, , *7F";
	paragraph << "$GPGSA, A, 3, 01, 04, 07, 16, 20, , , , , , , , 3.6, 2.2, 2.7 * 35";
	paragraph << "$GPGSV, 3, 1, 09, 01, 38, 103, 37, 02, 23, 215, 00, 04, 38, 297, 37, 05, 00, 328, 00 * 70";
	paragraph << "$GPGSV, 3, 2, 09, 07, 77, 299, 47, 11, 07, 087, 00, 16, 74, 041, 47, 20, 38, 044, 43 * 73";
	paragraph << "$GPGSV, 3, 3, 09, 24, 12, 282, 00 * 4D";
	paragraph << "$GPGLL, 3553.5295, N, 13938.6570, E, 002454, A, A * 4F";
	paragraph << "$GPRMC, 002456, A, 3553.5295, N, 13938.6570, E, 0.0, 43.1, 180700, 7.1, W, A * 3D";
	decode(paragraph.str());
	resetData();
	decode("$GPVTG, 054.7, T, 034.4, M, 005.5, N, 010.2, K * 48");
	resetData();
}




