#include "object.h"

object::object()
{
	clientID =0;
	object_handle = 0;
	name = "R";
}

object::object(std::string names)
{
	name = names;
	clientID =0;
	object_handle = 0;
}

void object::setClientID(int cID)
{
	clientID = cID;
}

void object::getHandle()
{
	int e3 =simxGetObjectHandle(clientID,name.c_str(), &object_handle, simx_opmode_blocking);

	if(e3 == simx_return_ok)
	{
		simxGetObjectPosition(clientID,object_handle,-1,position,simx_opmode_streaming);
		simxGetObjectOrientation(clientID,object_handle,-1,orient,simx_opmode_streaming);
		std::cout << name << "	SUCCESS TO GETHANDLE \n";
	}
	else
		std::cout << name <<"	FAIL TO GET GETHANDLE \n";
}

datas object::getData()
{
	if (simxGetObjectPosition(clientID,object_handle,-1,position,simx_opmode_buffer)==simx_return_ok)
	{
		status.x = position[0];
		status.y = position[1];
	}
	if (simxGetObjectOrientation(clientID,object_handle,-1,orient,simx_opmode_buffer)==simx_return_ok)
	{
		status.theta = orient[2];
	}
	return status;
}