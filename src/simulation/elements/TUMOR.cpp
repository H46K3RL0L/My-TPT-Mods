#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_TUMOR()
{
	Identifier = "DEFAULT_PT_TUMOR";
	Name = "TUMR";
	Colour = PIXPACK(0x554040);
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

	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	HeatConduct = 29;
	Description = "Tumor. Certain death/blobification for biology";

	Properties = TYPE_SOLID|PROP_NEUTPENETRATE|TYPE_BIO|TYPE_DISEASE;

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
	Max_CO2 =  100;

	DefaultProperties.bio.health = 250; // In real life, cancer survives better than regular tissue (citation: HeLa)
	DefaultProperties.bio.o2 = Max_O2;
	DefaultProperties.bio.co2 = 0;
}

static int update(UPDATE_FUNC_ARGS)
{
	// O2 use itself (Increased due to increased biological activity)
    Biology::UseO2(100, UPDATE_FUNC_IN);
	// Steal resources from biological material
	Biology::StealResources(2, 2, UPDATE_FUNC_IN);

	int r, rx, ry;

    rx =  RNG::Ref().between(-2, 2);
    ry =  RNG::Ref().between(-2, 2);
    
    if (RNG::Ref().chance(1, 2000) && (BOUNDS_CHECK && (rx || ry)))
    {
        r = pmap[y+ry][x+rx];
		int target = ID(r);
		int target_type = parts[target].type;

        if (r) {
			// Metastasize
			if (target_type == PT_BLD){
				parts[target].ctype = PT_TUMOR;
			}	
        }
    }
	if (RNG::Ref().chance(1, 100)){

		r = pmap[y+ry][x+rx];
		int target = ID(r);
		int target_type = parts[target].type;

		// convert biology to tumor (grow)
		if (sim->elements[target_type].Properties & TYPE_BIO && target_type != PT_TUMOR){
			
			sim->part_change_type(target, parts[target].x, parts[target].y, PT_TUMOR);
			parts[i].bio.o2--;
			parts[i].bio.co2++;
		}
	}

	// Health related things
	Biology::DoHeatDamage(50, 323.15, 273, UPDATE_FUNC_IN);
	Biology::DoRespirationDamage(50, UPDATE_FUNC_IN);
	Biology::DoHealing(50, UPDATE_FUNC_IN);
	Biology::HandleDeath(UPDATE_FUNC_IN);

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
    // Oxygen
    int o = cpart->bio.o2;

    // C02
    int c = cpart->bio.co2;

	*colr = (int)fmax(3 * o, 77);
	*colg = (int)fmax(3 * o, 62);
	*colb = (int)fmax(3 * o, 62);
	*pixel_mode |= PMODE_BLUR;

	// Life mix
	*colr = int(*colr * (cpart->bio.health) / 100.0f);
	*colg = int(*colg * (cpart->bio.health) / 100.0f);
	*colb = int(*colb * (cpart->bio.health) / 100.0f);

	return 0;
}
