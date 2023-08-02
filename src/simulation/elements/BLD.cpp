#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_BLD()
{
	Identifier = "DEFAULT_PT_BLD";
	Name = "BLD";
	Colour = PIXPACK(0x990000);
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
	HotAir = 0.000f  * CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;

	Weight = 10;
	DefaultProperties.bio.health = 500;
	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	HeatConduct = 29;
	Description = "Blood. Absorbs oxygen and transfers it to other living pixels.";

	Properties = TYPE_LIQUID|PROP_NEUTPENETRATE|TYPE_BIO;

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
}

static int update(UPDATE_FUNC_ARGS)
{

    // O2 use by blood itself (made very slow for somewhat accuracy)
    Biology::UseO2(1000, UPDATE_FUNC_IN);
	// Try to collect O2
	if (Biology::TryCollect(2, 1, PT_O2, UPDATE_FUNC_IN)){
		parts[i].bio.o2 += 5;
	}
	// Try to collect glucose
	if (Biology::TryCollect(2, 1, PT_GLUC, UPDATE_FUNC_IN)){
		parts[i].bio.glucose += 1000;
	}
	// Diffuse resources
	Biology::DiffuseResources(1, 2, UPDATE_FUNC_IN);
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


	int rand_x =  RNG::Ref().between(-2, 2);
	int rand_y =  RNG::Ref().between(-2, 2);

	if (BOUNDS_CHECK && (rand_x || rand_y)){ 

		int pos = pmap[y + rand_y][x + rand_x];
		int target = ID(pos);
		int target_type = parts[target].type;

		//Metastasis code
		if (parts[i].ctype == PT_TUMOR){
			if (RNG::Ref().chance(1, 100)){
			// convert biology to tumor (grow)
				if (sim->elements[target_type].Properties & TYPE_BIO && target_type != PT_TUMOR){
					sim->part_change_type(target, x + rand_x, y + rand_y, PT_TUMOR);
				}
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
    int c = cpart->bio.co2 / 3;

	int q = cpart->bio.o2;
	*colr = (int)fmax(3 * o, 75);
	*colg = 0;
	*colb = 8 * c;
	*pixel_mode |= PMODE_BLUR;

	//*colr = int(*colr * (cpart->bio.health) / 100.0f);
	//*colg = int(*colg * (cpart->bio.health) / 100.0f);
	//*colb = int(*colb * (cpart->bio.health) / 100.0f);

	return 0;
}
