#ifndef LORIS_GPS_MESSAGE_INTERFACE
#define LORIS_GPS_MESSAGE_INTERFACE

#define INDENT_SPACES "  "
#include "Message.h"
#include "MessageBuilder.h"
#include "GPS_Data_Types.h"
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