/**********************************************************************************************************************************************************************************************************************************/
/************************************************************************************************************* LICENSE ************************************************************************************************************/
/**********************************************************************************************************************************************************************************************************************************/
//
//    Copyright © 2016 Bastien Pasdeloup (name.surname@gmail.com) and Télécom Bretagne
//
//    This file is part of PyRat.
//
//    PyRat is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    PyRat is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with PyRat.  If not, see <http://www.gnu.org/licenses/>.
//
/**********************************************************************************************************************************************************************************************************************************/
/****************************************************************************************************** INCLUDES & NAMESPACES *****************************************************************************************************/
/**********************************************************************************************************************************************************************************************************************************/

    /** EXTERNAL **/
    #include <arpa/inet.h>
    #include <ifaddrs.h>
    #include <string>
    #include <time.h>
    #include <unistd.h>
    
    /** INTERNAL **/
    #include <NetworkObject.cpp>
    #include <Shell.cpp>
    
    /** NAMESPACES **/
    using namespace std;

/**********************************************************************************************************************************************************************************************************************************/
/************************************************************************************************************** CLASS *************************************************************************************************************/
/**********************************************************************************************************************************************************************************************************************************/
    
    #ifndef _NETWORK_CLIENT_
    #define _NETWORK_CLIENT_
        
        class NetworkClient : public NetworkObject
        {
            
            /**********************************************************************************************************************************************************************************************************/
            /************************************************************************************************ CONSTANTS ***********************************************************************************************/
            /**********************************************************************************************************************************************************************************************************/
                
                /** DOCUMENTATION **/
                // Time to sleep before retrying to connect to a non-yet-started server
                
                /** CODE **/
                private : const int MICROSECONDS_BEFORE_RETRY = 5000;
            
            /**********************************************************************************************************************************************************************************************************/
            /********************************************************************************************** CONSTRUCTORS **********************************************************************************************/
            /**********************************************************************************************************************************************************************************************************/
            
                /** DOCUMENTATION **/
                // Constructor with the IP address and port
                
                /** CODE **/
                protected : NetworkClient (string address, int port) : NetworkObject(address, port)
                {}
            
            /**********************************************************************************************************************************************************************************************************/
            /*********************************************************************************************** DESTRUCTOR ***********************************************************************************************/
            /**********************************************************************************************************************************************************************************************************/
                
                /** DOCUMENTATION **/
                // Empty destructor
                
                /** CODE **/
                protected : virtual ~NetworkClient ()
                {}
                
            /**********************************************************************************************************************************************************************************************************/
            /************************************************************************************************* METHODS ************************************************************************************************/
            /**********************************************************************************************************************************************************************************************************/
                
                /** DOCUMENTATION **/
                // Connects to a TCP socket server at the given address and port
                
                /** CODE **/
                public : void establishConnection ()
                {
                    
                    // We create a socket
                    NetworkObject::_socket = socket(AF_INET, SOCK_STREAM, 0);
                    if (NetworkObject::_socket < 0) 
                        Shell::getInstance().error("Could not open socket");
                    
                    // We configure the socket to the given address/port
                    struct sockaddr_in socketDescription;
                    socketDescription.sin_family = AF_INET;
                    socketDescription.sin_addr.s_addr = inet_addr(NetworkObject::_address.c_str());
                    socketDescription.sin_port = htons(NetworkObject::_port);
                    
                    // We try connecting until the server becomes available
                    time_t start, currentTime;
                    time(&start);
                    time(&currentTime);
                    while (difftime(currentTime, start) <= NetworkObject::START_CONNECTION_TIMEOUT)
                    {
                        int result = connect(_socket, (struct sockaddr*)(&socketDescription), sizeof(socketDescription));
                        if (result >= 0)
                        {
                            NetworkObject::_connected = true;
                            break;
                        }
                        usleep(MICROSECONDS_BEFORE_RETRY);
                        time(&currentTime);
                    }
                    
                }
                
            /**********************************************************************************************************************************************************************************************************/
            /**********************************************************************************************************************************************************************************************************/
            
        };
        
    #endif

/**********************************************************************************************************************************************************************************************************************************/
/**********************************************************************************************************************************************************************************************************************************/