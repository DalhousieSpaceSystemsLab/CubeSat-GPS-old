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

using namespace std;
//#include "MessageSenderInterface.h"

chrono::system_clock::time_point last_poll = chrono::system_clock::now();
chrono::system_clock::time_point last_empty_poll = chrono::system_clock::now();
bool send_message(gps_data* decoded_data);
bool get_message(KeyValuePairContainer* message);
#endif