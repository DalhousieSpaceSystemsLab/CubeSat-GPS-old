#include "GPSMessageInterface.hpp"
Identifiers identifiers;
MessageReceivingService interface(identifiers.payload_repository);

GPS_Data_Types data_types;
chrono::system_clock::time_point last_poll = chrono::system_clock::now();
chrono::system_clock::time_point last_empty_poll = chrono::system_clock::now();

// code for sending out and recieving messages from the OBC
// note: this class can absolutely be mocked out for unit tests
bool send_message(gps_data* decoded_data) {
	//KeyValuePairContainer container;
	CommandMessage message;

	message.SetSender(identifiers.payload_subsystem);
	message.SetRecipient(identifiers.payload_repository);
	message.SetTimeCreated(CURRENT_TIME); //check that CURRENT_TIME actually works in GPS.hpp
	//?????? maybe?
	message.Add(data_types.latitude, decoded_data -> latitude);
	message.Add(data_types.longitude, decoded_data -> longitude);

	if (!isnan(decoded_data -> time))
		message.Add(data_types.time, (int) decoded_data -> time);
	message.Add(data_types.height, decoded_data -> height);
	message.Add(data_types.altitude, decoded_data -> altitude);

	interface.Reply(message);
	
	//MessageSenderInterface ms(message.GetRecipient());
	//ms.SendMessage(message);
	
	cout << endl << "== BEGIN CONTENTS ==" << endl;
	cout << endl << "RECIPIENT: " << message.GetRecipient() << endl
		<< "SENDER: " << message.GetSender() << endl
		<< "TIME CREATED: " << message.GetTimeCreated() << endl
		<< "CONTENTS:" << endl
		<< INDENT_SPACES << "TIME STAMP: " << message.GetMessageContents().GetInt(data_types.time) << endl
		<< INDENT_SPACES << "LATITUDE: " << message.GetMessageContents().GetFloat(data_types.latitude) << endl
		<< INDENT_SPACES << "LONGITUDE: " << message.GetMessageContents().GetFloat(data_types.longitude) << endl
		<< INDENT_SPACES << "HEIGHT: " << message.GetMessageContents().GetFloat(data_types.height) << endl
		<< INDENT_SPACES << "ALTITUDE: " << message.GetMessageContents().GetFloat(data_types.altitude) << endl;
	cout << endl << "== END CONTENTS ==" << endl << endl; 
	return true;
}


CommandMessage get_message(){
	CommandMessage message;
	Identifiers indentifiers;
	status_codes codes;
	//every 20 seconds, send a REQUEST message 
	if (20 - chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - last_poll).count() == 0) {
		cout << "TEST: Sending REQUEST message..." << endl;
		message.Add(0, codes.request);
		last_poll = chrono::system_clock::now();
	}
	//every 30 seconds, send nothing
	else if (20 - chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - last_empty_poll).count() == 0) {
		cout << "TEST: Sending EMPTY message..." << endl;
		last_empty_poll = chrono::system_clock::now();
	}
	else {
		cout << "TEST: Sending STANDBY message..." << endl;
    		message.Add(0, codes.standby);
	}

	message.SetRecipient(identifiers.payload_subsystem);
	message.SetSender(identifiers.payload_repository);
	return message;
}
