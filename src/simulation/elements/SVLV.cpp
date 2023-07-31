#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_SVLV()
{
	Identifier = "DEFAULT_PT_SVLV";
	Name = "SVLV";
	Colour = PIXPACK(0x660033);
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
	Hardness = 8;

	Weight = 150;

	DefaultProperties.bio.health = 100;
	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	DefaultProperties.bio.maxHealth = 100;
	HeatConduct = 29;
	Description = "Stomach valve. Transports DIGE (digested material) from above itself to below itself.";

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
	Max_Health = 200;

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
	// Heal naturally
	Biology::DoHealing(100, UPDATE_FUNC_IN);
	// Death check
	Biology::HandleDeath(UPDATE_FUNC_IN);

    if (RNG::Ref().chance(1, 100)){
        if (parts[i].bio.glucose > 2 && parts[i].bio.o2 > 5 && parts[i].bio.health > 75) {

            int r = pmap[y-1][x];
            int aboveType = TYP(r);

            int r2 = pmap[y+1][x];

            if (aboveType == PT_DIGE && (!r2)){
                //sim->delete_part(x, y-1);
                //sim->create_part(-1, x, y+1, PT_DIGE);
                sim->do_move(ID(r), x, y-1, x, y+1);
            }
        }
    }

	return 0;
}


static int graphics(GRAPHICS_FUNC_ARGS)
{
    // Oxygen
    int o = cpart->bio.o2;

    // C02
    int c = cpart->bio.co2;

	*colr = (int)fmin(150, fmax(3 * o, 100));
	*colg = 0;
	*colb = (int)fmin(65, fmax(1.5 * o, 30));
	*pixel_mode |= PMODE_BLUR;

	// Life mix
	*colr = int(*colr * (cpart->bio.health) / 160.0f);
	*colg = int(*colg * (cpart->bio.health) / 160.0f);
	*colb = int(*colb * (cpart->bio.health) / 160.0f);

	return 0;
}
