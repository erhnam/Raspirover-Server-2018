#include "SocketServer.hpp"

void SocketServer::error(const char *msg){
	perror(msg);
	exit(1);
}

void TcpSocket::error(const char *msg){
	perror(msg);
	exit(1);
}

SocketServer::SocketServer(int port){

	portNumber = port;
	blocking = 1;

	socketId = socket(AF_INET, SOCK_STREAM, 0);

	if (socketId == -1){
		error("ERROR opening socket");
	}

	clientAddr.sin_family = AF_INET;
	clientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	clientAddr.sin_port = htons(portNumber);

}

bool TcpSocket::isAlive(){
	check = getsockopt(getSocketId(), SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
	if (check != 0){
		cout << "Socket Desconectado" << endl;
		return false;
	}
	else{
		return true;
	}

}

void TcpSocket::bindSocket(){
	
	if ( ::bind(socketId, (struct sockaddr *)&clientAddr, sizeof(struct sockaddr_in) ) == -1){
		error("ERROR on binding");
		exit(-1);
	}

}

void TcpSocket::connectToServer(string& serverNameOrAddr, hostType hType){ 
	HostInfo serverInfo(serverNameOrAddr, hType);
	
	// Store the IP address and socket port number	
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(serverInfo.getHostIPAddress());
	serverAddress.sin_port = htons(portNumber);

	// Connect to the given address
	if (connect(socketId,(struct sockaddr *)&serverAddress,sizeof(serverAddress)) == -1){
		error("ERROR connecting to server");
		exit(-1);
	}
}

TcpSocket* TcpSocket::acceptClient(string& clientHost){
	int newSocket;   // the new socket file descriptor returned by the accept systme call

	// the length of the client's address
	socklen_t clientAddressLen = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddress;    // Address of the client that sent data

	// Accepts a new client connection and stores its socket file descriptor
	if ((newSocket = accept(socketId, (struct sockaddr *) &clientAddress, &clientAddressLen)) == -1){

		if (newSocket < 0)
			error("Error to accept socket\n");
		else{
			connected = true;
			printf("Connected to socket, waiting message...\n");
		}
   }
	
	// Get the host name given the address
	char *aux = inet_ntoa((struct in_addr)clientAddress.sin_addr);
	string sAddress(aux);
	HostInfo clientInfo(sAddress, ADDRESS);
	char* hostName = clientInfo.getHostName();
	clientHost += string(hostName);
	
	// Create and return the new TcpSocket object
	TcpSocket* retSocket = new TcpSocket();
	retSocket->setSocketId(newSocket);
	return retSocket;
}

void TcpSocket::listenToClient(int totalNumPorts){
	if (listen(socketId,totalNumPorts) == -1){
		error("Error to listen to client\n");
	}
}

int TcpSocket::sendMessage(string& message){
	int numBytes;  // the number of bytes sent

	/* 
	   for each message to be sent, add a header which shows how long this message
	   is. This header, regardless how long the real message is, will always be
	   of the length MSG_HEADER_LEN.
	*/

	char msgLength[MSG_HEADER_LEN+1];
	sprintf(msgLength,"%6lu",message.size());
	string sendMsg = string(msgLength);
	sendMsg += message;

	// Sends the message to the connected host
	if ((numBytes = send(socketId,sendMsg.c_str(),sendMsg.size(),0)) == -1){
		error("Error on sending message...");
	}

	return numBytes;
}

char * TcpSocket::recieveMessage(){
	int numBytes;  // The number of bytes recieved

	bzero(buffer, MAX_MSG_LEN);
	numBytes = read(socketId,buffer,MAX_MSG_LEN);
	if (numBytes < 0){
		error("ERROR reading from socket\n");
	}

	return buffer;
}

void TcpSocket::closeSocket(){
	for (int i= 0; i<getSensors().size(); i++) {
		sensors[i]->closeSensor();                
	}

	close(socketId);
}

void TcpSocket::ProcessConfigurationMessage(char * message){
	char * copyMessage = (char*)malloc(strlen(message) + 1);    
	char * aux;
	strcpy(copyMessage, message);

	//Detect the command
	aux = strtok(copyMessage, ":");
	
	//Operate with the command
	if(strcmp(aux, "OPT") == 0){
		this->command = OPT;
		char * sensors;

		explorationName = strtok(0, "/");
//		cout << "EXPLORATION: " << explorationName << endl;

		sensors = strtok(0, "/");
//        cout <<  "SENSORS: " << sensors << endl;

		timeSensors = atoi(strtok(0, "/"))*1000;
//        cout <<  "TIME: " << timeSensors << endl;

		char* subchar_array = strtok(sensors, "-");
		while (subchar_array) {
			this->sensorList.push_back(subchar_array);
//            cout << subchar_array << '\n';
			subchar_array = strtok(NULL, "-");
		}
	}
	
}

void TcpSocket::ProcessMessage(char * message){
	char * copyMessage = (char*)malloc(strlen(message) + 1);    
	char * aux;
	strcpy(copyMessage, message);

	//Detect the command
	aux = strtok(copyMessage, ":");
//	cout <<  "COMMAND: " << command << endl;

	if(strcmp(aux, "MOV") == 0){
		this->command = MOV;
		movement = strtok(0, ":");
//        cout <<  "MOVEMENT: " << movement << endl;
	}
	else if(strcmp(aux, "EXIT") == 0){
		this->command = EXIT;
		cout << "Closing: " << this->getSocketId() << endl;
		this->closeSocket();
	}
}

bool TcpSocket::createExploration(){
	int retval;

	char query[500];

	conn = mysql_init (NULL);

	if (strcmp(explorationName,"NONE") != 0){

		if (!mysql_real_connect(conn,host,db_user,db_pass,database,80,NULL,0)) {
			fprintf(stderr,"Error de Conexión: %s\n",mysql_error(conn));
			return false;
		}
		else{
			sprintf(query,"INSERT INTO EXPLORATION (name) VALUES ('%s');", explorationName);
			retval = mysql_query(conn,query);
			if(retval==0){
	//          printf("Inserción Correcta EXPLORATION: %s\n", query);
				return true;
			}
			else{
	//          fprintf(stderr,"Inserción Incorrecta EXPLORATION: %s\n",mysql_error(conn));
				return false;
			}
		}
	}

	mysql_close(conn);
	return false;
}

void TcpSocket::createSensors(){
	//Sensor * s; // Vector de sensores
	Sensor * s;
	
	for (int i= 0; i<sensorList.size(); i++) {

		if( (strcmp(sensorList[i].c_str(), "DHT22HUM") == 0) || (strcmp(sensorList[i].c_str(), "DHT22TEMP") == 0) || (strcmp(sensorList[i].c_str(), "DHT22") == 0) ){
			s = Sensor::crearObjetoSensor("DHT22", sensorList[i]);
			s->setName("DHT22");
		}
		else if(strcmp(sensorList[i].c_str(),"CAMERA") == 0){
			s = Sensor::crearObjetoSensor("CAMERA", to_string(getSocketId()) );
			s->setName("CAMERA");
		}
		else{
			s = Sensor::crearObjetoSensor(sensorList[i]); //Tenemos que meter el tipo del sensor que es.
			s->setName(sensorList[i]);
		}
		s->setExplorationName(explorationName);
		s->init(timeSensors);
		sensors.push_back(s);
	}
}
