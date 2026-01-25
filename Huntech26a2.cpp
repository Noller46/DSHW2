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
    if (squadId1 <= 0 || squadId2 <= 0 || squadId1 == squadId2) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    try
    {
        auto node1 = squads.find(squadId1);
        auto node2 = squads.find(squadId2);
        Squad& s1 = node1->data;
        Squad& s2 = node2->data;
        if (s1.isEmpty()||s2.isEmpty())
            return output_t<int>(StatusType::FAILURE);
        int score1 = s1.representative->squad_sum_aura + s1.experience;
        int score2 = s2.representative->squad_sum_aura + s2.experience;
        int result = 0;
        if (score1 > score2)
        {
            s1.experience += 3;
            result = 1;
        }
        else if (score2 > score1){
        s2.experience += 3;
        result = 3;
        }
        else
        {
            const NenAbility& nen1 = s1.representative->squad_total_nen;
            const NenAbility& nen2 = s2.representative->squad_total_nen;

            if (nen1 > nen2)
            {
                s1.experience += 3;
                result = 2;
            }
            else if (nen2 > nen1)
            {
                s2.experience += 3;
                result = 4;
            }
            else
            {
                s1.experience++;
                s2.experience++;
            }
        }
        s1.representative->squad_fights_cnt++;
        s2.representative->squad_fights_cnt++;
        return output_t<int>(result);
    }
    catch (...)
    {
        return output_t<int>(StatusType::FAILURE);
    }
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) {
    if (hunterId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);
    try
    {
        if (!hunters.contains(hunterId))
            return output_t<int>(StatusType::FAILURE);
        std::shared_ptr<MemberNode> node = hunters.get(hunterId);
        FindResult res = node->find();
        return output_t<int>(res.pathFights);
    }
    catch (...)
    {
        return output_t<int>(StatusType::FAILURE);
    }
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
