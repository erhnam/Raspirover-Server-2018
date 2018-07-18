#ifndef __HCSR04_HPP__
#define	__HCSR04_HPP__

#include "../sensor.hpp"

class HCSR04 : public Sensor {
	
	public:

		HCSR04():Sensor() {}
		
		~HCSR04() {
			if (update_thread.joinable()) update_thread.join();
		}
		
		bool init(unsigned int millisecs) {
			value = 0;
			echo= 13;
			trigger= 24;
			if(getExplorationName() == "NONE"){
				setDataBase(false);
			}
			else{
				setDataBase(true);
				if(!getIdExploration())
					return false;
			}			
			pinMode (trigger, OUTPUT); //Trigger
			pinMode (echo, INPUT); //Echo
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
			long inicio;
			long fin;
			double transcurrido;

			digitalWrite(trigger, HIGH);
			delayMicroseconds(10);
			digitalWrite(trigger, LOW);
			while(digitalRead(echo) == LOW);
			inicio = micros();
			while(digitalRead(echo) == HIGH);
			fin = micros();

			//Calculo del tiempo tardado
			transcurrido = fin-inicio;

			setData(((transcurrido/1000000.0)*34320)/2);
			
			cout << "HCSR04 LEIDO" << endl;
			return true;
		}

		void update( atomic<bool>& program_is_running, unsigned int update_interval_millisecs ){
        	std::lock_guard<std::mutex> guard(mtx);
			const auto wait_duration = chrono::milliseconds(update_interval_millisecs) ;
			while( program_is_running ){
				readData();
				this_thread::sleep_for(wait_duration) ; 
			}
		}

		void setData(float _value){
			value = _value;
		}

		float getData(){
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
				sprintf(query,"INSERT INTO HCSR04 (id_exploration, value, date, time) VALUES (%u, %f,'%s','%s');", id_exploration, getData(), fecha, hora);
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
		
		int echo;
		
		int trigger;
			
		float value;
};

#endif

