/*!
  \file   SocketServer.hpp
  \brief   
  \author Francisco Jesus Arcos Romero
  \date   23/06/2018
*/
 
#ifndef __SOCKETSERVER_HPP__
#define __SOCKETSERVER_HPP__

#include <fcntl.h>
#include <sys/filio.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <list>
#include <vector>

#include "hostInfo.hpp"

#define OPT 1
#define MOV 2
#define EXIT 3

const int MAX_RECV_LEN = 512;
const int MAX_MSG_LEN = 256;
const int MSG_HEADER_LEN = 6;

class SocketServer{

	public:

		int portNumber;        				// Socket port number    
	    int socketId;          				// Socket file descriptor
		int blocking;          				// Blocking flag
	    int bindFlag;          				// Binding flag
	    struct sockaddr_in clientAddr;    	// Address of the client that sent data
		bool connected;
		char buffer [MAX_MSG_LEN];
		SocketServer() {}
		
		void setSocketId(int socketFd) { socketId = socketFd; }

		SocketServer(int port);

	    virtual ~SocketServer(){close(socketId);}

		// returns the socket file descriptor
	    int getSocketId() { return socketId; }

		// returns the port number
		int getPortNumber() { return portNumber; }

		// show the socket 
		friend std::ostream& operator << (std::ostream &, SocketServer &);

		void error(const char *msg);

};

class TcpSocket : public SocketServer {

	public:

	    vector <string> sensorList;
	    unsigned short int timeSensors;
	    unsigned short int command;
		char * movement;

		TcpSocket() {
			    timeSensors = -1;
			    command = -1;
			    movement = (char*)malloc(20);
		};
		
		~TcpSocket() {};

		TcpSocket(int portId) : SocketServer(portId) { };

		vector <string> getSensorList(){return sensorList;};

		unsigned short int getTime(){return timeSensors;};

		int sendMessage(string&);

		char * recieveMessage();

		void ProcessMessage(char * message);

		void ProcessConfigurationMessage(char * message);

		void bindSocket();

		virtual void connectToServer(string&,hostType);

		TcpSocket* acceptClient(string&);

		void listenToClient(int numPorts = 5);

		void error(const char *msg);

		void closeSocket ();

};

#endif