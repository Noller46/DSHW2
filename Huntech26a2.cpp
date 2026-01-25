// You can edit anything you want in this file.
// However, you need to implement all public Huntech functions, which are provided below as a template.

#include "Huntech26a2.h"

Huntech::Huntech() = default;

Huntech::~Huntech() = default;

StatusType Huntech::add_squad(int squadId) {
    if (squadId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        Squad newSquad(squadId);
        squads.insert(newSquad,squadId);
        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    catch (const std::invalid_argument&)
    {
        return StatusType::FAILURE;
    }
    catch (...){
        return StatusType::FAILURE;
    }
}

StatusType Huntech::remove_squad(int squadId) {
    if (squadId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        auto node = squads.find(squadId);
        squads.remove(squadId);
        return StatusType::SUCCESS;
    }
    catch (...)
    {
        return StatusType::FAILURE;
    }
}

StatusType Huntech::add_hunter(int hunterId,
                               int squadId,
                               const NenAbility &nenType,
                               int aura,
                               int fightsHad)
{
    return StatusType::FAILURE;
}

output_t<int> Huntech::squad_duel(int squadId1, int squadId2) {
    return 0;
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) {
    if (hunterId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        if (!hunters.contains(hunterId))
            return StatusType::FAILURE;
        std::shared_ptr<MemberNode> node = hunters.get(hunterId);
        FindResult res = node->find();
        return output_t<int>(res.pathFights);
    }
    catch (...)
    {
        return {StatusType::FAILURE};
    }
    return 0;
}

output_t<int> Huntech::get_squad_experience(int squadId) {
    return 0;
}

output_t<int> Huntech::get_ith_collective_aura_squad(int i) {
    return 0;
}

output_t<NenAbility> Huntech::get_partial_nen_ability(int hunterId) {
    return NenAbility();
}

StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    return StatusType::FAILURE;
}
