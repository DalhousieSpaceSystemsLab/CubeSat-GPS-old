#include "GPS.hpp"

#define INDENT_SPACES "  "

gps_data data;
auto start_time = std::chrono::high_resolution_clock::now();
std::chrono::duration<double> elapsed = start_time - start_time;

//checks if GPS is turned on, if not, turn on. 
//returns TRUE if successful, FALSE otherwise
bool init_gps() {
    if (!gpsOn) {
        //if can turn on gps
        cout << "GPS cold start begin" << endl;
        gpsOn = true;
	
	elapsed = std::chrono::high_resolution_clock::now() - start_time;
	cout << "GPS started - TIME:" << elapsed.count() << endl;
        return true;
        //if cant turn on gps, return false
    }
    if (gpsOn) {
        cout << "GPS warm start begin" << endl;
        return true;
    }
    return false;
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
	data = (const struct gps_data) {};
	minmea_sentence_gga ggaFrame;
	minmea_sentence_zda zdaFrame;
	elapsed = std::chrono::high_resolution_clock::now() - start_time;
	cout << "GPS decode: begin parsing sentence - TIME:" << elapsed.count() << endl;

	const char* parse = raw.c_str();
	stringstream ss(parse);
	string to;

	elapsed = std::chrono::high_resolution_clock::now() - start_time;
	cout << "GPS decode: end parsing sentence and start storing data - TIME:" << elapsed.count() << endl;
	
	if (parse != NULL) {
		while (getline(ss, to, '\n')) {
			string sentence = to;
			if (minmea_parse_gga(&ggaFrame, sentence.c_str()) && minmea_check(sentence.c_str(), false)) {
				data.altitude = minmea_tofloat(&ggaFrame.altitude);
				data.latitude = minmea_tocoord(&ggaFrame.latitude);
				data.longitude = minmea_tocoord(&ggaFrame.longitude);
				data.height = minmea_tofloat(&ggaFrame.height);
				data.time = encode_time_as_int(&ggaFrame.time);
				data.fix_quality = ggaFrame.fix_quality;
			}
			else if (minmea_parse_zda(&zdaFrame, sentence.c_str())) {
				data.time = encode_time_as_int(&zdaFrame.time);
				//TODO - add date information from ZDA to data struct
			}
		}
	}
}

bool check_gps_data(gps_data data) {
	if (data.fix_quality == 0 ||
		isnan(data.altitude) || isnan(data.height) ||
		isnan(data.latitude) || data.latitude < -90 || data.latitude > 90 ||
		isnan(data.longitude) || data.longitude < -180 || data.longitude > 180
		) {
		return false;
	}
	return true;
}

string toStringTime(struct gps_time *time) {
	stringstream timeString;
	timeString << time->hours << ":" << time->minutes << ":" << time->seconds << ":" << time->microseconds;
	return timeString.str();
}


unsigned int encode_time_as_int(struct minmea_time *time) {
    unsigned int BITS = 32;
    unsigned int H_BITS = 5;
    unsigned int M_BITS = 6;
    unsigned int S_BITS = 6;
    unsigned int enc = 0;
    unsigned int hours = (unsigned int)(time->hours);
    unsigned int minutes = (unsigned int)(time->minutes);
    unsigned int seconds = (unsigned int)(time->seconds);
    
    enc = enc | (hours << (BITS - H_BITS) >> (BITS - H_BITS - M_BITS - S_BITS));
    enc = enc | (minutes << (BITS - M_BITS) >> (BITS - M_BITS - S_BITS));
    enc = enc | (seconds << (BITS - S_BITS) >> (BITS - S_BITS));
    
    return enc;
}

//scheduler class to send messages
void test() {
	cout << "== TEST MODE ==" << endl;
	//poll()
	decode("$GPGGA,184911.211,4448.547,N,09323.027,W,1,12,1.0,0.0,M,0.0,M,,*7E");
	send_message(&data);
	string rawnmea;
	poll(&rawnmea);
	send_message(&data);
	poll(&rawnmea);
	send_message(&data);
	poll(&rawnmea);
	send_message(&data);
	poll(&rawnmea);
	send_message(&data);


	decode("$GPGGA,012219,1237.038,N,01531.000,E,1,08,0.9,125.4,M,46.9,M,,*47\n$GPZDA,201530.00,04,07,2002,00,00*60");
	stringstream paragraph;
	paragraph << "$GPGGA,023042,4007.3837,N,12102.89684,W,1,04,2.3,507.3,M,-24.1,M,,*75\n";
	paragraph << "$GPGGA,023042,3907.3837,N,12102.4684,W,1,04,2.3,507.3,M,-24.1,M,,*75\n";
	paragraph << "$GPGGA,022454,3553.5295,N,13938.6570,E,1,05,2.2,18.3,M,39.0,M,,*7F\n";

	decode(paragraph.str());
	send_message(&data);
	
	
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
	start_time = std::chrono::high_resolution_clock::now();
	cout << "Start gps_loop" << endl;
	
	cout << "Initializing GPS..." << endl;
	if (!init_gps()) {
		cout << "INITIALIZING FAILED" << endl;
		return 0;
	}
	cout << "GPS Initialized!" << endl;

	elapsed = std::chrono::high_resolution_clock::now() - start_time;
	cout << "GPS initialized, start poll - TIME:" << elapsed.count() <<endl;
	
	cout << "Polling GPS..." << endl;
	if (!poll(&raw_nmea)) {
		cout << "POLL FAILED" << endl;
		close_nmea_file();
		return 0;
	}
	cout << "GPS Polled!" << endl;

	elapsed = std::chrono::high_resolution_clock::now() - start_time;
	cout << "GPS poll finished, start decode - TIME:" << elapsed.count() << endl;
	
	cout << "Decoding NMEA Data" << endl;
	decode(raw_nmea);

	elapsed = std::chrono::high_resolution_clock::now() - start_time;
	cout << "GPS decode finished, start sending - TIME:" << elapsed.count() << endl;
	
	cout << "Sending results..." << endl;
	if (!send_message(&data)) {
		cout << "MESSAGE FAILED TO SEND" << endl;
		return 0;
	}
	cout << "Message sent!" << endl;

	elapsed = std::chrono::high_resolution_clock::now() - start_time;
	cout << "GPS message sent - TOTAL TIME:" << elapsed.count() << endl;
	return 1;
}

int main(int argc, char *argv[]) {
	if (argc == 2 && (strcmp(argv[1], "-t") == 0)) {
		test();
		return 1;
	}
	cout << "== NORMAL MODE ==" << endl;

	status_codes codes;
	chrono::system_clock::time_point last_poll = chrono::system_clock::now();
	CommandMessage message;
	string raw_nmea;
	bool messageSuccess = false;

	while (true) {
		messageSuccess = false;
		cout << "Checking for messages..." << endl;
		message = get_message();
		if (message.GetMessageContents().GetAmountofIntPairs() != 0) {
			cout << endl << "== Received status: " << message.GetInt(0) << "! ==" << endl << endl;
			if (message.GetInt(0) == codes.request) {
				cout << "REQUEST RECEIVED - Initiating GPS Sequence" << endl;;
				messageSuccess = true;
				
				if (gps_loop()) {
					cout << endl << "== SUCCESS == " << endl << endl;
					last_poll = chrono::system_clock::now();
				}
				else
					cout << endl << "== FAILURE ==" << endl << endl;
			}
			else if (message.GetInt(0) == codes.standby) {
				cout << "STANDBY RECEIVED - Awaiting further instruction" << endl;
			}
			else {
				cout << "Invalid message sent" << endl;
				cout << "Received status: " << message.GetInt(0);
			}
		}
		else {
			cout << "EMPTY REQUEST - Awaiting further instruction" << endl;
		}

		if (!messageSuccess) {
			this_thread::sleep_for(chrono::seconds(5));
			//for testing, this is just 2 minutes, will be 720 (12 hours) in production
			int timediff = 2 - chrono::duration_cast<chrono::minutes>(chrono::system_clock::now() - last_poll).count(); //12 hours in minutes
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
				cout << "Sleeping for " << timediff << " more minutes..." << endl << endl;
			}
		}

	}
}
	





