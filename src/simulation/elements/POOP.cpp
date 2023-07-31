#include "simulation/ElementCommon.h"

void Element::Element_POOP()
{
	Identifier = "DEFAULT_PT_POOP";
	Name = "POOP";
	Colour = PIXPACK(0x403030);
	MenuVisible = 1;
	MenuSection = SC_BIO;
	Enabled = 1;

	Advection = 0.7f;
	AirDrag = 0.02f * CFDS;
	AirLoss = 0.96f;
	Loss = 0.80f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 20;
	Explosive = 0;
	Meltable = 0;
	Hardness = 30;
	PhotonReflectWavelengths = 0x3FFFFFC0;

	Weight = 100;

	HeatConduct = 70;
	Description = "Waste material.";

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
