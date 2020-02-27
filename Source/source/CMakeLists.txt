cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/../Build)
set (CMAKE_BINARY_DIR ${CMAKE_HOME_DIRECTORY}/../Build)

project (LORIS-GPS)
SET ( OS include/internal-messaging)

include_directories ( 
   include 

   ${OS}/messaging-service/header
   ${OS}/message/header 
   ${OS}/message/identifiers 
   ${OS}/unixdomain/header
   include/minmea
)

SET ( unixdomain

    ${OS}/unixdomain/cc/UnixDomainStreamSocket.cc 
    ${OS}/unixdomain/cc/UnixDomainStreamSocketClient.cc 
    ${OS}/unixdomain/cc/UnixDomainStreamSocketServer.cc 
)

SET ( message 
   ${OS}/message/cc/MessageBuilder.cc
   ${OS}/message/cc/Message.cc
   ${OS}/message/cc/DataMessage.cc
   ${OS}/message/cc/CommandMessage.cc
   ${OS}/message/cc/KeyValuePairContainer.cc
   ${OS}/message/cc/Key.cc 
   ${OS}/message/cc/KeyIntPair.cc 
   ${OS}/message/cc/KeyFloatPair.cc
   ${OS}/message/cc/KeyStringPair.cc
   ${OS}/message/cc/KeyValuePairContainerInterface.cc 
   ${OS}/message/cc/Message.cc 
   ${OS}/message/cc/MessageBuilder.cc 
   ${OS}/message/cc/MessageBuilderInterface.cc
)

SET ( messaging-service 
   ${OS}/messaging-service/cc/MessageRecievingService.cc
   ${OS}/messaging-service/cc/PhoneBook.cc 
   ${OS}/messaging-service/cc/MessageRecipientInterface.cc
)

set ( sanitycheck

    SanityCheck.cpp )

#add_executable( Loris-GPS GPS.cpp include/minmea.c ${unixdomain} ${message} ${messaging-service})
add_executable( Loris-GPS 

   GPS.cpp 
   GPSMessageInterface.cpp 
   GPSDecode.cpp
   include/minmea.c 
   ${message} 
   ${unixdomain} 
   ${messaging-service}
   ${sanitycheck} 
)
