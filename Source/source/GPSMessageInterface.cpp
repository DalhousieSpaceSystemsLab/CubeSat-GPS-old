#include "GPSMessageInterface.hpp"
GPS_Data_Types data_types;
chrono::system_clock::time_point last_poll = chrono::system_clock::now();
chrono::system_clock::time_point last_empty_poll = chrono::system_clock::now();

// code for sending out and recieving messages from the OBC
// note: this class can absolutely be mocked out for unit tests
bool send_message(gps_data* decoded_data) {
	MessageBuilder messageBuilder;
	Identifiers identifiers;
	messageBuilder.StartMessage();
	

	KeyValuePairContainer container;
	container.AddKeyValuePair(data_types.latitude, decoded_data -> latitude);
	container.AddKeyValuePair(data_types.longitude, decoded_data -> longitude);
	if (!isnan(decoded_data -> time))
		container.AddKeyValuePair(data_types.time, (int) decoded_data -> time);
	container.AddKeyValuePair(data_types.height, decoded_data -> height);
	container.AddKeyValuePair(data_types.altitude, decoded_data -> altitude);

	messageBuilder.SetMessageContents(container);
	messageBuilder.SetRecipient(identifiers.gps_repository);
	messageBuilder.SetSender(identifiers.gps_subsystem);
	

	Message message = messageBuilder.CompleteMessage();

	char msg[256] = "";
	message.flatten(msg);
	
	//MessageSenderInterface ms(message.GetRecipient());
	//ms.SendMessage(message);

	
	message = Message(msg);

	//vector<int> keys = message.GetMessageContents().GetKeys();

	KeyValuePairContainer c = message.GetMessageContents();
	
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


Message get_message(){
	MessageBuilder builder;
	builder.StartMessage();
	KeyValuePairContainer container;

	status_codes codes;
	Identifiers identifiers;
	//every 20 seconds, send a REQUEST message 
	if (20 - chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - last_poll).count() == 0) {
		cout << "TEST: Sending REQUEST message..." << endl;
		container.AddKeyValuePair(0, codes.request);
		last_poll = chrono::system_clock::now();
	}
	//every 30 seconds, send nothing
	else if (20 - chrono::duration_cast<chrono::seconds>(chrono::system_clock::now() - last_empty_poll).count() == 0) {
		cout << "TEST: Sending EMPTY message..." << endl;
		last_empty_poll = chrono::system_clock::now();
	}
	else {
		cout << "TEST: Sending STANDBY message..." << endl;
    		container.AddKeyValuePair(0, codes.standby);
	}

	builder.SetMessageContents(container);
	builder.SetRecipient(identifiers.gps_subsystem);
	builder.SetSender(identifiers.gps_repository);
	return builder.CompleteMessage();
}
