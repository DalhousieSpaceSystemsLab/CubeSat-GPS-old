#ifndef sanitycheck_h
#define sanitycheck_h


#include <iostream>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

// testing-only declarations follow, to be removed once we have GPS hardware interface
static ifstream *nmea_datafile = NULL;
static ifstream *nmea_checkfile = NULL;
static string nmea_filename = "nmea01.txt"; // alter filename as needed

bool read_nmea_from_file(string *data);
bool read_sanitycheck_from_file(string *data);
void reset_datafile();
void open_file(string fn, ifstream *nmea_file);

ifstream* open_datafile();
ifstream* open_checkfile();
void close_datafile();
void close_checkfile();

#endif
