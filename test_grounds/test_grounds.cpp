#include <iostream>
#include <boost/units/systems/si/volume.hpp>
#include <boost/units/systems/si/mass.hpp>
#include "string"

//for Storage_tank.cpp
#include <boost/units/systems/si/io.hpp> //Storage_tank.cpp and UnitTests.cpp
#include <boost/units/systems/si/prefixes.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <cmath>
#include <boost/units/cmath.hpp>

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


	
	if (tankType == "v")
	{
		if (contentLevel_raw >= tankHeight_raw)
		{
			return (tankVolume);
		}
		contentVolume = (3.1415 * tankRadius * tankRadius * contentLevel);
		return contentVolume; //m^3
	}
	else if (tankType == "h")
	{
		if (contentLevel_raw >= tankDiameter_raw)
		{
			return (tankVolume);
		}

		quantity<plane_angle> sectorAngle(
			2.0 * boost::units::acos((tankRadius - contentLevel) / tankRadius));

		quantity<area> sectorSquare(
			(sectorAngle.value() * tankRadius * tankRadius) / 2.0); //mm sqaured

		quantity<length> triangleBase(
			2.0*boost::units::sqrt(
				(tankRadius * tankRadius) -
				((tankRadius - contentLevel) *
					(tankRadius - contentLevel)))); //

		quantity<length> p(
			(tankRadius + tankRadius + triangleBase) / 2.0);

		quantity<area> triangleSquare(
			boost::units::sqrt(
				p *
				(p - tankRadius) *
				(p - tankRadius) *
				(p - triangleBase)));

		contentVolume = (sectorSquare - triangleSquare) * tankHeight;

 		return contentVolume; //m^3
	}
	else
	{
		throw std::exception("Invalid tank type. Only 'v' and 'h' accepted. ");
		return contentVolume;
	}
	
return contentVolume;
}

int main()
{
	ContentVolume("f", 2000, 1000, 2000);
	return 0;
}