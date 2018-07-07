/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   gps.h
 * Author: paco
 *
 * Created on 26 de abril de 2017, 11:03
 */

#ifndef GPS_H
#define GPS_H

struct location {
    double latitude;
    double longitude;
    double speed;
    double altitude;
    double course;
};
typedef struct location loc_t;

// Initialize device
inline void gps_init(void){};
// Activate device
inline void gps_on(void){};
// Get the actual location
inline void gps_location(loc_t *){};


// Turn off device (low-power consumption)
inline void gps_off(void){};

// -------------------------------------------------------------------------
// Internal functions
// -------------------------------------------------------------------------

// convert deg to decimal deg latitude, (N/S), longitude, (W/E)
inline void gps_convert_deg_to_dec(double *, char, double *, char) {}
inline double gps_deg_dec(double){return 0;}

#endif /* GPS_H */

