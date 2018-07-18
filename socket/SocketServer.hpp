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
#include "../sensor/sensor.hpp"
#include "/usr/local/Cellar/mariadb/10.3.8/include/mysql/mysql.h"

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
		int error_code;								//Check status
		socklen_t error_code_size = sizeof(error_code);	//Check status
		int check;

		MYSQL *conn;
		MYSQL_RES *result;
		MYSQL_ROW row;
		const char* host = "localhost";
		const char* database = "db_rover";
		const char* db_user = "raspirover";
		const char* db_pass = "raspirover";

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
		char * explorationName;
		vector <Sensor *> sensors;

		TcpSocket() {
			    timeSensors = -1;
			    command = -1;
			    movement = (char*)malloc(20);
		};
		
		~TcpSocket() {};

		TcpSocket(int portId) : SocketServer(portId) { };

		vector <string> getSensorList(){return sensorList;};

		unsigned short int getTime(){return timeSensors;};

		char * getExplorationName() { return explorationName; }

		int sendMessage(string&);

		char * recieveMessage();

		void ProcessMessage(char * message);

		void ProcessConfigurationMessage(char * message);

		void bindSocket();

		virtual void connectToServer(string&,hostType);

		TcpSocket* acceptClient(string&);

		void listenToClient(int numPorts = 1);

		void error(const char *msg);

		void closeSocket ();

		bool isAlive();

		vector <Sensor *> getSensors() const {return sensors;}

		void setSensors(vector <Sensor *> s) {this->sensors = s;}

		void createSensors();

		bool createExploration();
    
};

#endif