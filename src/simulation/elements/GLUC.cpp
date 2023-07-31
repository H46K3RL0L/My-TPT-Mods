#include "simulation/ElementCommon.h"

void Element::Element_GLUC()
{
	Identifier = "DEFAULT_PT_GLUC";
	Name = "GLUC";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_BIO;
	Enabled = 1;

	Advection = 0.4f;
	AirDrag = 0.04f * CFDS;
	AirLoss = 0.94f;
	Loss = 0.95f;
	Collision = -0.1f;
	Gravity = 0.3f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 1;
	Explosive = 0;
	Meltable = 5;
	Hardness = 1;

	Weight = 75;

	HeatConduct = 110;
	Description = "Glucose, basic energy for cells.";

	Properties = TYPE_PART|TYPE_FOOD;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = 1173.0f;
	HighTemperatureTransition = PT_LAVA;
}
