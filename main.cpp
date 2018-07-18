#include "socket/SocketServer.hpp"
#include "socket/hostInfo.hpp"
#include "sensor/sensorTypes/sensorTypes.hpp"
#include "sensor/sensor.hpp"
#include <algorithm>
#include <time.h>
#include <unistd.h>
#include <cstring>
#include <string>

#define PORTNUM 1234

using namespace std;

int main(){

	char configurationSensor[MAX_MSG_LEN];
	string servername = "localhost";
    TcpSocket client;    // connection dedicated for client communication
	string clientMessageIn = "";
    string clientHost;      // client name etc. 
    string explorationName = "";
    HostInfo uHostAddress(servername,NAME);
	string localHostName = uHostAddress.getHostName();
    string localHostAddr = uHostAddress.getHostIPAddress();
	cout << "------------------------------------------------------" << endl;
	cout << "		Server information:" << endl;
	cout << "		Name:    " << localHostName << endl;
    cout << "		Address: " << localHostAddr << endl;
	cout << "------------------------------------------------------" << endl;

	while (true){

		cout << "INITIALIZE CONNECTION" << endl;
		// open socket on the local host
		TcpSocket Server(PORTNUM);

	    Server.bindSocket();
		cout << endl << "Server finishes binding process... " << endl;
		
		Server.listenToClient();
		cout << "Server is listening to the port ... " << endl;

	    // wait to accept a client connection.  
		// processing is suspended until the client connects
		cout << "Server is waiting for client connecction ... " << endl;

	    client = *Server.acceptClient(clientHost);
		        
	    cout << endl << "==> A client from [" << clientHost << "] is connected with ID: " << client.getSocketId() << endl;

		//Receive configuration sensors and time from of client
		// receive from the client
	    strcpy(configurationSensor, client.recieveMessage());
		cout   << "[RECV:" << clientHost << "]: " << configurationSensor << endl;
		client.ProcessConfigurationMessage(configurationSensor);

		usleep(1000);
		
		client.createExploration();
		usleep(1000);

		//CREAR SENSORES
		client.createSensors();

		usleep(1000);

		while(client.isAlive()){
			char message[MAX_MSG_LEN];
			// receive from the client
	        strcpy(message, client.recieveMessage());
			cout   << "[RECV:" << clientHost << "]: " << message << endl;
			client.ProcessMessage(message);
		}

		system("clear");
	}
    return 0;
}