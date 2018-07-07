#ifndef __NEO6MV2_HPP__
#define	__NEO6MV2_HPP__

#include "../sensor.hpp"
#include <sys/mman.h>
#include <stdio.h>

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

protected:
    
    int tx;
    
    int rx;

    GPSData value;
        
};


#endif

