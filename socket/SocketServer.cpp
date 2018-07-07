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
	if ((newSocket = accept(socketId, (struct sockaddr *)&clientAddress, &clientAddressLen)) == -1){

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
    close(socketId);
}

void TcpSocket::ProcessConfigurationMessage(char * message){
    char * copyMessage = (char*)malloc(strlen(message) + 1);    
    char * aux;
    strcpy(copyMessage, message);

    //Detect the command
    aux = strtok(copyMessage, ":");
    cout <<  "COMMAND: " << aux << endl;

    //Operate with the command
    if(strcmp(aux, "OPT") == 0){
        this->command = OPT;
        char * sensors;

        sensors = strtok(0, "/");
//        cout <<  "SENSORS: " << sensors << endl;

        timeSensors = atoi(strtok(0, "/"));
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
    cout <<  "COMMAND: " << command << endl;

    //Operate with the command
    if(strcmp(aux, "OPT") == 0){
        this->command = OPT;
        char * sensors;

        sensors = strtok(0, "/");
//        cout <<  "SENSORS: " << sensors << endl;

        timeSensors = atoi(strtok(0, "/"));
//        cout <<  "TIME: " << timeSensors << endl;

        char* subchar_array = strtok(sensors, "-");
        while (subchar_array) {
            this->sensorList.push_back(subchar_array);
//            cout << subchar_array << '\n';
            subchar_array = strtok(NULL, "-");
        }
    }
    else if(strcmp(aux, "MOV") == 0){
        this->command = MOV;
        movement = strtok(0, ":");
//        cout <<  "MOVEMENT: " << movement << endl;
    }
    else if(strcmp(aux, "EXIT") == 0){
        this->command = EXIT;
//        cout << "Closing: " << this->getSocketId() << endl;
        //this->closeSocket();
    }


}
