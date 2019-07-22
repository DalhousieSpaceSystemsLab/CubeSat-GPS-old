#ifndef LORIS_GPS_MESSAGE_INTERFACE
#define LORIS_GPS_MESSAGE_INTERFACE

#include "Message.h"
#include "MessageBuilder.h"
#include "GPSDataTypes.hpp"
#include <string>
#include <iostream>
#include <math.h>
#include <chrono>
#include "Identifiers.h"

using namespace std;
//#include "MessageSenderInterface.h"

bool send_message(gps_data* decoded_data);
Message get_message();

#endif

#ifndef INDENT_SPACES
#define INDENT_SPACES "  "
#endif