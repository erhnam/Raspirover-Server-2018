#ifndef __FIRE_HPP__
#define	__FIRE_HPP__

#include "../sensor.hpp"

class FIRE : public Sensor {
	
	public:

		FIRE():Sensor() {}

		~FIRE() {
			if (update_thread.joinable()) update_thread.join();
		}
		
		//Leer valor analógico del bus SPI
		int myAnalogRead(int spiChannel,int channelConfig,int analogChannel) const {
			unsigned char buffer[3] = {1}; // start bit
			buffer[1] = (channelConfig+analogChannel) << 4;
			nuestroSPIDataRW(spiChannel, buffer, 3);
			return ( (buffer[1] & 3 ) << 8 ) + buffer[2]; // get last 10 bits
		}

		bool init(unsigned int millisecs) {
			channel = 7;
			value = 0;
			id_sensor = 1;
			res = NULL;
			row = NULL;
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
			update_thread = thread(&Sensor::update, this, ref(running), millisecs);
			return true;
		}
				
		void closeSensor() {            
			running = false ;
			update_thread.join() ;
		}

		bool readData() {
			value = (unsigned short)myAnalogRead( 0, 8, channel );
			return true;
		}

		void update( atomic<bool>& program_is_running, unsigned int update_interval_millisecs ){
			const auto wait_duration = chrono::milliseconds(update_interval_millisecs) ;
			mtx.lock();
			while( program_is_running ){
				readData();
				storeDataInDB();
				this_thread::sleep_for(wait_duration) ; 
			}
			mtx.unlock();

		}

		void setData(unsigned int _value){
			value = _value;
		}

		unsigned int getData(){
			return value;
		}    

		bool storeDataInDB(){
			conn = mysql_init (NULL);

			if (!mysql_real_connect(conn,host,db_user,db_pass,database,8080,NULL,0)) {
				fprintf(stderr,"Error de Conexión: %s\n",mysql_error(conn));
				return false;
			}
			else{
				sprintf(query,"INSERT INTO %s (fk_exp, value, type) VALUES (%u, %u, '%s');", getName().c_str(), id_exploration, getData(), getName().c_str());
				if(mysql_query(conn,query)==0){
					printf("Inserción FIRE Correcta\n");
					addIdSensor();
				}
				else{
					fprintf(stderr,"Inserción FIRE Incorrecta %s\n", query);
				}	
			}
			mysql_close(conn);
			return true;

		}

	protected:
		   
		int channel;
		
		unsigned int value;
};


#endif