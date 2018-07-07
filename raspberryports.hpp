#ifndef __RASPBERRYPORTS_HPP__
#define	__RASPBERRYPORTS_HPP__

// Para conocer si se incluye simulación o no
//#ifndef SIMU_WIRINGPI
/*
#include <gps/serial.h>
#include <gps/nmea.h>
#include <gps/gps.h>
#include <raspicam/raspicam.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <nuestroSPI.h>
*/
#include "simu/wiringPi.h"
#include "simu/gps.h"
#include "simu/raspicam.h"
#include "simu/raspicamtypes.h"
#include <iostream>

//using namespace raspicam;

//#else
/*
#include <simu/wiringPi.h>
#include <simu/gps.h>
#include <simu/raspicam.h>
#include <simu/raspicamtypes.h>
#include <iostream>
*/
using namespace std;

inline int nuestroSPIGetFd (int channel) {return 1;}
inline int nuestroSPIDataRW (int channel, unsigned char *data, int len){return 0;}
inline int nuestroSPISetup (int channel, int speed){return 1;}

#endif

