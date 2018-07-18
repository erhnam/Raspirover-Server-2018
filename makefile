OBJECTOS = main.o SocketServer.o hostInfo.o sensor.o
FLAGS =  -g -Wall -O2 -std=c++11 

all: main clean

main: $(OBJECTOS)
	g++ $(FLAGS) -lmariadb -lz -liconv -lssl -lcrypto -o server.out $(OBJECTOS)
	@echo  Terminado
	@echo  Ejecute ./server.out para iniciar el programa. 

sensor.o: sensor/sensor.cpp
	g++ -c $(FLAGS) sensor/sensor.cpp 

main.o: main.cpp
	g++ -c $(FLAGS) main.cpp

SocketServer.o: socket/SocketServer.cpp
	g++ -c $(FLAGS) socket/SocketServer.cpp

hostInfo.o: socket/hostInfo.cpp
	g++ -c $(FLAGS) socket/hostInfo.cpp

clean:
	@echo Borrando ficheros.o ...
	rm *.o
	chmod +x server.out

.PHONY : clean
