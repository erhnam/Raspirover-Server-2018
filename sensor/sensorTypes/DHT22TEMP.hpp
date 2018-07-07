#ifndef __DHT22TEMP_HPP__
#define	__DHT22TEMP_HPP__

#include "../sensor.hpp"

#define MAX_TIMINGS	85

class DHT22TEMP : public Sensor {
	
	public:

		DHT22TEMP():Sensor(){}
	
		~DHT22TEMP() {
			if (update_thread.joinable()) update_thread.join();
		}

		bool init(unsigned int millisecs){
			value = 0;
			pin = 2;
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
				float h = (float)((data[0] << 8) + data[1]) / 10;
				if ( h > 100 ){
						h = data[0];    // for DHT11
				}
				float c = (float)(((data[2] & 0x7F) << 8) + data[3]) / 10;
				if ( c > 125 ){
						c = data[2];    // for DHT11
				}
				if ( data[2] & 0x80 ){
						c = -c;
				}
				//float f = c * 1.8f + 32;
		        
				setData(c);
				cout << "TEMP LEIDO" << endl;
				return true;
			}
			else{        
				return false;
			}

			
		}

    void update( atomic<bool>& program_is_running, unsigned int update_interval_millisecs ){
        const auto wait_duration = chrono::milliseconds(update_interval_millisecs) ;
        while( program_is_running ){
            readData();
            //meter valores en MySQL y poner Semáforos
            this_thread::sleep_for(wait_duration) ; 
        }
    }

    void setData(float _value){
    	value = _value;
    }

    float getData(){
    	return value;
    }

	protected:

		int pin;

		int data[5] = {0,0,0,0,0}; 

		float value;	   
};


#endif

