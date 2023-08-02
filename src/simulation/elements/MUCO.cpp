#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_MUCO()
{
	Identifier = "DEFAULT_PT_MUCO";
	Name = "MUCO";
	Colour = PIXPACK(0x666611);
	MenuVisible = 1;
	MenuSection = SC_BIO;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 0.97f;
	Loss = 0.50f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 12;

	Weight = 150;

	DefaultProperties.bio.health = 100;
	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	DefaultProperties.bio.maxHealth = 100;
	HeatConduct = 29;
	Description = "Mucuous membrane. Protects against stomach acid (and bacteria!)";

	Properties = TYPE_SOLID|PROP_NEUTPENETRATE|TYPE_BIO;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = IPH;
	HighPressureTransition = NT;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
	Graphics = &graphics;

	// Bio stuff

	Max_O2 = 100;
	Max_CO2 = 100;
	Max_Health = 400;

	DefaultProperties.bio.o2 = Max_O2;
	DefaultProperties.bio.co2 = 0;
	DefaultProperties.bio.health = Max_Health;
}

static int update(UPDATE_FUNC_ARGS)
{
	// O2 use by cells
	Biology::UseO2(100, UPDATE_FUNC_IN);
	// Diffuse resources
	Biology::DiffuseResources(2, 2, UPDATE_FUNC_IN);
	// Radiation damage
	Biology::DoRadiationDamage(2, 2, UPDATE_FUNC_IN);
	// Damage from extreme heat or cold
	Biology::DoHeatDamage(5, 323.15, 273, UPDATE_FUNC_IN);
	// Damage from lack of O2 or too much CO2
	Biology::DoRespirationDamage(100, UPDATE_FUNC_IN);
    // Hurt diseases
    Biology::AttackDisease(5, 1, 10, UPDATE_FUNC_IN);
	// Heal naturally
	Biology::DoHealing(100, UPDATE_FUNC_IN);
	// Death check
	Biology::HandleDeath(UPDATE_FUNC_IN);

	return 0;
}


static int graphics(GRAPHICS_FUNC_ARGS)
{
	*pixel_mode |= PMODE_BLUR;

	// Life mix
	*colr *= (int)fmax((cpart->bio.health) / 200.0f, 1);
	*colg *= (int)fmax((cpart->bio.health) / 200.0f, 1);
	*colb *= (int)fmax((cpart->bio.health) / 200.0f, 1);

	return 0;
}
