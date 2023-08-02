#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_WBLD()
{
	Identifier = "DEFAULT_PT_WBLD";
	Name = "WBLD";
	Colour = PIXPACK(0xFFFFFF);
	MenuVisible = 1;
	MenuSection = SC_BIO;
	Enabled = 1;

	Advection = 0.6f;
	AirDrag = 0.01f * CFDS;
	AirLoss = 0.98f;
	Loss = 0.95f;
	Collision = 0.0f;
	Gravity = 0.1f;
	Diffusion = 0.10f;
	HotAir = 0.000f  * CFDS;
	Falldown = 2;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 20;

	Weight = 10;
	DefaultProperties.bio.health = 200;
	DefaultProperties.temp = R_TEMP - 2.0f + 273.15f;
	HeatConduct = 29;
	Description = "White blood cells. Fight infection, but use a lot of resources. Multiply if infection is detected.";

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


	// O2 use
	Biology::UseO2(100, UPDATE_FUNC_IN);

	int r, rx, ry;

	int iMaxO2 = sim->elements[parts[i].type].Max_O2;

	// Fight disease
	if (Biology::AttackDisease(1, 2, 50, UPDATE_FUNC_IN)){
		// If there was disease, multiply 
		if (RNG::Ref().chance(1, 100)){
			rx =  RNG::Ref().between(-2, 2);
			ry =  RNG::Ref().between(-2, 2);

			r = pmap[y+ry][x+rx];

			int t = TYP(r);
			int ir = ID(r);

			if (parts[i].bio.o2 >= iMaxO2){
				// Can replace nothing or blood
				if (t == 0){
					sim->create_part(ir, x, y, PT_WBLD);
					parts[ir].bio.o2 = iMaxO2 / 2;
					parts[i].bio.o2 = iMaxO2 / 2;
					parts[ir].bio.health = 200;
				}
				else if (t == PT_BLD){
					sim->part_change_type(ir, x, y, PT_WBLD);
					parts[ir].bio.o2 = iMaxO2 / 2;
					parts[i].bio.o2 = iMaxO2 / 2;
					parts[ir].bio.health = 200;
				}
			}
		}
	}

    rx =  RNG::Ref().between(-4, 4);
    ry =  RNG::Ref().between(-4, 4);
    
    if (BOUNDS_CHECK && (rx || ry))
    {
        r = pmap[y+ry][x+rx];

		int t = TYP(r);
		int ir = ID(r);

        if (r){
			// Take resources from blood
			if (t  == PT_BLD){
				// Take oxygen
				if (parts[ir].bio.o2 > 0 && parts[i].bio.o2 < iMaxO2 * 2){
					parts[i].bio.o2++;
					parts[ir].bio.o2--;
				}
				// Give co2
				if (parts[i].bio.co2 > 0){
					parts[i].bio.co2--;
					parts[ir].bio.co2++;
				}
				
			}
		}
    }

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
    // Health
    int l = cpart->bio.health;

	*colr = (int)fmax(9 * l, 75);
	*colg = (int)fmax(9 * l, 75);
	*colb = (int)fmax(9 * l, 75);
	*pixel_mode |= PMODE_BLUR;

	//(l / 200) * 255;

	//*colr = int(*colr * (cpart->bio.health) / 100.0f);
	//*colg = int(*colg * (cpart->bio.health) / 100.0f);
	//*colb = int(*colb * (cpart->bio.health) / 100.0f);

	return 0;
}
