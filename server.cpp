#include "socket/SocketServer.hpp"
#include "socket/hostInfo.hpp"
#include "sensor/sensorTypes/sensorTypes.hpp"
#include "sensor/sensor.hpp"
#include <algorithm>

#define PORTNUM 1234

using namespace std;

int main(){

	bool configurated = false;
	string servername = "localhost";
    TcpSocket client;    // connection dedicated for client communication
	string clientMessageIn = "";
    string clientHost;      // client name etc. 

    HostInfo uHostAddress(servername,NAME);
	string localHostName = uHostAddress.getHostName();
    string localHostAddr = uHostAddress.getHostIPAddress();
	cout << "------------------------------------------------------" << endl;
	cout << "		Server information:" << endl;
	cout << "		Name:    " << localHostName << endl;
    cout << "		Address: " << localHostAddr << endl;
	cout << "------------------------------------------------------" << endl;

	// open socket on the local host
	TcpSocket Server(PORTNUM);

    Server.bindSocket();
	cout << endl << "server finishes binding process... " << endl;
	
	Server.listenToClient();
	cout << "server is listening to the port ... " << endl;

    // wait to accept a client connection.  
	// processing is suspended until the client connects
	cout << "server is waiting for client connecction ... " << endl;

    client = *Server.acceptClient(clientHost);
	        
    cout << endl << "==> A client from [" << clientHost << "] is connected with ID: " << client.getSocketId() << endl;


	//Receive configuration sensors and time from of client

	unsigned int loopTime = 0;

	while (loopTime == 0){
		//First message to configurate sensors and time
		char configurationSensor[MAX_MSG_LEN];
		// receive from the client
	    strcpy(configurationSensor, client.recieveMessage());
		cout   << "[RECV:" << clientHost << "]: " << configurationSensor << endl;
		client.ProcessConfigurationMessage(configurationSensor);
		loopTime = client.getTime()*1000; // Pasar a milisegundos
	}

	bool continuar = true;

	Sensor * s; // Vector de sensores

	vector <string> sensorList; // lista de nombres de sensores

	sensorList = client.getSensorList();
	
    for (int i= 0; i<sensorList.size() && continuar; i++) {
        
        s= 0;

		s= Sensor::crearObjetoSensor(sensorList[i]); //Tenemos que meter el tipo del sensor que es.

	    if (s == 0)
	        continuar= false;
	    else {
	        s->setName(sensorList[i]);
	        s->init(loopTime);
	    }
	}

	configurated = true;

	while(configurated == true){
		int numBytes = 0;
		char message[MAX_MSG_LEN];
		// receive from the client
        strcpy(message, client.recieveMessage());
		cout   << "[RECV:" << clientHost << "]: " << message << endl;
		client.ProcessMessage(message);

/*
		// send to the client
		char sendmsg[MAX_MSG_LEN+1];
		memset(sendmsg,0,sizeof(sendmsg));
		cout << "[" << localHostName << ":SEND] ";
		cin.getline(sendmsg,MAX_MSG_LEN);

		if ( numBytes == -99 ) 
			break;

		string sendMsg(sendmsg);
		if ( sendMsg.compare("Bye") == 0 || sendMsg.compare("bye") == 0 )
			break;

		cout   << "[" << localHostName << ": SEND] " << sendMsg << endl;
	
		client->sendMessage(sendMsg);
*/

    }

    return 0;
}