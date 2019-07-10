#include "SanityCheck.h"

// reads static data directly from a file, emulating string data to be returned by GPS hardware
bool read_nmea_from_file(string *data) {
    string nmea_paragraph("");
    string line;
    
    nmea_datafile = open_datafile();
    //open_file(nmea_filename, nmea_datafile);
    
    if(nmea_datafile->is_open()) {
        for(int i=0; i<3; i++) {
            if(getline(*nmea_datafile, line)) {
                nmea_paragraph += line.substr(0, line.length()-1) + "\n";  // strip trailing endline
            } else {
                cout << "  No more lines, closing data file" << endl;
                close_datafile();
                break;
            }
        }
    } else {
        cout << "Unable to open file: '" << nmea_filename << "'" << endl;
    }
    *data = nmea_paragraph;

    return (nmea_paragraph.length() > 0);
}



bool read_sanitycheck_from_file(string *data) {
    nmea_checkfile = open_checkfile();
    
    if(nmea_checkfile->is_open()) {
        if(getline(*nmea_checkfile, *data)) {
            return true;
        } else {
            close_checkfile();
            return false;
        }
    } else {
        return false;
    }
}


// helper method for reading static data
void reset_datafile() {
    if(nmea_datafile)
        nmea_datafile->seekg(0, nmea_datafile->beg);
}

// helper method for reading static data
void open_file(string fn, ifstream *nmea_file) {
    if(nmea_file) {
        nmea_file->open("../../../../source/resources/nmea_data/" + fn); // visual studio
        if(!nmea_file->good()) {
            nmea_file->open("../source/resources/nmea_data/" + fn); // unix
        }
        if(!nmea_file->good())
            cout << "  Error reading or EOF for " + fn << endl;
    } else {
        cout << "  File already opened? (null ptr)" + fn << endl;
    }
}


ifstream* open_datafile() {
    if(!nmea_datafile) {
        nmea_datafile = new ifstream();
        open_file(nmea_filename, nmea_datafile);
    }
    return nmea_datafile;
}

ifstream* open_checkfile() {
    if(!nmea_checkfile) {
        nmea_checkfile = new ifstream();
        open_file(nmea_filename + ".check", nmea_checkfile);
    }
    return nmea_checkfile;
}

void close_datafile() {
    if(nmea_datafile)
        nmea_datafile->close();
    nmea_datafile = NULL;
}

void close_checkfile() {
    if(nmea_checkfile)
        nmea_checkfile->close();
    nmea_checkfile = NULL;
}
