#include "hostInfo.hpp"

void HostInfo::error(const char *msg){
    perror(msg);
    exit(1);
}

HostInfo::HostInfo(){ 
	openHostDb();
}

HostInfo::HostInfo(string& hostName, hostType type){
	
	searchHostDB = 0;

	if (type == NAME){
		// Retrieve host by name
		hostPtr = gethostbyname(hostName.c_str());
		if (hostPtr == NULL)
			error("ERROR");

    }
	else if (type == ADDRESS){
		// Retrieve host by address
	    unsigned long netAddr = inet_addr(hostName.c_str());
		if (netAddr == -1){
			error("ERROR");
		}

        hostPtr = gethostbyaddr((char *)&netAddr, sizeof(netAddr), AF_INET);

		if (hostPtr == NULL){
			error("ERROR");
		}
    }
}

char HostInfo::getNextHost(){
	// Get the next host from the database
    if (searchHostDB == 1){
		if ((hostPtr = gethostent()) == NULL)
			return 0;
		else
			return 1;
    }
    return 0;
}
