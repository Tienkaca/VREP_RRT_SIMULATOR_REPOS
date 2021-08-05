#include "robot.h"

robot::robot()
{
	leftWheel.name = L_WHEEL_ID;
	rightWheel.name = R_WHEEL_ID;
	Robot.name = ROBOT_ID;
}
void robot::setClientID(int cID)
{
	leftWheel.setClientID(cID);
	rightWheel.setClientID(cID);
	Robot.setClientID(cID);
	clientID = cID;
}
void robot::getHandle()
{
	leftWheel.getHandle();
	rightWheel.getHandle();
	Robot.getHandle();
}

void robot::setVel(float rW, float lW)
{
	simxSetJointTargetVelocity(clientID, rightWheel.object_handle, rW, simx_opmode_oneshot);
	simxSetJointTargetVelocity(clientID, leftWheel.object_handle, lW, simx_opmode_oneshot);
}
datas robot::getData()
{
	return Robot.getData();
}