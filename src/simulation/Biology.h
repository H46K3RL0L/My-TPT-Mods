#include "Particle.h"

class Biology
{
    public:
        Biology();
        static void UseO2(int chance, UPDATE_FUNC_ARGS);
        static void AttackBio(int chance, int range, int damage, UPDATE_FUNC_ARGS);
        static void DoHeatDamage(int chance, double max_temp, double min_temp, UPDATE_FUNC_ARGS);
        static void DoRespirationDamage(int chance, UPDATE_FUNC_ARGS);
        static void DoRadiationDamage(int chance, int range, UPDATE_FUNC_ARGS);
        static void DoHealing(int chance, UPDATE_FUNC_ARGS);
        static void HandleDeath(UPDATE_FUNC_ARGS);
        static void GrowInRange(int chance, int range, int grow_on, UPDATE_FUNC_ARGS);
        static void DiffuseResources(int chance, int range, UPDATE_FUNC_ARGS);
        static bool TryCollect(int chance, int range, int type, UPDATE_FUNC_ARGS);
        static void StealResources(int chance, int range, UPDATE_FUNC_ARGS);
        static bool AttackDisease(int chance, int range, int damage, UPDATE_FUNC_ARGS);
};