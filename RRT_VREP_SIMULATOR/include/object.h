#ifndef __OBJECT_H
#define __OBJECT_H

extern "C" {
#include "extApi.h"
}
#include <string>
#include <iostream>
#include "datas.h"
class object
{
private:
	int clientID;
	float orient[3];
	datas status;
public:
	float position[3];
	int object_handle;
	std::string name;
	object();
	object(std::string names);
	//~object();
	void setClientID(int cID);
	void getHandle();
	datas getData();
	
	
};

#endif
