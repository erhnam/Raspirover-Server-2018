#include "sensorTypes/DHT22TEMP.hpp"
#include "sensorTypes/DHT22TEMP.hpp"
#include "sensorTypes/DHT22HUM.hpp"
#include "sensorTypes/HCSR04.hpp"
#include "sensorTypes/NEO6MV2.hpp"
#include "sensorTypes/FIRE.hpp"
#include "sensorTypes/LDR.hpp"
#include "sensorTypes/MQ2.hpp"
#include "sensorTypes/CAMERA.hpp"
#include "sensor.hpp"

#include <iostream>

using namespace std;
           
    Sensor * Sensor::crearObjetoSensor(const string &sensorClassName) {

        Sensor *s= 0; // Sensor a crear

        if (sensorClassName == sensorClassNames[0]) {
            s= new DHT22TEMP();
        }else if (sensorClassName == sensorClassNames[1]) {
            s= new DHT22HUM();
        } else if (sensorClassName == sensorClassNames[2]) {
            s= new HCSR04();
        } else if (sensorClassName == sensorClassNames[3]) {
            s= new NEO6MV2();
        } else if (sensorClassName == sensorClassNames[4]) {
            s= new FIRE();
        } else if (sensorClassName == sensorClassNames[5]) {
            s= new LDR();
        } else if (sensorClassName == sensorClassNames[6]) {
            s= new MQ2();
        } else if (sensorClassName == sensorClassNames[7]) {
            s= new CAMERA();
        }
        return s;
    }
