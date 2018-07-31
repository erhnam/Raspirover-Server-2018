#ifndef __NEO6MV2_HPP__
#define	__NEO6MV2_HPP__

#include "../sensor.hpp"
#include <sys/mman.h>
#include <stdio.h>
#include "../../gps/gps.h"

struct GPSData {
	double lat;
	double lon;
};


class NEO6MV2 : public Sensor {

	public:

		NEO6MV2():Sensor() {}

		~NEO6MV2() {
			if (update_thread.joinable()) update_thread.join();
		}

		bool init(unsigned int millisecs) {
			tx= 14;
			rx= 15;
			value.lat = 0;
			value.lon = 0;
			gps_init();
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
			update_thread.join();    
			gps_off();
		}

		bool readData() {
		   //Variable para los datos GPS
			loc_t data;

			gps_location(&data);

			value.lat = data.latitude;
			value.lon = data.longitude;
			cout << "GPS LEIDO" << endl;

			//printf("%lf %lf\n" ,dataOut->lat , dataOut->lon );
			return true;
		}

		void update( atomic<bool>& program_is_running, unsigned int update_interval_millisecs ){
			std::lock_guard<std::mutex> guard(mtx);
			const auto wait_duration = chrono::milliseconds(update_interval_millisecs) ;
			while( program_is_running ){
				readData();
				//meter valores en MySQL y poner Semáforos
				this_thread::sleep_for(wait_duration) ; 
			}
		}
		
		void setData(GPSData _value){
			value.lat = _value.lat;
			value.lon = value.lon;
		}

		GPSData getData(){
			return value;
		}

		bool storeDataInDB(){
			//inizio codice creazione data
			time_t rawtime;
			struct tm *info;
			char hora[25];
			char fecha[25];
			int retval;

			time(&rawtime);
			info = localtime(&rawtime);
			strftime(fecha,sizeof(fecha),"%Y-%m-%d",info);
			strftime(hora,sizeof(hora),"%H:%M:%S",info);

			char query[500];

			conn = mysql_init (NULL);

			if (!mysql_real_connect(conn,host,db_user,db_pass,database,8606,NULL,0)) {
				fprintf(stderr,"Error de Conexión: %s\n",mysql_error(conn));
				return false;
			}
			else{
				sprintf(query,"INSERT INTO NEO6MV2 (id_exploration, lat , lon, date, time) VALUES (%u, %f, %f, '%s','%s');", id_exploration, getData().lat, getData().lon, fecha, hora);
				retval = mysql_query(conn,query);
				if(retval==0){
					printf("Inserción Correcta: %s\n", query);
					return true;
				}
				else{
					fprintf(stderr,"Inserción Incorrecta: %s\n",mysql_error(conn));
					return false;
				}
			}
			mysql_close(conn);
			return true;

		}
		
		bool getIdExploration(){
			conn = mysql_init (NULL);

			if (!mysql_real_connect(conn,host,db_user,db_pass,database,8080,NULL,0)) {
				fprintf(stderr,"Error de Conexión: %s\n",mysql_error(conn));
				return false;
			}
			else{
				sprintf(query,"SELECT id FROM EXPLORATION WHERE name LIKE '%s';", getExplorationName().c_str());
				retval = mysql_query(conn,query);
				MYSQL_RES *res;
				res = mysql_use_result(conn);
				MYSQL_ROW row = mysql_fetch_row(res);
				if(retval==0){
					printf("Inserción Correcta con ID: %d\n", atoi(row[0]));
					id_exploration = atoi(row[0]);
				}
				else{
					fprintf(stderr,"Inserción Incorrecta %s\n", query);
					return false;
				}
			}
			mysql_close(conn);
			return true;	
		}

	protected:
		
		int tx;
		
		int rx;

		GPSData value;
		
};


#endif

