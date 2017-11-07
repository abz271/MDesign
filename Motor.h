/*
 * Motor.h
 *
 *  Created on: 07.11.2017
 *      Author: torbe
 */

#ifndef MOTOR_H_
#define MOTOR_H_

class Motor {
public:
	Motor();
	virtual ~Motor();
	void sendMotorPower(float leftWheelPower, float rightWheelPower);
};

#endif /* MOTOR_H_ */
