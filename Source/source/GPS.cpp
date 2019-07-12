#include "GPS.hpp"
#include "include/minmea.h"
#include <fstream>
#include <sstream>
#define INDENT_SPACES "  "

GPS_Data_Types data_types;

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
    if (init_gps()) {
        cout << "GPS on" << endl;
        return read_nmea_from_file(message);
    }
    return false;
}

//send in a NMEA string, get organized gps_data
gps_data decode(string raw) { 
	gps_data data = {};
	minmea_sentence_gga ggaFrame;

	const char* parse = raw.c_str();
	stringstream ss(parse);
	string to;
	if (parse != NULL) {
		while (getline(ss, to, '\n')) {
			string sentence = to;
			if (minmea_parse_gga(&ggaFrame, sentence.c_str())) {
				data.altitude = minmea_tofloat(&ggaFrame.altitude);
				data.latitude = minmea_tocoord(&ggaFrame.latitude);
				data.longitude = minmea_tocoord(&ggaFrame.longitude);
				data.height = minmea_tofloat(&ggaFrame.height);
				data.time_stamp = toStringTime(&ggaFrame.time);
				data.time = encode_time_as_int(&ggaFrame.time);
			}
		}
	}
	struct gps_time decoded_time = decode_time_from_int(data.time);
	printf("time: %s\nenc_time: %s\naltitude: %f\n(%f, %f)\nheight: %f\n\n",
	data.time_stamp.c_str(), toStringTime(&decoded_time).c_str(), data.altitude, data.latitude, data.longitude, data.height);
	
	return data;
}

string toStringTime(struct minmea_time *time) {
	stringstream timeString;
	timeString << time->hours << ":" << time->minutes << ":" << time->seconds << ":" << time->microseconds;
	return timeString.str();
}

string toStringTime(struct gps_time *time) {
	stringstream timeString;
	timeString << time->hours << ":" << time->minutes << ":" << time->seconds << ":" << time->microseconds;
	return timeString.str();
}


unsigned int encode_time_as_int(struct minmea_time *time) {
    unsigned int BITS = 32;
    unsigned int H_BITS = 5;
    unsigned int M_BITS = 6;
    unsigned int S_BITS = 6;
    unsigned int enc = 0;
    unsigned int hours = (unsigned int)(time->hours);
    unsigned int minutes = (unsigned int)(time->minutes);
    unsigned int seconds = (unsigned int)(time->seconds);
    
    enc = enc | (hours << (BITS - H_BITS) >> (BITS - H_BITS - M_BITS - S_BITS));
    enc = enc | (minutes << (BITS - M_BITS) >> (BITS - M_BITS - S_BITS));
    enc = enc | (seconds << (BITS - S_BITS) >> (BITS - S_BITS));
    
    return enc;
}


struct gps_time decode_time_from_int(unsigned int time_int) {
    unsigned int hours, minutes, seconds;
    unsigned int BITS = 32;
    unsigned int H_BITS = 5;
    unsigned int M_BITS = 6;
    unsigned int S_BITS = 6;
    struct gps_time time = {};
    
    time.seconds = time_int << (BITS - S_BITS) >> (BITS - S_BITS);
    time.minutes = time_int << (BITS - M_BITS - S_BITS) >> (BITS - M_BITS);
    time.hours = time_int << (BITS - H_BITS - M_BITS - S_BITS) >> (BITS - H_BITS);
    
    return time;
}


bool send_message(gps_data decoded_data) {
	MessageBuilder messageBuilder;
	messageBuilder.StartMessage();

	KeyValuePairContainer container;
	container.AddKeyValuePair(data_types.latitude, decoded_data.latitude);
	container.AddKeyValuePair(data_types.longitude, decoded_data.longitude);
	//container.AddKeyValuePair(data_types.time_stamp, decoded_data.time_stamp);
	container.AddKeyValuePair(data_types.height, decoded_data.height);
	container.AddKeyValuePair(data_types.altitude, decoded_data.altitude);
	

	messageBuilder.SetMessageContents(container);
	messageBuilder.SetRecipient(1097346);
	messageBuilder.SetSender(6858902);
	

	Message message = messageBuilder.CompleteMessage();

	char msg[256] = "";
	message.flatten(msg);
	
	//SerializeMessage(&message, msg);
	//MessageSenderInterface ms(message.GetRecipient());
	//ms.SendMessage(msg);

	
	message = Message(msg);

	//vector<int> keys = message.GetMessageContents().GetKeys();

	KeyValuePairContainer c = message.GetMessageContents();
	
	cout << "\nRECIPIENT: " << message.GetRecipient() << endl
		<< "SENDER: " << message.GetSender() << endl
		<< "TIME CREATED: " << message.GetTimeCreated() << endl
		<< "CONTENTS:" << endl
		//<< INDENT_SPACES << "TIME STAMP: " << message.GetMessageContents().GetInt(data_types.time_stamp) << endl
		<< INDENT_SPACES << "LATITUDE: " << message.GetMessageContents().GetFloat(data_types.latitude) << endl
		<< INDENT_SPACES << "LONGITUDE: " << message.GetMessageContents().GetFloat(data_types.longitude) << endl
		<< INDENT_SPACES << "HEIGHT: " << message.GetMessageContents().GetFloat(data_types.height) << endl
		<< INDENT_SPACES << "ALTITUDE: " << message.GetMessageContents().GetFloat(data_types.altitude) << endl;

	return true;
}


//scheduler class to send messages
int main() {
	send_message(decode("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47"));

	decode("$GPGGA,012219,1237.038,N,01531.000,E,1,08,0.9,125.4,M,46.9,M,,*47\n$GPZDA,201530.00,04,07,2002,00,00*60");
	stringstream paragraph;
	paragraph << "$GPGGA,023042,4007.3837,N,12102.89684,W,1,04,2.3,507.3,M,-24.1,M,,*75\n";
	paragraph << "$GPGGA,023042,3907.3837,N,12102.4684,W,1,04,2.3,507.3,M,-24.1,M,,*75\n";
	paragraph << "$GPGGA,022454,3553.5295,N,13938.6570,E,1,05,2.2,18.3,M,39.0,M,,*7F\n";

	decode(paragraph.str());
	
	
    // Set 'true' to print verification for ALL data in nmea file, assuming associated *.check file exists (see SanityCheck.h)
    if(false) {
        string paragraph, check_line;
        ifstream check_file(nmea_filename + ".check");
        
        close_nmea_file(); // reset file stream
        while(poll(&paragraph)) {
            if(send_message(decode(paragraph))) {
                if(check_file.is_open() && !getline(check_file, check_line))
                    check_file.close();
                else
                    cout << INDENT_SPACES << INDENT_SPACES << "Sanity check: " + check_line << endl << endl;
            }
        }
    }

}




