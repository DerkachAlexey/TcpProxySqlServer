cmake_minimum_required (VERSION 3.12)

project( proxy_sql_server)

set( SRC main.cpp
	 TcpProxyServer.hpp
	 TcpProxyServer.cpp
	 Connection.hpp
	 Connection.cpp
	 Logger.hpp
	 Logger.cpp
	 PacketsParser.hpp
	 PacketsParser.cpp
	 Packet.hpp
	 Packet.cpp)

add_executable( ${CMAKE_PROJECT_NAME} ${SRC})

SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "C:/Program Files/boost_1_69_0")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "C:/Program Files/boost_1_69_0/libs")


find_package( Boost )


if( Boost_FOUND )
   
  include_directories( ${Boost_INCLUDE_DIRS} )
   
  target_link_libraries( proxy_sql_server ${Boost_LIBRARIES} )

endif()
