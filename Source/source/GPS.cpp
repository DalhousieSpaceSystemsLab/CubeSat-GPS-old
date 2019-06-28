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
bool poll(string *message) {
    if (gpsOn) {
        cout << "GPS on" << endl;
    }
    return read_nmea_from_file(message);
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

// dummy method that reads static data directly from a file, emulating string data to be returned by GPS hardware
bool read_nmea_from_file(string *data) {
    string nmea_paragraph("");
    string line;
    
    open_nmea_file(nmea_filename);
    
    if(nmea_datafile->is_open()) {
        //cout << "\tData file opened, reading three lines" << endl;
        for(int i=0; i<3; i++) {
            if(getline(*nmea_datafile, line)) {
                nmea_paragraph += line.substr(0, line.length()-1) + "\n";  // strip \lf and (re-)add \n
            } else {
                cout << INDENT_SPACES << "No more lines, closing data file" << endl;
                nmea_datafile->close();
                break;
            }
        }
    } else {
        cout << "Unable to open file: '" << nmea_filename << "'" << endl;
    }
    *data = nmea_paragraph;

    return (nmea_paragraph.length() > 0);
}

// dummy helper method for reading static data
void close_nmea_file() {
    if(nmea_datafile) {
        nmea_datafile->close();
        nmea_datafile = NULL;
    }
}

// dummy helper method for reading static data
void open_nmea_file(string fn) {
    if(!nmea_datafile) {
        //will need to be set per build environment. This is set for "build" directory
        nmea_datafile = new ifstream("../source/resources/nmea_data/" + fn);
        cout << INDENT_SPACES << "Reading from data file " + fn << endl;
    }
}


int main() {
    string nmea_data;
    
    // Poll GPS once
    cout << endl << "Polling GPS... " << endl;
    init_gps();
    if(poll(&nmea_data)) {
        cout << "\nRaw data from file: " << endl << nmea_data << endl;
        
        send_message(decode(nmea_data)); // assumes that decode() parses paragraph to check sentence-type of each line (or that first sentence is always GGA)
        
        cout << "\nSanity check for first paragraph of nmea01.txt is:  45.3907333.. / -60.91918333.. degrees" << endl;
    }
    
    
    // Set 'true' to print verification for ALL data in file, assuming *.check file exists
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




