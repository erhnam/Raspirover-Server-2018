#ifndef __DHT22_HPP__
#define	__DHT22_HPP__

#include "../sensor.hpp"

#define MAX_TIMINGS	85

struct dht22Data{
	float temp;
	float hum;
};


class DHT22 : public Sensor {
		
	public:

		DHT22(string _type):Sensor() {
			this->type = _type;
		}
		
		~DHT22() {
			if (update_thread.joinable()) update_thread.join();
		}

		bool init(unsigned int millisecs) {
			value.temp = 0;
			value.hum = 0;
			pin = 2;
			if(getExplorationName() == "NONE"){
				setDataBase(false);
			}
			else{
				setDataBase(true);
				if(!getIdExploration())
					return false;
			}
			
			setUpdate_interval(millisecs);
			running = true;
			update_thread = thread(&Sensor::update, this, ref(running), millisecs) ;
			return true;
		}    
			
		void closeSensor() {			
			running = false ;
			update_thread.join() ;
		}

		bool readData() {
			/*
			uint8_t laststate = HIGH;
			uint8_t counter = 0;
			uint8_t j = 0, i;

			data[0] = data[1] = data[2] = data[3] = data[4] = 0;

			pinMode( pin , OUTPUT );
			digitalWrite( pin, LOW );
			delay( 18 );

			pinMode( pin , INPUT );

			for ( i = 0; i < MAX_TIMINGS; i++ ){
				counter = 0;
				while ( digitalRead( pin ) == laststate ){
						counter++;
						delayMicroseconds( 1 );
						if ( counter == 255 ){
								break;
						}
				}

				laststate = digitalRead( pin );

				if ( counter == 255 ){
					 break;
				}

				if ( (i >= 4) && (i % 2 == 0) ){
						data[j / 8] <<= 1;
						if ( counter > 16 ){
								data[j / 8] |= 1;
						}
					j++;
				}
			}
			if ( (j >= 40) && (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) ){
				if ( strcmp(type.c_str(), "DHT22HUM") == 0){
					value.hum = (float)((data[0] << 8) + data[1]) / 10;
					return true;
				}
				else if (strcmp(type.c_str(), "DHT22TEMP") == 0){
					value.temp = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
					if ( data[2] & 0x80 ){
							value.temp = -value.temp;
					}
					return true;
				}

				else{
					value.hum = (float)((data[0] << 8) + data[1]) / 10;
					value.temp = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
					if ( data[2] & 0x80 ){
							value.temp = -value.temp;
					}
					return true;
				}
			}
			else{
				return false;
			}
			*/
			value.temp = rand() % 100;
			value.hum = rand() % 100;
			return true;
		}

		void update( atomic<bool>& program_is_running, unsigned int update_interval_millisecs ){
        	mtx.lock();
			const auto wait_duration = chrono::milliseconds(update_interval_millisecs) ;
			while( program_is_running ){
				readData();
				if (getDataBase())
					storeDataInDB();
				cout << "TEMP: " << value.temp << endl;
				this_thread::sleep_for(wait_duration) ; 
			}
        	mtx.unlock();
		}

		void setData(dht22Data _value){
			value.temp = _value.temp;
			value.hum = _value.hum;

		}

		dht22Data getData(){
			return value;
		}

		bool storeDataInDB(){
			conn = mysql_init (NULL);

			if (!mysql_real_connect(conn,host,db_user,db_pass,database,8080,NULL,0)) {
				fprintf(stderr,"Error de Conexión: %s\n",mysql_error(conn));
				return false;
			}
			else{
				if(strcmp(type.c_str(), "DHT22HUM") == 0){
					sprintf(query,"INSERT INTO %s(fk_exp, hum, type) VALUES (%u, %4.2f,'%s');", getName().c_str(), id_exploration, getData().hum, type.c_str());
				}
				else if(strcmp(type.c_str(), "DHT22TEMP") == 0){
					sprintf(query,"INSERT INTO %s(fk_exp, temp, type) VALUES (%u, %4.2f,'%s');", getName().c_str(), id_exploration, getData().temp, type.c_str());
				}
				else if(strcmp(type.c_str(), "DHT22") == 0){
					sprintf(query,"INSERT INTO %s(fk_exp, temp, hum, type) VALUES (%u, %4.2f, %4.2f, '%s');", getName().c_str(), id_exploration, getData().temp, getData().hum, type.c_str());
				}

				if(mysql_query(conn,query) == 0){
					printf("Inserción DHT22 Correcta\n");
					addIdSensor();
				}
				else{
					fprintf(stderr,"Inserción Incorrecta: %s\n",mysql_error(conn));
				}
			}
			mysql_close(conn);
			return true;

		}

	protected:
		
		int pin;

		int data[5] = {0,0,0,0,0}; 

		dht22Data value;

		string type = "DHT22";
		   
};


#endif

