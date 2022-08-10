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

//======================================

namespace extended_area_names {
	namespace squared_millimeters_system
	{
		// "area" is not a base unit
		typedef make_scaled_unit<area, scale<10, static_rational<-6>>>::type millimeter_unit;
		typedef make_system<millimeter_unit>::type system;
		typedef unit<area_dimension, system> area;

		BOOST_UNITS_STATIC_CONSTANT(squared_millimeter, area);
		BOOST_UNITS_STATIC_CONSTANT(squared_millimeters, area);
	} // namespace squared_millimeters_system

	typedef quantity<squared_millimeters_system::area> quantity_area_square_millimeter;
	using squared_millimeters_system::squared_millimeter;
	using squared_millimeters_system::squared_millimeters;
} // namespace extended_area_names

typedef make_scaled_unit<area, scale<10, static_rational<-6>>>::type millimeter_unit;

//======================================

Volume ContentVolume(std::string tankType, double contentLevel_raw, double tankHeight_raw, double tankDiameter_raw)
{
	quantity<length> contentLevel(contentLevel_raw * milli * meters);
	quantity<length> tankHeight(tankHeight_raw * milli * meters);
	quantity<length> tankDiameter(tankDiameter_raw * milli * meters);
	quantity<length> tankRadius = tankDiameter / 2.0;
	Volume tankVolume(3.1415 * (tankRadius * tankRadius) * tankHeight);
	Volume contentVolume;

	//typedef quantity<length> length;
	//static const auto millimeters = milli * meters;

	//auto x = length(5 * millimeters);
	//auto mm = double(x / meters * 1000.0);

	//typedef quantity<area> area;
	//static const auto square_millimeters = square_meter / 1000000;
	//auto ploshad = area(5 * square_millimeters);

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
			//using namespace extended_area_names;

			quantity
				<unit<plane_angle_dimension, degree::system>
				> sectorAngle(2 * acos((tankRadius - tankHeight) / tankRadius)
					* radian);

			//auto sectorSquare(
			//	(sectorAngle.value() * (tankRadius.value() * tankRadius.value()) / 2.0)
			//	* square_meter);

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

			//quantity<millimeter_unit> triangleSquare(
			//	sqrt(
			//		p.value() *
			//		(p.value() - tankRadius.value()) *
			//		(p.value() - tankRadius.value()) *
			//		(p.value() - triangleBase.value()))
			//	* square_meter);

			quantity<area> triangleSquare(
				sqrt(
					p.value() *
					(p.value() - tankRadius.value()) *
					(p.value() - tankRadius.value()) *
					(p.value() - triangleBase.value()))
				*milli * milli * square_meter );
			contentVolume = (sectorSquare - triangleSquare) * tankHeight;

			return contentVolume; //m^3
		}
		//else
		//{
		//	todo: исключение о несоответсвии 
		//}
	}
	return contentVolume;
}

int main()
{
	std::string tankType = "h";
	ContentVolume("h", 600.00, 2000.00, 10000.00);
	return 0;
}