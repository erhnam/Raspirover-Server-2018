/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   wiringPi.h
 * Author: paco
 *
 * Created on 25 de abril de 2017, 15:07
 */

#ifndef WIRINGPI_H
#define WIRINGPI_H

#define	LOW     0
#define	HIGH	1
#define	INPUT   0
#define	OUTPUT	1


#include <cstdlib>
#include <ctime>

inline int wiringPiSetupGpio(){return 1;};
inline unsigned int micros (void){return 1000;}; //1 segundo
inline void delay (unsigned int howLong){};
inline int digitalRead (int pin){srand(time(0)); return rand()%2;};
inline void digitalWrite (int pin, int value){};
inline int wiringPiSPISetup (int , int) {return 1;}
inline void delayMicroseconds(unsigned int howLong ) {};
inline void pinMode (int pin, int mode){} ;
inline int wiringPiSPIDataRW (int channel, unsigned char *data, int len){return 1;} ;

#endif /* WIRINGPI_H */

