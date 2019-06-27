#include "GPS.hpp"
#include "include/minmea.h"
#include <fstream>
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
bool poll(string *message) {
	if (gpsOn) {
	    cout << "GPS on" << endl;
		*message = get_message();
		return true;
	}
	return false;
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
    string message;
	read_nmea_from_file("nmea01.txt", &message); // set message
	return message;
}

bool send_message(gps_data decoded_data) {
	return true;
}

// dummy method that reads static data directly from a file, emulating string data to be returned by GPS hardware
bool read_nmea_from_file(string fn, string *data) {
    string nmea_paragraph("");
    string line;
    
    open_nmea_file(fn);
    
    if(nmea_datafile->is_open()) {
        //cout << "\tData file opened, reading three lines" << endl;
        for(int i=0; i<3; i++) {
            if(getline(*nmea_datafile, line)) {
                nmea_paragraph += line.substr(0, line.length()-1) + "\n";  // strip \lf and (re-)add \n
            } else {
                cout << INDENT_SPACES << "No more lines, closing data file" << endl;
                nmea_datafile->close();
                break;
            }
        }
    } else {
        cout << "Unable to open file: '" << fn << "'" << endl;
        return false;
    }
    *data = nmea_paragraph;
    return true;
}

// dummy helper method for reading static data
void close_nmea_file() {
    if(nmea_datafile) {
        nmea_datafile->close();
        nmea_datafile = NULL;
    }
}

// dummy helper method for reading static data
void open_nmea_file(string fn) {
    if(!nmea_datafile) {
        nmea_datafile = new ifstream(fn);
        cout << INDENT_SPACES << "Reading from data file " + fn << endl;
    }
}


int main() {
    string nmea_data;
    
	decode("abc"); //should be false
	decode("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47"); //should be true
	
	
	// Poll GPS once
	cout << endl << "Polling GPS... " << endl;
	init_gps();
	if(poll(&nmea_data)) {
	    cout << "\nRaw data from file: " << endl << nmea_data << endl;
	    
	    decode(nmea_data); // assumes that decode() parses paragraph to check sentence-type of each line (or that first sentence is always GGA)
	    
	    cout << "\nSanity check for first paragraph of nmea01.txt is:  45.3907333.. / -60.91918333.. degrees" << endl;
    }
    close_nmea_file();
    
    
    // Set 'true' to print verification for ALL data in file, assuming *.check file exists
    if(false) {
        string fn = "nmea01.txt";
        string paragraph, check_line;
        ifstream check_file(fn + ".check");
        open_nmea_file(fn);
        
        while(read_nmea_from_file(fn, &paragraph)) {
            decode(paragraph);
            if(check_file.is_open() && !getline(check_file, check_line))
                check_file.close();
            cout << INDENT_SPACES << INDENT_SPACES << "Sanity check: " + check_line << endl << endl;
        }
        close_nmea_file();
    }
}




