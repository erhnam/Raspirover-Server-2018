/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   raspicam.h
 * Author: paco
 *
 * Created on 26 de abril de 2017, 11:08
 */

#ifndef RASPICAM_H
#define RASPICAM_H

#include <cstdio>
#include <string>
#include "raspicamtypes.h"
#include "private_impl.h"

using namespace std;


    /**Base class that do all the hard work
     */
    class RaspiCam
    {
        public:
        /**Constructor
         */
        RaspiCam() {}
    

        /**Destructor
         */
        ~RaspiCam() {}
        /**Opens the camera
        * @param StartCapture determines if camera must start capture or not.
              */
        inline bool open ( bool StartCapture=true ){return true;};
        /**Makes camera start capturing
         */
        inline bool startCapture(){return true;};
        /**indicates if camera is open
        */
        inline bool isOpened() const{return true;} ;
        /**Grabs the next frame and keeps it in internal buffer. Blocks until next frame arrives
        */
        inline bool grab(){return true;};
        /**Retrieves the buffer previously grabbed.
         * You can decide how image is given by setting type. The input buffer provided must have the appropriate
         * size accordingly. You can use getImageTypeSize() to determine the size
            * NOTE: Change in version 0.0.5. Format is stablished in setFormat function
            * So type param is ignored. Do not use this parameter.
            * You can use getFormat() to know the current format
          */
        inline void retrieve ( unsigned char *data,RASPICAM_FORMAT type=RASPICAM_FORMAT_IGNORE ){};
        /**Alternative to retrieve. Returns a pointer to the original image data buffer (which is in getFormat() format).
          *
          * Be careful, if you call grab(), this will be rewritten with the new data
          */
        inline unsigned char *getImageBufferData() const{ static unsigned char c[3]= {0,0,0}; return c;};
        /**
         * Returns the size of the images captured.
         */
        inline size_t getImageBufferSize() const{return sizeof(unsigned short);};


        /** Stops camera and free resources
        */
        inline void release(){};

        /**Sets capture format
         */
        inline void setFormat ( RASPICAM_FORMAT fmt ){};
        /**Sets camera width. Use a multiple of 320 (640, 1280)
         */
        inline void setWidth ( unsigned int width ){} ;
        /**Sets camera Height. Use a multiple of 240 (480, 960)
         */
        inline void setHeight ( unsigned int height ){};

        inline void setCaptureSize ( unsigned int width, unsigned int height ){};
        /** Set image brightness [0,100]
         */
        inline void setBrightness ( unsigned int brightness );
        /**
         * Set image sharpness (-100 to 100)
         */
        inline void setSharpness ( int sharpness );
        /**
         *  Set image contrast (-100 to 100)
         */
        inline void setContrast ( int contrast );
        /**
         * Set capture ISO (100 to 800)
         */
        inline void setISO ( int iso ){};
        /**
         * Set image saturation (-100 to 100)
         */
        inline void setSaturation ( int saturation ){};
        /**Sets on/off video stabilisation
         */
        inline void setVideoStabilization ( bool v ){};
        /**
         *  Set EV compensation (-10,10)
         */
        inline void setExposureCompensation ( int val ); //-10,10
        inline void setRotation ( int rotation );
        inline void setExposure ( RASPICAM_EXPOSURE exposure );
        inline void setShutterSpeed ( unsigned int ss );
        inline void setAWB ( RASPICAM_AWB awb );
        // et specific values for whitebalance. Requires to set seAWB in OFF mode first
        inline void setAWB_RB ( float r,float b );//range is 0-1.
        inline void setImageEffect ( RASPICAM_IMAGE_EFFECT imageEffect );
        inline void setMetering ( RASPICAM_METERING metering );
        inline void setHorizontalFlip ( bool hFlip ){};
        inline void setVerticalFlip ( bool vFlip ){};
        inline void setFrameRate ( int frames_per_second );

        //Accessors
        inline unsigned int getWidth() const{return 800;};
        inline unsigned int getHeight() const{return 600;};
        RASPICAM_EXPOSURE getExposure() const ;
        RASPICAM_AWB getAWB() const;
        int getFrameRate() const;
        inline bool isHorizontallyFlipped() const{return true;};
        inline bool isVerticallyFlipped() const {return true;};

        /**Returns the size of the required buffer for the different image types in retrieve
         */
        inline size_t getImageTypeSize ( RASPICAM_FORMAT type ) const {return 600;};
        
        private:

    };

#endif /* RASPICAM_H */

