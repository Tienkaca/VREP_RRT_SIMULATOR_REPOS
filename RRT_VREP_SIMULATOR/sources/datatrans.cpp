#include "datatrans.h"

int Vrep2Qt_dataTransform(float a)
{
	return round((a+2.5)/5*RD_AREA_SIZE);
}
float Qt2Vrep_dataTransform(int a)
{
	return ((float)a/400*5 -2.5);
}