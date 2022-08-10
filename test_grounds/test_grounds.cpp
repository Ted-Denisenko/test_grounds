#include <iostream>
#include <boost/units/systems/si/volume.hpp>
#include <boost/units/systems/si/mass.hpp>
#include "string"

//for Storage_tank.cpp
#include <boost/units/systems/si/io.hpp> //Storage_tank.cpp and UnitTests.cpp
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <cmath>

using Volume = boost::units::quantity<boost::units::si::volume>;
using Square = boost::units::quantity<boost::units::area_dimension>;

using namespace boost::units;
using namespace boost::units::si;


Volume ContentVolume(std::string tankType, double contentLevel_raw, double tankHeight_raw, double tankDiameter_raw)
{
	quantity<length> contentLevel(contentLevel_raw * milli * meters);
	quantity<length> tankHeight(tankHeight_raw * milli * meters);
	quantity<length> tankDiameter(tankDiameter_raw * milli * meters);
	quantity<length> tankRadius = tankDiameter / 2.0;
	Volume tankVolume(3.1415 * (tankRadius * tankRadius) * tankHeight);
	Volume contentVolume;


	if (contentLevel_raw > tankHeight_raw)
	{
		return (tankVolume);
	}
	else
	{
		if (tankType == "v")
		{
			if (contentLevel_raw > tankHeight_raw)
			{
				return (tankVolume);
			}
			contentVolume = (3.1415 * (tankRadius * tankRadius) * contentLevel);
			return contentVolume; //m^3
		}
		else if (tankType == "h")
		{
			if (contentLevel_raw > (tankDiameter_raw/2.0))
			{
				return (tankVolume);
			}

			double x = ((tankRadius.value() - tankHeight.value()) / tankRadius.value());
			double y = acos(x);
			y = 2 * y;
			quantity
				<unit<plane_angle_dimension, degree::system>
				> sectorAngle((2.0 * acos(x))
					* radian);

			quantity<area> sectorSquare(
				(sectorAngle.value() * (tankRadius.value() * tankRadius.value()) / 2.0)
				* milli * milli * square_meter); //mm sqaured

			quantity<length> triangleBase(
				sqrt(
					(tankRadius.value() * tankRadius.value()) -
					((tankRadius.value() - tankHeight.value()) *
						(tankRadius.value() - tankHeight.value())))
				* milli * meters);

			quantity<length> p(
				(tankRadius + tankRadius + triangleBase) / 2.0);

			quantity<area> triangleSquare(
				sqrt(
					p.value() *
					(p.value() - tankRadius.value()) *
					(p.value() - tankRadius.value()) *
					(p.value() - triangleBase.value()))
				* milli * milli * square_meter );
			contentVolume = (sectorSquare - triangleSquare) * tankHeight;

 			return contentVolume; //m^3
		}
	}
	return contentVolume;
}

int main()
{
	std::string tankType = "h";
	ContentVolume("h", 10.00, 10.00, 200.00);
	return 0;
}