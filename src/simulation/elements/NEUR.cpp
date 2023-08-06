#include "simulation/ElementCommon.h"

static int update(UPDATE_FUNC_ARGS);
static int graphics(GRAPHICS_FUNC_ARGS);

void Element::Element_NEUR()
{
	Identifier = "DEFAULT_PT_NEUR";
	Name = "NEUR";
	Colour = PIXPACK(0xCF95B8);
	MenuVisible = 1;
	MenuSection = SC_BIO;
	Enabled = 1;

	Advection = 0.0f;
	AirDrag = 0.00f * CFDS;
	AirLoss = 1.00f;
	Loss = 0.00f;
	Collision = 0.0f;
	Gravity = 0.0f;
	Diffusion = 0.00f;
	HotAir = 0.000f	* CFDS;
	Falldown = 0;

	Flammable = 0;
	Explosive = 0;
	Meltable = 0;
	Hardness = 1;

	Weight = 100;

	HeatConduct = 10;
	Description = "Neurons. Randomly emit electrical pulses when alive, relative to their health.";

    // Bio stuff

	Max_O2 = 100;
	Max_CO2 = 100;

	// Neurons are weak
	Max_Health = 50;

	DefaultProperties.bio.o2 = Max_O2;
	DefaultProperties.bio.co2 = 0;
	DefaultProperties.bio.health = Max_Health;

	Properties = TYPE_SOLID|TYPE_BIO;

	LowPressure = IPL;
	LowPressureTransition = NT;
	HighPressure = 7.0f;
	HighPressureTransition = PT_NONE;
	LowTemperature = ITL;
	LowTemperatureTransition = NT;
	HighTemperature = ITH;
	HighTemperatureTransition = NT;

	Update = &update;
    Graphics = &graphics;
}

static int update(UPDATE_FUNC_ARGS)
{

    int r, nnx, nny, rx, ry;

	// NOTE: Neurons are fast to die and slow to heal

    // O2 use itself
    Biology::UseO2(150, UPDATE_FUNC_IN);
    // Diffuse resources
	Biology::DiffuseResources(2, 2, UPDATE_FUNC_IN);
    // Radiation damage
	Biology::DoRadiationDamage(2, 2, UPDATE_FUNC_IN);
	// Damage from extreme heat or cold
	Biology::DoHeatDamage(5, 323.15, 273, UPDATE_FUNC_IN);
	// Damage from lack of O2 or too much CO2
	Biology::DoRespirationDamage(50, UPDATE_FUNC_IN);
	// Heal naturally
	Biology::DoHealing(200, UPDATE_FUNC_IN);
	// Death check
	Biology::HandleDeath(UPDATE_FUNC_IN);

    // Emit signals
    if (RNG::Ref().chance(parts[i].bio.health, 5000)){
        for (rx=-1; rx<2; rx++)
            for (ry=-1; ry<2; ry++)
                if (BOUNDS_CHECK && (rx || ry) && abs(rx)+abs(ry)<4)
                {
                    r = pmap[y+ry][x+rx];
                    if (!r)
                        continue;
                    int rt = TYP(r);
                    if (sim->parts_avg(i,ID(r),PT_INSL) != PT_INSL)
                    {
                        if ((sim->elements[rt].Properties&PROP_CONDUCTS) && parts[ID(r)].life==0)
                        {
                            parts[ID(r)].life = 4;
                            parts[ID(r)].ctype = rt;
                            sim->part_change_type(ID(r),x+rx,y+ry,PT_SPRK);
                        }
                    }
                }
    }

	return 0;
}


static int graphics(GRAPHICS_FUNC_ARGS)
{
	// Life mix
	*colr = int(*colr * (cpart->bio.health) / 100.0f);
	*colg = int(*colg * (cpart->bio.health) / 100.0f);
	*colb = int(*colb * (cpart->bio.health) / 100.0f);

	return 0;
}