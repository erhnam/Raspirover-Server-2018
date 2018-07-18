#ifndef __SENSOR_HPP__
#define	__SENSOR_HPP__

#include <string>
#include <cstring>
#include <thread>
#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>
#include "../raspberryports.hpp"
#include <mutex>         
#include <condition_variable>
#include <cstdlib>
#include <vector>
#include <time.h>


#include "/usr/local/Cellar/mariadb/10.3.8/include/mysql/mysql.h"

using namespace std;

const string sensorPortTypeNames[]= {"GPIO", "SPI", "CSI", "I2C", "UART", "USB"};

#define RASPNUMSENSORPORTTYPES 6
#define SENSOR_INVALID -1
#define SENSOR_GPIO 0
#define SENSOR_SPI0 1
#define SENSOR_CSI 2
#define SENSOR_I2C 3
#define SENSOR_UART 4
#define SENSOR_USB 5
#define SENSORCLASSES 7

const string sensorClassNames[]= {"DHT22", "HCSR04", "NEO6MV2", "FIRE", "LDR", "MQ2", "CAMERA"};

class Sensor {

	public:
		Sensor() {}

		Sensor(Sensor const &) {}

		virtual ~Sensor() {}

		virtual void closeSensor()= 0;

		virtual bool readData() = 0;

		virtual bool storeDataInDB() = 0;

		virtual bool init(unsigned int millisecs) = 0;

		virtual void update( atomic<bool>& program_is_running, unsigned int update_interval_millisecs ) = 0;

		bool getIdExploration();

		string getName() const { return name;}

		bool getLastIdSensor();

		unsigned int getIdSensor() {return id_sensor;}

		void addIdSensor() { id_sensor = id_sensor + 1;}

		void setName(const string &_name) {name= _name;}

		string getExplorationName() const { return explorationName;}

		void setExplorationName(const string &_name) {explorationName= _name;}

		void setUpdate_interval(unsigned int millisecs){this->update_interval = millisecs;}

		unsigned int getUpdate_interval() const { return this->update_interval;}

		static Sensor * crearObjetoSensor(const string &sensorClassName);

		static Sensor * crearObjetoSensor(const string &sensorClassName, const string &type);

		bool getDataBase () const {return this->db;};

		void setDataBase (bool _db) {this->db = _db;};

	protected:

		string name;

		string explorationName;

		atomic <bool> running { false };

		unsigned int update_interval;

		thread update_thread;

		mutex mtx;

		MYSQL *conn;

		MYSQL_ROW row;

		MYSQL_RES *res;

		const char* host = "localhost";

		const char* database = "db_rover";

		const char* db_user = "raspirover";

		const char* db_pass = "raspirover";

		unsigned int id_exploration;

		unsigned int id_sensor;

		char query[500];

		int retval;

		bool db = false;

};


#endif	/* __SENSOR_HPP__ */

