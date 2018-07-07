#ifndef __L298N_HPP__
#define __L298N_HPP__

#include <inttypes.h>

#define L298N_ENA 13
#define L298N_IN1 6
#define L298N_IN2 5
#define L298N_IN3 27
#define L298N_IN4 22
#define L298N_ENB 12

#define DEF_SPEED		10
#define MAX_SPEED       100
#define MIN_SPEED       10

#define PWM_PERIOD      100
#define PWM_STOP        0
#define PWM_MAX         (MAX_SPEED)

class L298N{
	public:
	    //- Constructors:
	    L298N();
		
		//- Functions:
		void begin();
		void goForward();
		void goBack();
		void goLeft();
		void goRight();
		void stop();

		void setSpeed(int _speed);
		int getSpeed();
		
	private:
		uint8_t speed;
		
		int    enAPin;
		int    en1Pin;
		int    en2Pin;
		int    enBPin;
		int    en3Pin;
		int    en4Pin;		
};

#endif
