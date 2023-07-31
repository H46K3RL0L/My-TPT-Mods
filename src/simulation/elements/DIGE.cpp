#include "simulation/ElementCommon.h"

void Element::Element_DIGE()
{
	Identifier = "DEFAULT_PT_DIGE";
	Name = "DIGE";
	Colour = PIXPACK(0x554080);
	MenuVisible = 1;
	MenuSection = SC_BIO;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.20f;
	HotAir = 0.000f	* CFDS;
	Falldown = 2;

	Flammable = 20;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	PhotonReflectWavelengths = 0x3FFFFFC0;

	Weight = 15;

	HeatConduct = 40;
	Description = "Digested material. Turned into glucose by INTE";

	Properties = TYPE_LIQUID;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;
}
