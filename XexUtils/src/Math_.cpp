#include "pch.h"
#include "Math_.h"

namespace XexUtils
{
namespace Math
{
	double Radians(double degrees)
	{
		double pi = 3.14159265359;
		return (degrees * (pi / 180));
	}
}
}