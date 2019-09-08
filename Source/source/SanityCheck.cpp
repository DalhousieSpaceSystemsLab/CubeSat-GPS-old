#include "SanityCheck.h"

// reads static data directly from a file, emulating string data to be returned by GPS hardware
bool read_nmea_from_file(string *data) {
    string nmea_paragraph("");
    string line;
    
    open_nmea_file(nmea_filename);
    
    if(nmea_datafile->is_open()) {
        for(int i=0; i<3; i++) {
            if(getline(*nmea_datafile, line)) {
                nmea_paragraph += line.substr(0, line.length()-1) + "\n";  // strip trailing endline
            } else {
                cout << "  No more lines, closing data file" << endl;
                nmea_datafile->close();
                break;
            }
        }
    } else {
        cout << "Unable to open file: '" << nmea_filename << "'" << endl;
    }
    *data = nmea_paragraph;
    return (nmea_paragraph.length() > 0);
}

// helper method for reading static data
void close_nmea_file() {
    if(nmea_datafile) {
        nmea_datafile->close();
        nmea_datafile = NULL;
    }
}

// helper method for reading static data
void open_nmea_file(string fn) {
    if(!nmea_datafile) {
        nmea_datafile = new ifstream("../../../../source/resources/nmea_data/" + fn); // visual studio
        if(!nmea_datafile->good()) {
            nmea_datafile->open("../source/resources/nmea_data/" + fn); // unix
        }
        cout << "  Reading from data file " + fn << endl;
    }
}
