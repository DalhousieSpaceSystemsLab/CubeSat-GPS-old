#ifndef GPS_TESTING_H
#define GPS_TESTING_H

#include "GPS.hpp"
#include "SanityCheck.h"


bool compare_floats(float a, float b);
float parse_token_float(string line, string separator, unsigned int index);

bool test_decode(bool verbose);
bool test_build_message(bool verbose);
void print_test_summary(string label, bool success);

#endif
