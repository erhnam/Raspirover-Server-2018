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
            value = 0;
            channel = 0;
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
    		value = (unsigned short)myAnalogRead( 0, 8, channel );
            cout << "FIRE LEIDO" << endl;
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

        void setData(unsigned int _value){
            value = _value;
        }

        unsigned int getData(){
            return value;
        }

    protected:
            
        int channel;

        unsigned int value;
    
};


#endif