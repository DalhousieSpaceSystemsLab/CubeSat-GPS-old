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
string poll() {
	if (gpsOn) {
		return get_message();
	}
	return NULL;
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
			}
		}
	}
	printf("time: %s\naltitude: %f\n(%f, %f)\nheight: %f\n\n",
	data.time_stamp.c_str(), data.altitude, data.latitude, data.longitude, data.height);
	
	return data;
}

string toStringTime(struct minmea_time *time) {
	stringstream timeString;
	timeString << time->hours << ":" << time->minutes << ":" << time->seconds << ":" << time->microseconds;
	return timeString.str();
}

string get_message() {
	string message; // set message
	return message;
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

vector<string> read_nmea_from_file() {
	int n = NULL;
    vector<string> nmea_data;
    string line;
	//will need to be set per build environment. This is set for "build" directory
	ifstream nmea_file("../source/resources/nmea_data/nmea01.txt"); //unix
    //ifstream nmea_file("../../../../source/resources/nmea_data/nmea01.txt"); //visual studio
    
    if(nmea_file.is_open()) {
        while(getline(nmea_file, line)) {
            nmea_data.push_back(line);
        }
        nmea_file.close();
    } else {
        cout << "Unable to open file";
    }
    
    return nmea_data;
}

//scheduler class to send messages
int main() {
	vector<string> nmea_data;
    
	nmea_data = read_nmea_from_file();
	cout << "\n" << "Read data from file: " << nmea_data[0] << "\n";
//	send_message(decode(nmea_data[0]));
	decode(nmea_data[0]);
	
	send_message(decode("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47"));

	decode("$GPGGA,012219,1237.038,N,01531.000,E,1,08,0.9,125.4,M,46.9,M,,*47\n$GPZDA,201530.00,04,07,2002,00,00*60");
	stringstream paragraph;
	paragraph << "$GPGGA,023042,4007.3837,N,12102.89684,W,1,04,2.3,507.3,M,-24.1,M,,*75\n";
	paragraph << "$GPGGA,023042,3907.3837,N,12102.4684,W,1,04,2.3,507.3,M,-24.1,M,,*75\n";
	paragraph << "$GPGGA,022454,3553.5295,N,13938.6570,E,1,05,2.2,18.3,M,39.0,M,,*7F\n";

	decode(paragraph.str());

}




