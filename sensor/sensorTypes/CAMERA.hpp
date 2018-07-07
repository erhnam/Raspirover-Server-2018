#ifndef __CAMERA_HPP__
#define	__CAMERA_HPP__

#include "../sensor.hpp"
#include <iostream>

class CAMERA : public Sensor {
    
public:

    CAMERA():Sensor() {}
    
    ~CAMERA() {
        if (update_thread.joinable()) update_thread.join();
    }
    
    bool init(unsigned int millisecs) {
        width = 320;
        height= 240;
        formato= "GRAY";
        value = nullptr;

        //Configuración de la cámara
        cam.setWidth(width);
        cam.setHeight(height);
        cam.setVideoStabilization(true);
        cam.setHorizontalFlip (true);
        cam.setVerticalFlip(true);
        
        //Comprueba el formato de la imagen
        if (formato == "RGB"){
            cam.setFormat(RASPICAM_FORMAT_RGB);
        }
        else{
            cam.setFormat(RASPICAM_FORMAT_GRAY);
        }
        
        //Abrir cámara
        if ( !cam.open()) {
            cerr << "Error inicializando la cámara\n";
            return false;
        }
        
        //Asignación de memoria
        bytes = cam.getImageBufferSize();

        setUpdate_interval(35);
        running = true;
        update_thread = thread(&Sensor::update, this, ref(running), millisecs) ;
        return true;
    }
    
    void closeSensor() {
        running = false ;
        update_thread.join() ;
        //Libera la cámara
        cam.release();
    }
    
    bool readData() {
        //Captura
        cam.grab();
        //Extrae la imagen en formato RGB
        cam.retrieve ( value, RASPICAM_FORMAT_GRAY );
        cout << "CAMARA LEIDO" << endl;
        return true;
    }
    
    unsigned int getWidth() {
        return width;
    }

    unsigned int getHeight() {
        return height;
    }    

    void update( atomic<bool>& program_is_running, unsigned int update_interval_millisecs ){
        const auto wait_duration = chrono::milliseconds(update_interval_millisecs) ;
        while( program_is_running ){
            readData();
            //meter valores en MySQL y poner Semáforos
            this_thread::sleep_for(wait_duration) ; 
        }
    }    

protected:
       
    unsigned int width;
    
    unsigned int height;

    string formato;
    
    unsigned char * value;

    //Objeto de la clase cámara Noir
    RaspiCam cam;

    //Variable para el tamaño de la imagen
    unsigned long bytes;

};


#endif

