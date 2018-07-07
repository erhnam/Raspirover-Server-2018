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
        
        int echo;
        
        int trigger;
            
        float value;
};

#endif

