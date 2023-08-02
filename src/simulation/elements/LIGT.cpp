#include "simulation/ElementCommon.h"

void Element::Element_LIGT()
{
	Identifier = "DEFAULT_PT_LIGT";
	Name = "LIGT";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_SPECIAL;
    Enabled = 1;

	Advection = 1.0f;
	AirDrag = 0.0001f * CFDS;
	AirLoss = 0.0f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = -100.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;


	Flammable = 100;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;

	Weight = -2147483647;

	HeatConduct = 0;
	Description = "VERY light particles. Defy most laws of physics.";

	Properties = TYPE_PART;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
}
