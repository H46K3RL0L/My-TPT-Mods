#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_LUNG()
{
	Identifier = "DEFAULT_PT_LUNG";
	Name = "LUNG";
	Colour = PIXPACK(0x990066);
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
	Description = "Lung. Peforms gas exchange with Blood (BLD).";

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

	Max_O2 = 500;
	Max_CO2 = 500;
	Max_Health = 150;

	DefaultProperties.bio.o2 = Max_O2;
	DefaultProperties.bio.co2 = 0;
	DefaultProperties.bio.health = Max_Health;
}

static int update(UPDATE_FUNC_ARGS)
{
    // O2 use by lung itself
	Biology::UseO2(200, UPDATE_FUNC_IN);

	int r, rx, ry;

    rx =  RNG::Ref().between(-2, 2);
    ry =  RNG::Ref().between(-2, 2);

    if (BOUNDS_CHECK && (rx || ry))
    {
        r = pmap[y+ry][x+rx];
		int ir = ID(r);

        if (r) {

			int targetMaxO2 = sim->elements[parts[i].type].Max_O2;

			// Oxygen collection (more efficient than BLD)
			if (parts[i].bio.o2 < targetMaxO2 && TYP(r) == PT_O2){
				parts[i].bio.o2 += 50;

				// Replace with CO2 (if present)
				if (parts[i].bio.co2 > 0){
					sim->part_change_type(ID(r), x, y, PT_CO2);
					parts[i].bio.co2 -= 50;
				}
				else{
					sim->part_change_type(ID(r), x, y, PT_NONE);
				}
			}
			// Blood interactions
			else if (TYP(r) == PT_BLD || parts[ir].ctype == PT_BLD){
				// Give oxygen
				if (parts[i].bio.o2 > 0 && parts[i].bio.o2 > parts[ir].bio.o2){
					parts[ir].bio.o2 += 5;
					parts[i].bio.o2 -= 5;
				}

				// Take CO2
				if (parts[ir].bio.co2 > 0){
					parts[i].bio.co2 += 5;
					parts[ir].bio.co2 -= 5;
				}
			}
		}
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

	return 0;
}

static int graphics(GRAPHICS_FUNC_ARGS)
{
    // Oxygen
    int o = cpart->bio.o2;

    // C02
    int c = cpart->bio.co2;
	
	*colr = (int)fmax(7 * o, 100);
	*colg = 0;
	*colb = (int)fmax(7 * o, 60);
	*pixel_mode |= PMODE_BLUR;

	*colr = int(*colr * (cpart->bio.health) / 100.0f);
	*colg = int(*colg * (cpart->bio.health) / 100.0f);
	*colb = int(*colb * (cpart->bio.health) / 100.0f);

	return 0;
}
