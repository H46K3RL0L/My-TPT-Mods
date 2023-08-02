#include "ElementCommon.h"

#define CHANCE(chance) RNG::Ref().chance(1, chance)

Biology::Biology()
{
	
}

void Biology::UseO2(int chance, UPDATE_FUNC_ARGS){
	if (!CHANCE(chance)) return;

	if (parts[i].bio.o2 > 0 && parts[i].bio.glucose > 0){

		// Respiration requires glucose
		parts[i].bio.glucose -= 1;

		parts[i].bio.o2 -= 1;
		parts[i].bio.co2 += 1;
	}
}

// "That's a lot of damage!"
void Biology::AttackBio(int chance, int range, int damage, UPDATE_FUNC_ARGS){
	if (!CHANCE(chance)) return;

	int rand_x, rand_y;

	rand_x =  RNG::Ref().between(-range, range);
	rand_y =  RNG::Ref().between(-range, range);

	if (!(BOUNDS_CHECK && (rand_x || rand_y))) return;

	int pos = pmap[y + rand_y][x + rand_x];

	int target = ID(pos);
	int target_type  = parts[target].type;
	int this_type = parts[i].type;

	Element this_element = sim->elements[this_type];
	Element target_element = sim->elements[target_type];

	// Don't harm own type
	if (this_type == target_type || target_type == 0) return;

	// Epidermis and mucous repels attack
	if (target_type == PT_SKINE || target_type == PT_MUCO) return;


	if (!target) return;

	// Kill other cells for resources
	parts[target].bio.health -= damage;

	// Take O2
	if (parts[target].bio.o2 > 0 && 
		parts[i].bio.o2 < this_element.Max_O2)
	{

		parts[i].bio.o2 += damage;
		parts[target].bio.o2 -= damage;
	}

	// Take glucose
	if (parts[target].bio.glucose > 0)
	{

		parts[i].bio.glucose += damage;
		parts[target].bio.glucose -= damage;
	}

	// Give CO2
	if (parts[i].bio.co2 > 0 && 
		parts[target].bio.co2 < target_element.Max_CO2)
	{
		parts[i].bio.co2 -= damage;
		parts[target].bio.co2 += damage;
	}
}

void Biology::DoHeatDamage(int chance, double max_temp, double min_temp, UPDATE_FUNC_ARGS){
	if (!CHANCE(chance)) return;

	// Temp check
	if (parts[i].temp > max_temp){
		int damage = ((parts[i].temp - max_temp) / 10) + 1;
		parts[i].bio.health -= damage;
	}
	else if (parts[i].temp < min_temp){
		int damage = ((-(min_temp - (parts[i].temp))) / 10) + 1;
	}
}

void Biology::DoRespirationDamage(int chance, UPDATE_FUNC_ARGS){
	if (!CHANCE(chance)) return;

	int this_type = parts[i].type;
	Element this_element = sim->elements[this_type];

	// Damage check
	if (parts[i].bio.co2 > this_element.Max_CO2 || parts[i].bio.o2 < 1 || parts[i].bio.glucose < 1){
		parts[i].bio.health--;
	}
}

void Biology::DoHealing(int chance, UPDATE_FUNC_ARGS){
	if (!CHANCE(chance)) return;

	int this_type = parts[i].type;
	Element this_element = sim->elements[this_type];

	if (parts[i].bio.health < this_element.Max_Health && 
		parts[i].bio.o2 > this_element.Max_O2 / 2){
			parts[i].bio.health++;
			parts[i].bio.o2--;
	}
}

void Biology::HandleDeath(UPDATE_FUNC_ARGS){
	if (parts[i].bio.health < 1){
		sim->part_change_type(i, x, y, PT_DT);
	}
}

void Biology::GrowInRange(int chance, int range, int grow_on, UPDATE_FUNC_ARGS){
	if (!CHANCE(chance)) return;

	int this_type = parts[i].type;
	Element this_element = sim->elements[this_type];

	// Require over half health
	if (!(parts[i].bio.health > (this_element.Max_Health / 2))) return;

	// Require over half of all resources
	if (!(parts[i].bio.o2 > (this_element.Max_O2 / 2))) return;
	// Require over 50 glucose
	if (!(parts[i].bio.glucose > 50)) return;

	int rand_x, rand_y;

	rand_x =  RNG::Ref().between(-range, range);
	rand_y =  RNG::Ref().between(-range, range);

	if (!(BOUNDS_CHECK && (rand_x || rand_y))) return;

	int pos = pmap[y + rand_y][x + rand_x];
	int target = ID(pos);
	int target_type  = parts[target].type;

	// Grow into empty
	if (grow_on == PT_NONE && (!pos)){
		sim->create_part(-1, x + rand_x, y + rand_y, this_type);
		parts[target].bio.o2 = parts[i].bio.o2 / 2;
		parts[i].bio.o2 /= 2;
		parts[target].bio.glucose = parts[i].bio.glucose / 2;
		parts[i].bio.glucose /= 2;
		parts[target].bio.health = this_element.Max_Health;
		parts[target].bio.co2 = 0;
	}
	// Grow onto other elements
	else if (grow_on == target_type){
		sim->part_change_type(target, x + rand_x, y + rand_y, this_type);
		parts[target].bio.o2 = parts[i].bio.o2 / 2;
		parts[i].bio.o2 /= 2;
		parts[target].bio.glucose = parts[i].bio.glucose / 2;
		parts[i].bio.glucose /= 2;
		parts[target].bio.health = this_element.Max_Health;
		parts[target].bio.co2 = 0;
	}
}

void Biology::DiffuseResources(int chance, int range, UPDATE_FUNC_ARGS){
	if (!CHANCE(chance)) return;

	int rand_x, rand_y;

	rand_x =  RNG::Ref().between(-range, range);
	rand_y =  RNG::Ref().between(-range, range);

	if (!(BOUNDS_CHECK && (rand_x || rand_y))) return;

	int pos = pmap[y + rand_y][x + rand_x];

	int target = ID(pos);
	int target_type  = parts[target].type;

	if (!target) return;

	Element target_element = sim->elements[target_type];

	// Diffuse among bio
	if (sim->elements[target_type].Properties & TYPE_BIO && // Must be bio
		!(sim->elements[target_type].Properties & TYPE_DISEASE)) // Must not be disease) 
	{

		// Only blood should give O2 to blood - tissue should not hand it back
		if (parts[i].type == PT_BLD || target_type != PT_BLD ){
			if (parts[i].bio.o2 > parts[target].bio.o2){

				int amount = (parts[i].bio.o2 - parts[target].bio.o2) / 2;

				parts[i].bio.o2 -= amount;
				parts[target].bio.o2 += amount;
			}
		}

		if (parts[i].bio.glucose > parts[target].bio.glucose){

			int amount = (parts[i].bio.glucose - parts[target].bio.glucose) / 2;

			parts[i].bio.glucose -= amount;
			parts[target].bio.glucose += amount;
		}

		if (parts[i].bio.co2 > parts[target].bio.co2){

			int amount = (parts[i].bio.co2 - parts[target].bio.co2) / 2;

			parts[i].bio.co2 -= amount;
			parts[target].bio.co2 += amount;
		}
	}
}

void Biology::DoRadiationDamage(int chance, int range, UPDATE_FUNC_ARGS){
	if (!CHANCE(chance)) return;

	int rand_x, rand_y;

	rand_x = RNG::Ref().between(-range, range);
	rand_y = RNG::Ref().between(-range, range);

	if (!(BOUNDS_CHECK && (rand_x || rand_y))) return;

	int pos = pmap[y + rand_y][x + rand_x];
	int nuc_pos = sim->photons[y][x];

	int target = ID(pos);
	int target_type  = parts[target].type;

	int nuc = ID(nuc_pos);
	int nuc_type  = parts[nuc].type;

	if (target_type){
		Element target_element = sim->elements[target_type];
		Element nuc_element = sim->elements[nuc_type];

		if (target_element.MenuSection == SC_NUCLEAR ||
			nuc_element.MenuSection == SC_NUCLEAR){

			parts[i].bio.radDamage++;
			parts[i].bio.health--;
			if (RNG::Ref().chance(parts[i].bio.radDamage, 100000)){
				sim->part_change_type(i, x, y, PT_TUMOR);
			}
		}
	}
}

bool Biology::TryCollect(int chance, int range, int type, UPDATE_FUNC_ARGS){
	if (!CHANCE(chance)) return false;

	int rand_x, rand_y;

	rand_x =  RNG::Ref().between(-range, range);
	rand_y =  RNG::Ref().between(-range, range);

	if (!(BOUNDS_CHECK && (rand_x || rand_y))) return false;

	int pos = pmap[y + rand_y][x + rand_x];
	int target = ID(pos);
	int target_type  = parts[target].type;

	if (target_type == type){
		sim->delete_part(x + rand_x, y + rand_y);
		return true;
	}

	return false;
}

void Biology::StealResources(int chance, int range, UPDATE_FUNC_ARGS){
	if (!CHANCE(chance)) return;

	int rand_x, rand_y;

	rand_x = RNG::Ref().between(-range, range);
	rand_y = RNG::Ref().between(-range, range);

	if (!(BOUNDS_CHECK && (rand_x || rand_y))) return;

	int pos = pmap[y + rand_y][x + rand_x];
	int target = ID(pos);
	int target_type = parts[target].type;

	// steal o2 from bio, offload co2 to bio (tumor is greedy)
	if (sim->elements[target_type].Properties & TYPE_BIO){
		if (parts[target].bio.o2 > 1){
			parts[i].bio.o2++;
			parts[target].bio.o2--;
		}
		if (parts[target].bio.glucose > 1){
			parts[i].bio.glucose++;
			parts[target].bio.glucose--;
		}
		if (parts[i].bio.co2 > 0){
			parts[i].bio.co2--;
			parts[target].bio.co2++;
		}
	}
}

// Time for revenge, evil bacterium!
bool Biology::AttackDisease(int chance, int range, int damage, UPDATE_FUNC_ARGS){
	if (!CHANCE(chance)) return false;

	int rand_x, rand_y;

	rand_x =  RNG::Ref().between(-range, range);
	rand_y =  RNG::Ref().between(-range, range);

	if (!(BOUNDS_CHECK && (rand_x || rand_y))) return false;

	int pos = pmap[y + rand_y][x + rand_x];
	int target = ID(pos);
	int target_type  = parts[target].type;

	int this_type = parts[i].type;

	Element this_element = sim->elements[this_type];
	Element target_element = sim->elements[target_type];

	// Only hurt disease
	if (!(target_element.Properties & TYPE_DISEASE)) return false;

	if (!target) return false;

	// Kill disease
	parts[target].bio.health -= damage;

	// Take resources from disease (because why not?)
	if (parts[target].bio.o2 > 0 && 
		parts[i].bio.o2 < this_element.Max_O2)
	{

		parts[i].bio.o2 += damage;
		parts[target].bio.o2 -= damage;
	}

	return true;
}
