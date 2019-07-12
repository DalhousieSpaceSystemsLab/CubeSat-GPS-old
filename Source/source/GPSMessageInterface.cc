#include "GPSMessageInterface.hpp"
GPS_Data_Types data_types;

bool send_message(gps_data* decoded_data) {
	MessageBuilder messageBuilder;
	messageBuilder.StartMessage();
	

	KeyValuePairContainer container;
	container.AddKeyValuePair(data_types.latitude, decoded_data -> latitude);
	container.AddKeyValuePair(data_types.longitude, decoded_data -> longitude);
	if (!isnan(decoded_data -> time))
		container.AddKeyValuePair(data_types.time_stamp, decoded_data -> time);
	container.AddKeyValuePair(data_types.height, decoded_data -> height);
	container.AddKeyValuePair(data_types.altitude, decoded_data -> altitude);

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

bool get_message(KeyValuePairContainer* message){
    //replace this with identifiers, this will be fetched from OS once their code is ready
    message -> AddKeyValuePair(0, 1);
	return false;
}