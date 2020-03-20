#ifndef LORIS_GPS_MESSAGE_INTERFACE
#define LORIS_GPS_MESSAGE_INTERFACE

#include "Message.h"
#include "DataMessage.h"
#include "PhoneBook.h"
#include "CommandMessage.h"
#include "MessageBuilder.h"
#include "MessageReceivingService.h"
#include "GPSDataTypes.hpp"
#include <string>
#include <iostream>
#include <math.h>
#include <chrono>
#include "Identifiers.h"

using namespace std;

bool send_message(gps_data* decoded_data);
CommandMessage get_message();

#endif

#ifndef INDENT_SPACES
#define INDENT_SPACES "  "
#endif

#ifndef CURRENT_TIME
#define CURRENT_TIME chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count()
#endif