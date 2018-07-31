#include "sensorTypes/DHT22.hpp"
#include "sensorTypes/HCSR04.hpp"
//#include "sensorTypes/NEO6MV2.hpp"
#include "sensorTypes/FIRE.hpp"
#include "sensorTypes/LDR.hpp"
#include "sensorTypes/MQ2.hpp"
#include "sensorTypes/CAMERA.hpp"
#include "sensor.hpp"

#include <iostream>

using namespace std;
		   
	Sensor * Sensor::crearObjetoSensor(const string &sensorClassName) {

		Sensor *s= 0; // Sensor a crear

		if (sensorClassName == sensorClassNames[1]) {
			s= new HCSR04();
			s->setName("HCSR04");
		} 
/*		else if (sensorClassName == sensorClassNames[2]) {
			s= new NEO6MV2();
			s->setName("NEO6MV2");
		} 
*/		else if (sensorClassName == sensorClassNames[3]) {
			s= new FIRE();
			s->setName("FIRE");
		} 
		else if (sensorClassName == sensorClassNames[4]) {
			s= new LDR();
			s->setName("LDR");
		} 
		else if (sensorClassName == sensorClassNames[5]) {
			s= new MQ2();
			s->setName("MQ2");
		} 
		return s;
	}

	Sensor * Sensor::crearObjetoSensor(const string &sensorClassName, const string &_type) {

		Sensor *s= 0; // Sensor a crear

		if (sensorClassName == sensorClassNames[0]) {
			s= new DHT22(_type);
		}
		else if (sensorClassName == sensorClassNames[6]) {
			s= new CAMERA(_type);
			s->setName("CAMERA");
		}
		return s;
	}

	bool Sensor::getLastIdSensor() {

		res = NULL;
		row = NULL;
		conn = mysql_init (NULL);

		if (!mysql_real_connect(conn,host,db_user,db_pass,database,8080,NULL,0)) {
			fprintf(stderr,"Error de Conexión: %s\n",mysql_error(conn));
			return false;
		}
		else{
			sprintf(query,"SELECT MAX(id) FROM '%s';", getName().c_str());
			if (mysql_query(conn,query) != 0){
				cout<<"Error: mysql_real_query ha fallado en la consulta:" << endl;
				mysql_close(conn);
				return false;
			}
			else{
				res = mysql_store_result(conn);
				if( res == NULL ){
					cout << "Not enough memory to allocate to res" << endl;
					mysql_close(conn);
					return false;
				} 
				row = mysql_fetch_row(res); 
				if( row == NULL ){
					cout << "Nothing returned" << endl;
					mysql_close(conn);
					return false;
				}
				else{
					if (row[0] == NULL)
						id_sensor = 1;
					else{
						id_sensor = atoi(row[0])+1;
					}
				}
				mysql_free_result(res);		
					
			} 
		}

		mysql_close(conn);
		return true; 

	}    

	bool Sensor::getIdExploration(){
		conn = mysql_init (NULL);

		if (!mysql_real_connect(conn,host,db_user,db_pass,database,8080,NULL,0)) {
			fprintf(stderr,"Error de Conexión: %s\n",mysql_error(conn));
			mysql_close(conn);
			return false;
		}
		else{
			sprintf(query,"SELECT exp_id FROM EXPLORATION WHERE name LIKE '%s';", getExplorationName().c_str());
			cout << query << endl;
			if (mysql_query(conn,query) != 0){
				cout<<"Error: mysql_real_query ha fallado en la consulta:" << endl;
				return 0;
			}
			else{
				res = mysql_store_result(conn);
				if( res == NULL ){
					cout << "Not enough memory to allocate to res" << endl;
					mysql_close(conn);
					return false;
				} 
				row = mysql_fetch_row(res); 
				if( row == NULL ){
					cout << "Nothing returned" << endl;
					mysql_close(conn);
					return false;
				}
				else{
					if (row[0] == NULL)
						id_exploration = 1;
					else{
						id_exploration = atoi(row[0]);
					}
				}									

				mysql_free_result(res);		

			}
		}
		mysql_close(conn);
		return true;	
	}


