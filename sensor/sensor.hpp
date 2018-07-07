#ifndef __SENSOR_HPP__
#define	__SENSOR_HPP__

#include <string>
#include <thread>
#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>
#include "../raspberryports.hpp"


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
#define SENSORCLASSES 8

const string sensorClassNames[]= {"DHT22TEMP", "DHT22HUM", "HCSR04", "NEO6MV2", "FIRE", "LDR", "MQ2", "CAMERA"};

class Sensor {

    public:
        Sensor() {}

        virtual ~Sensor() {}

        virtual void closeSensor()= 0;

        virtual bool readData() = 0;

        virtual bool init(unsigned int millisecs) = 0;

        virtual void update( atomic<bool>& program_is_running, unsigned int update_interval_millisecs ) = 0;

        string getName() const { return name;}

        void setName(const string &_name) {name= _name;}

        void setUpdate_interval(unsigned int millisecs){this->update_interval = millisecs;}

        unsigned int getUpdate_interval() const { return this->update_interval;}

        static Sensor *crearObjetoSensor(const string &sensorClassName);

    protected:

        string name;

        atomic <bool> running { false } ;

        unsigned int update_interval;

        thread update_thread;

};


#endif	/* __SENSOR_HPP__ */

