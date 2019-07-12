#include "GPS.hpp"

#define INDENT_SPACES "  "

gps_data data;

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
bool poll(string *message) {
    if (init_gps()) {
        cout << "GPS on" << endl;
        return read_nmea_from_file(message);
    }
    return false;
}

//send in a NMEA string, get organized gps_data
void decode(string raw) { 
	data = {};
	minmea_sentence_gga ggaFrame;
	minmea_sentence_zda zdaFrame;

	const char* parse = raw.c_str();
	stringstream ss(parse);
	string to;
	if (parse != NULL) {
		while (getline(ss, to, '\n')) {
			string sentence = to;
			if (minmea_parse_gga(&ggaFrame, sentence.c_str())) {
				data.altitude = minmea_tofloat(&ggaFrame.altitude);
				data.latitude = minmea_tocoord(&ggaFrame.latitude);
				data.longitude = minmea_tocoord(&ggaFrame.longitude);
				data.height = minmea_tofloat(&ggaFrame.height);
				data.time_stamp = toStringTime(&ggaFrame.time);
			}
			if (minmea_parse_zda(&zdaFrame, sentence.c_str())) {
				timespec ts;
				minmea_gettime(&ts, &zdaFrame.date, &zdaFrame.time);
				data.time = ts.tv_sec;
			}
		}
	}
}

string toStringTime(struct minmea_time *time) {
	stringstream timeString;
	timeString << time->hours << ":" << time->minutes << ":" << time->seconds << ":" << time->microseconds;
	return timeString.str();
}

//scheduler class to send messages
void test() {
	//poll()
	decode("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47");
	send_message(&data);

	decode("$GPGGA,012219,1237.038,N,01531.000,E,1,08,0.9,125.4,M,46.9,M,,*47\n$GPZDA,201530.00,04,07,2002,00,00*60");
	stringstream paragraph;
	paragraph << "$GPGGA,023042,4007.3837,N,12102.89684,W,1,04,2.3,507.3,M,-24.1,M,,*75\n";
	paragraph << "$GPGGA,023042,3907.3837,N,12102.4684,W,1,04,2.3,507.3,M,-24.1,M,,*75\n";
	paragraph << "$GPGGA,022454,3553.5295,N,13938.6570,E,1,05,2.2,18.3,M,39.0,M,,*7F\n";

	decode(paragraph.str());
	
	
    // Set 'true' to print verification for ALL data in nmea file, assuming associated *.check file exists (see SanityCheck.h)
    if(false) {
        string paragraph, check_line;
        ifstream check_file(nmea_filename + ".check");
        
        close_nmea_file(); // reset file stream
        while(poll(&paragraph)) {
			decode(paragraph);
            if(send_message(&data)) {
                if(check_file.is_open() && !getline(check_file, check_line))
                    check_file.close();
                else
                    cout << INDENT_SPACES << INDENT_SPACES << "Sanity check: " + check_line << endl << endl;
            }
        }
    }
}

int gps_loop() {
	string raw_nmea;
	cout << "Initializing GPS..." << endl;
	if (!init_gps()) {
		cout << "INITIALIZING FAILED" << endl;
		return 0;
	}
	cout << "GPS Initialized!" << endl;
	cout << "Polling GPS..." << endl;
	if (!poll(&raw_nmea)) {
		cout << "POLL FAILED" << endl;
		close_nmea_file();
		return 0;
	}
	cout << "GPS Polled!";
	cout << "Decoding NMEA Data";
	decode(raw_nmea);
	cout << "Sending results...";
	if (!send_message(&data)) {
		cout << "MESSAGE FAILED TO SEND" << endl;
		return 0;
	}
	cout << "Message sent!" << endl;
	return 1;
}

int main(int argc, char *argv[]) {
	if (argc == 2 && strcmp(argv[1], "-t")) {
		test();
		return 1;
	}
	chrono::system_clock::time_point last_poll = chrono::system_clock::now();
	KeyValuePairContainer message;
	string raw_nmea;
	bool messageSuccess = false;

	while (true) {
		messageSuccess = false;
		if (get_message(&message)) {
			if (message.GetInt(0) == 0) {
				messageSuccess = true;
				cout << endl << "== Received status: " << message.GetInt(0) << "! ==" << endl << endl;
				if (gps_loop()) {
					cout << "SUCCESS" << endl;
					last_poll = chrono::system_clock::now();
				}
				else
					cout << "FAILURE" << endl;
			}
			else {
				cout << "Invalid message sent" << endl;
				cout << "Received status: " << message.GetInt(0);
			}
		}

		if (!messageSuccess) {
			this_thread::sleep_for(chrono::seconds(5));
			int timediff = 1 - chrono::duration_cast<chrono::minutes>(chrono::system_clock::now() - last_poll).count(); //12 hours in minutes
			if (timediff == 0) {
				cout << endl << "== Timeout elapsed == " << endl << endl;
				cout << "Updating GPS Data..." << endl;
				if (gps_loop()) {
					cout << endl << "== SUCCESS == " << endl << endl;
					last_poll = chrono::system_clock::now();
				}
				else {
					cout << endl << "== FAILURE ==" << endl << endl;
					cout << "Trying again in 5 minutes" << endl;
					last_poll -= chrono::minutes(5);
				}
			}
			else {
				cout << "Sleeping for " << timediff << " more minutes..." << endl;
			}
		}

	}
}
	





