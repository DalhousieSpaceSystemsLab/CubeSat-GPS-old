cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 11)

project (LORIS-GPS)
SET ( message include/internal-messaging)

include_directories ( include ${message}/message/header ${message}/message/identifiers ${message}/unixdomain/header include/minmea)



SET ( unixdomain

    ${message}/unixdomain/cc/UnixDomainStreamSocket.cc 
    ${message}/unixdomain/cc/UnixDomainStreamSocketClient.cc 
    ${message}/unixdomain/cc/UnixDomainStreamSocketServer.cc )
SET ( message 

   ${message}/message/cc/Key.cc 
   ${message}/message/cc/KeyIntPair.cc 
   ${message}/message/cc/KeyFloatPair.cc
   ${message}/message/cc/KeyValuePairContainer.cc 
   ${message}/message/cc/KeyValuePairContainerInterface.cc 
   ${message}/message/cc/Message.cc 
   ${message}/message/cc/MessageBuilder.cc 
   ${message}/message/cc/MessageBuilderInterface.cc  )
set ( sanitycheck

    SanityCheck.cpp )

#add_executable( Loris-GPS GPS.cpp include/minmea.c ${unixdomain} ${message} )
add_executable( Loris-GPS GPS.cpp GPSMessageInterface.cpp include/minmea.c ${message} ${sanitycheck} )
