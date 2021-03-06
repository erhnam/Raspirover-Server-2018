#ifndef __CAMERA_HPP__
#define	__CAMERA_HPP__
#include "../sensor.hpp"
#include <iostream>
#include <sys/socket.h>
//#include <unistd.h>

#include <chrono>
#include <thread>

class CAMERA : public Sensor {

public:

	CAMERA(string _type):Sensor() {
		this->socketId = atoi(_type.c_str());
	}

	~CAMERA() {
		if (update_thread.joinable()) update_thread.join();
	}

	bool init(unsigned int millisecs) {
		//Configuración de la cámara
		cam.setCaptureSize(320,240);
//		cam.setVideoStabilization(true);
	 	cam.setHorizontalFlip (true);
	        cam.setVerticalFlip(true);
		cam.setFormat(raspicam::RASPICAM_FORMAT_GRAY);

		//Abrir cámara
		if ( !cam.open()) {
			cerr << "Error inicializando la cámara\n";
			return false;
		}

		bytes=cam.getImageBufferSize();
//		cout << "BYTES: " << bytes << endl;
		value = new unsigned char[bytes];

		if(getExplorationName() == "NONE"){
			setDataBase(false);
		}
		else{
			setDataBase(true);
			if(!getIdExploration())
				return false;
		}

		setUpdate_interval(150);
                std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		running = true;
		update_thread = thread(&Sensor::update, this, ref(running), 150) ;
		return true;
	}

	void closeSensor() {
		running = false ;
		update_thread.join() ;
		//Libera la cámara
		cam.release();
		delete value;
	}

	bool readData() {
		unsigned long ini = micros();
		//Captura
		cam.grab();
		//Extrae la imagen en formato GRAY
		cam.retrieve ( value, RASPICAM_FORMAT_IGNORE );
//		cout << "CAMERA LEIDO" << endl;
		send(socketId,value,bytes,0);
		bzero(value, sizeof(value));
		cout << "Tiempo tardado: " << micros()-ini << endl;
	}

	void update( atomic<bool>& program_is_running, unsigned int update_interval_millisecs ){
	const auto wait_duration = chrono::milliseconds(update_interval_millisecs) ;
		while( program_is_running ){
			readData();
//			sendData();
			this_thread::sleep_for(wait_duration);
		}
	}

	void setData(unsigned char * _value){
		value = _value;
	}

	unsigned char * getData(){
		return value;
	}

	void sendData(){
//	    cout << "MANDANDO " << value << endl;
		send(socketId,value,bytes,0);
		bzero(value, sizeof(value));
	}

	bool storeDataInDB(){
		conn = mysql_init (NULL);

		if (!mysql_real_connect(conn,host,db_user,db_pass,database,8080,NULL,0)) {
			fprintf(stderr,"Error de Conexión: %s\n",mysql_error(conn));
			return false;
		}
		else{
			sprintf(query,"INSERT INTO %s (fk_exp, value, type) VALUES (%u, '%s', '%s');", getName().c_str(), id_exploration, getData(), getName().c_str());
			if(mysql_query(conn,query)==0){
				printf("Inserción CAMERA Correcta\n");
				addIdSensor();
			}
			else{
				fprintf(stderr,"Inserción CAMERA Incorrecta %s\n", query);
			}	
		}
		mysql_close(conn);
		return true;
	}

protected:
	   	
	//Objeto de la clase cámara Noir
	RaspiCam cam;

	unsigned char * value;

	unsigned int socketId;

	unsigned long bytes;

};


#endif

