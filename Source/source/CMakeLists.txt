cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)

project (LORIS-GPS)
include_directories ( include include/message/header include/message/identifiers include/unixdomain/header)

SET ( unixdomain

    ./include/unixdomain/cc/UnixDomainStreamSocket.cc 
    ./include/unixdomain/cc/UnixDomainStreamSocketClient.cc 
    ./include/unixdomain/cc/UnixDomainStreamSocketServer.cc )
SET ( message 

   ./include/message/cc/Key.cc 
   ./include/message/cc/KeyIntPair.cc 
   ./include/message/cc/KeyFloatPair.cc
   ./include/message/cc/KeyValuePairContainer.cc 
   ./include/message/cc/KeyValuePairContainerInterface.cc 
   ./include/message/cc/Message.cc 
   ./include/message/cc/MessageBuilder.cc 
   ./include/message/cc/MessageBuilderInterface.cc 
   ./include/message/cc/MessagingService.cc )

#add_executable( Loris-GPS GPS.cpp include/minmea.c ${unixdomain} ${message} )
add_executable( Loris-GPS GPS.cpp include/minmea.c ${message} )

