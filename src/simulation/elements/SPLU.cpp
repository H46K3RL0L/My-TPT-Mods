#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);

void Element::Element_SPLU()
{
	Identifier = "DEFAULT_PT_SPLU";
	Name = "SPLU";
	Colour = PIXPACK(0x407020);
	MenuVisible = 1;
	MenuSection = SC_NUCLEAR;
	Enabled = 1;

	Advection = 0.4f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.99f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.4f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 1;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 0;
	PhotonReflectWavelengths = 0x001FCE00;

	Weight = 90;

	DefaultProperties.temp = R_TEMP + 4.0f + 273.15f;
	HeatConduct = 255;
	Description = "Super plutonium, just explodes.";

	Properties = TYPE_PART|PROP_NEUTPASS|PROP_RADIOACTIVE;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
}

static int update(UPDATE_FUNC_ARGS)
{
		sim->create_part(i, x + 1, y + 1, PT_NEUT);
        sim->create_part(i, x - 1, y - 1, PT_NEUT);
        sim->create_part(i, x + 1, y - 1, PT_NEUT);
        sim->create_part(i, x - 1, y + 1, PT_NEUT);
        sim->create_part(i, x + 1, y, PT_NEUT);
        sim->create_part(i, x - 1, y, PT_NEUT);
        sim->create_part(i, x, y - 1, PT_NEUT);
        sim->create_part(i, x, y + 1, PT_NEUT);
	return 0;
}
