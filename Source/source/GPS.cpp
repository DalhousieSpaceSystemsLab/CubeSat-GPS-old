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
        //cout << "GPS on" << endl;
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
			}
		}
	}
	//printf("time: %s\naltitude: %f\n(%f, %f)\nheight: %f\n\n",
	//data.time_stamp.c_str(), data.altitude, data.latitude, data.longitude, data.height);
	
	return data;
}

string toStringTime(struct minmea_time *time) {
	stringstream timeString;
	timeString << time->hours << ":" << time->minutes << ":" << time->seconds << ":" << time->microseconds;
	return timeString.str();
}


void build_message(gps_data decoded_data, Message *message) {
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
	

	*message = messageBuilder.CompleteMessage();
}


bool send_message(gps_data decoded_data) {
	Message message;
	build_message(decoded_data, &message);
	
	//SerializeMessage(&message, msg);
	//MessageSenderInterface ms(message.GetRecipient());
	//ms.SendMessage(msg);

	char msg[256] = "";
	message.flatten(msg);
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
	
	


	// Beginning of general sanity checking: assumes associated *.check file exists (see SanityCheck.h)

    bool success;
    string SEPARATOR = "== - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - = - ==";
    
    cout << endl << SEPARATOR << endl;
    
    success = test_decode(false);
    cout << INDENT_SPACES << "Decode tests:  ";
    if(success)
        cout << "PASSED";
    else
        cout << "FAILED";
    cout << endl << SEPARATOR << endl;
    
    success = test_build_message(false);
    cout << INDENT_SPACES << "Build message tests:  ";
    if(success)
        cout << "PASSED";
    else
        cout << "FAILED";
    cout << endl;
    
    cout << SEPARATOR << endl << endl;

}



// test that decode() works as expected
bool test_decode(bool verbose) {
    string paragraph, check_line;
    gps_data decoded_data;
    float lat, lon;
    bool success = true;
    bool line_success = false;
    
    reset_datafile();
    reset_checkfile();
    
    while(poll(&paragraph)) {
        decoded_data = decode(paragraph);
        if(read_sanitycheck_from_file(&check_line)) {
            lat = decoded_data.latitude;
            lon = decoded_data.longitude;
            
            if(verbose) {
                cout << INDENT_SPACES << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
                cout << INDENT_SPACES << INDENT_SPACES << "Decoded      (LAT,LONG): " << lat << ", " << lon << endl;
                cout << INDENT_SPACES << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
                cout << INDENT_SPACES << INDENT_SPACES << "Sanity check (LAT,LONG): " + check_line << endl;
                cout << INDENT_SPACES << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl << endl;
            }
            
            line_success = compare_floats(lat, parse_token_float(check_line, 0)) && compare_floats(lon, parse_token_float(check_line, 1));
            if(verbose) {
                if(line_success)
                    cout << INDENT_SPACES << " ^ Passed ^ " << endl << endl;
                else
                    cout << INDENT_SPACES << " ^ FAILED ^ " << endl << endl;
            }
        }
        
        success = success && line_success;
    }
    return success;
}


// test that build_message() works as expected
bool test_build_message(bool verbose) {
    string paragraph, check_line;
    gps_data decoded_data;
    Message message;
    float lat, lon;
    bool success = true;
    bool line_success = false;
    
    reset_datafile();
    reset_checkfile();
    
    while(poll(&paragraph)) {
        decoded_data = decode(paragraph);
        build_message(decoded_data, &message);
        
        //char msg[256] = "";
	    //message.flatten(msg);
	    //message = Message(msg);
        
        if(read_sanitycheck_from_file(&check_line)) {
            lat = message.GetMessageContents().GetFloat(data_types.latitude);
            lon = message.GetMessageContents().GetFloat(data_types.longitude);
            
            if(verbose) {
                cout << INDENT_SPACES << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
                cout << INDENT_SPACES << INDENT_SPACES << "Built Message (LAT,LONG): " << lat << ", " << lon << endl;
                cout << INDENT_SPACES << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
                cout << INDENT_SPACES << INDENT_SPACES << "Sanity check  (LAT,LONG): " + check_line << endl;
                cout << INDENT_SPACES << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << endl;
            }
            
            line_success = compare_floats(lat, parse_token_float(check_line, 0)) && compare_floats(lon, parse_token_float(check_line, 1));
            if(verbose) {
                if(line_success)
                    cout << INDENT_SPACES << "^ Passed ^ " << endl << endl;
                else
                    cout << INDENT_SPACES << "^ FAILED ^ " << endl << endl;
            }
        }
            
        success = success && line_success;
    }
    
    return success;
}

