#ifndef __ROBOT_H
#define __ROBOT_H

extern "C" {
#include "extApi.h"
}
#include "config.h"
#include "object.h"
#include "datas.h"
#include <iostream>	

	class robot{
		private:
			object leftWheel;
			object rightWheel;
			object Robot;
			int clientID;
		public:
			robot();
			void getHandle();
			void setClientID(int cID);
			void setVel(float rW, float lW);
			datas getData();
	};
#endif
