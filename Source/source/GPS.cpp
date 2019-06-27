#include "GPS.hpp"

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
	minmea_sentence_gga frame;
	if (minmea_parse_gga(&frame, raw.c_str())) {
		cout << "true" << endl;
		printf(INDENT_SPACES "$xxGGA: raw coordinates : (%d/%d,%d/%d)\n",
                            frame.latitude.value, frame.latitude.scale,
                            frame.longitude.value, frame.longitude.scale);
        printf(INDENT_SPACES "$xxGGA fixed-point coordinates scaled to three decimal places: (%d,%d)\n",
                            minmea_rescale(&frame.latitude, 1000),
                            minmea_rescale(&frame.longitude, 1000));
        printf(INDENT_SPACES "$xxGGA floating point degree coordinates: (%f,%f)\n\n",
                            minmea_tocoord(&frame.latitude),
                            minmea_tocoord(&frame.longitude));
	}
	else {
		cout << "false" << endl;
	}
	gps_data data;
	return data;
}

string get_message() {
	string message; // set message
	return message;
}

bool send_message(gps_data decoded_data) {
	MessageBuilder messageBuilder;
	messageBuilder.StartMessage();

	KeyValuePairContainer container;
	container.AddKeyValuePair(data_types.altitude, decoded_data.altitude);
	container.AddKeyValuePair(data_types.horizontal_dilution, decoded_data.horizontal_dilution);
	container.AddKeyValuePair(data_types.latitude, decoded_data.latitude);
	container.AddKeyValuePair(data_types.longitude, decoded_data.longitude);
	container.AddKeyValuePair(data_types.num_sats, decoded_data.num_sats);
	container.AddKeyValuePair(data_types.time_stamp, decoded_data.time_stamp);
	container.AddKeyValuePair(data_types.type, decoded_data.type);

	messageBuilder.SetMessageContents(container);
	messageBuilder.SetRecipient(1097346);
	messageBuilder.SetSender(6858902);

	Message message = messageBuilder.CompleteMessage();

	char msg[256] = "";
	message.flatten(msg);
	//SerializeMessage(&message, msg);
	MessageSenderInterface ms(message.GetRecipient());
	ms.SendMessage(msg);

	message = Message(msg);


	vector<int> keys = message.GetMessageContents().GetKeys();

	KeyValuePairContainer c = message.GetMessageContents();
	cout << "\nRECIPIENT: " << message.GetRecipient() << endl
		 << "SENDER: " << message.GetSender() << endl
		 << "TIME CREATED: " << message.GetTimeCreated() << endl
		 << "CONTENTS:" << endl
		 << INDENT_SPACES << "TYPE: " << message.GetMessageContents().GetInt(data_types.type) << endl
		 << INDENT_SPACES << "TIME STAMP: " << message.GetMessageContents().GetInt(data_types.time_stamp) << endl
		 << INDENT_SPACES << "LATITUDE: " << message.GetMessageContents().GetFloat(data_types.latitude) << endl
		 << INDENT_SPACES << "LONGITUDE: " << message.GetMessageContents().GetFloat(data_types.longitude) << endl
		 << INDENT_SPACES << "NUM_SATS: " << message.GetMessageContents().GetInt(data_types.num_sats) << endl
		 << INDENT_SPACES << "HORIZONTAL DILUTION: " << message.GetMessageContents().GetFloat(data_types.horizontal_dilution) << endl
		 << INDENT_SPACES << "ALTITUDE: " << message.GetMessageContents().GetFloat(data_types.altitude) << endl;
	return true;
}

vector<string> read_nmea_from_file() {
    vector<string> nmea_data;
    string line;
	//will need to be set per build environment. This is set for "build" directory
    ifstream nmea_file("../source/resources/nmea_data/nmea01.txt");
    
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
    
	decode("abc"); //should be false
	decode("$GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,,*47"); //should be true
	
	nmea_data = read_nmea_from_file();
	cout << "\n" << "Read data from file: " << nmea_data[0] << "\n";
	send_message(decode(nmea_data[0]));
}




