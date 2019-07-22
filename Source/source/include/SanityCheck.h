#include <iostream>
#include <string>
#include <cstring>
#include <fstream>

using namespace std;

// testing-only declarations follow, to be removed once we have GPS hardware interface
static ifstream *nmea_datafile = NULL;
static string nmea_filename = "nmea02.txt"; // alter filename as needed

bool read_nmea_from_file(string *data);
void close_nmea_file();
void open_nmea_file(string fn);
