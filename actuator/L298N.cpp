/**************************************************
 * RNT-NTRD  SHK
 * 2015. 3. 05 Ver.01 
 **************************************************/

#include "L298N.h"

#include <wiringPi.h>
#include <wiringSerial.h>
#include <piFace.h>
#include <softPwm.h>

L298N::L298N(){
  this->enAPin = L298N_ENA;
  this->en1Pin = L298N_IN1;
  this->en2Pin = L298N_IN2;
  this->enBPin = L298N_ENB;
  this->en3Pin = L298N_IN3;
  this->en4Pin = L298N_IN4;
  this->speed = DEF_SPEED;
}

void L298N::begin(void){
    softPwmCreate(this->enAPin, PWM_STOP, PWM_PERIOD);
    softPwmCreate(this->enBPin, PWM_STOP, PWM_PERIOD);        
}

void L298N::goForward(){
  digitalWrite(this->en1Pin, HIGH);
  digitalWrite(this->en2Pin, LOW);

  digitalWrite(this->en3Pin, HIGH);
  digitalWrite(this->en4Pin, LOW);
}


void L298N::goBack(){

  digitalWrite(this->en1Pin, LOW);
  digitalWrite(this->en2Pin, HIGH);
  
  digitalWrite(this->en3Pin, LOW);
  digitalWrite(this->en4Pin, HIGH);
}

void L298N::goLeft(){
  digitalWrite(this->en1Pin, LOW);
  digitalWrite(this->en2Pin, HIGH);

  digitalWrite(this->en3Pin, HIGH);
  digitalWrite(this->en4Pin, LOW);
}

void L298N::goRight(){
  digitalWrite(this->en1Pin, HIGH);
  digitalWrite(this->en2Pin, LOW);

  digitalWrite(this->en3Pin, LOW);
  digitalWrite(this->en4Pin, HIGH);
}


void L298N::stop(){
  softPwmWrite(this->enAPin, PWM_STOP);
  softPwmWrite(this->enBPin, PWM_STOP);
  digitalWrite(this->en1Pin, LOW);
  digitalWrite(this->en2Pin, LOW);
  digitalWrite(this->en3Pin, LOW);
  digitalWrite(this->en4Pin, LOW);
}


void L298N::setSpeed(int speed){
  this->speed = speed;
  softPwmWrite(this->enAPin, this->speed);
  softPwmWrite(this->enBPin, this->speed);
}


int L298N::getSpeed(){
	return this->speed;
}


