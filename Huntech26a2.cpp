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
        squads.insert(newSquad,TreeKey(squadId,squadId));
        auraTree.insert(newSquad,TreeKey(0,squadId));
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
        Squad& s = squads.find(TreeKey(squadId,squadId));
        int currentAura = 0;
        if (!s.isEmpty())
            currentAura = s.representative->squad_sum_aura;
        auraTree.remove(TreeKey(currentAura,squadId));
        squads.remove(TreeKey(squadId,squadId));
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
    Hunter me = Hunter(hunterId, nenType, aura, fightsHad);
    Squad* my_squad = nullptr;
    TreeKey my_key;
    MemberNode* my_node = new MemberNode(me);

    try {
        my_squad = &squads.find(TreeKey(squadId, squadId));
        my_key = TreeKey(my_squad->getTotalAura(), squadId);
        hunters.put(make_shared<MemberNode>(*my_node),hunterId);
    } catch (const std::invalid_argument&) {
        return StatusType::FAILURE;
    } catch (...){
        return StatusType::ALLOCATION_ERROR;
    }
    if (hunterId <= 0 || squadId <= 0 || fightsHad < 0 || aura < 0) {
        return StatusType::INVALID_INPUT;
    }


    if (my_squad->representative == nullptr) {
        my_squad->representative = my_node;
        my_node->squad_sum_aura = aura;
    }
    else {
        MemberNode* parent = my_squad->representative;
        my_node->parent = parent;

        parent->size += my_node->size;
        parent->squad_sum_aura += aura;

        my_node->squad_fights_cnt = fightsHad-(parent->squad_fights_cnt);
        my_node->r_nen += parent->squad_total_nen-parent->hunter.getNenAbility();
        parent->squad_total_nen += my_node->squad_total_nen;
    }

    auraTree.remove(my_key);
    auraTree.insert(*my_squad, TreeKey(my_squad->getTotalAura(), squadId));
    return StatusType::SUCCESS;
}

output_t<int> Huntech::squad_duel(int squadId1, int squadId2) {
    if (squadId1 <= 0 || squadId2 <= 0 || squadId1 == squadId2) {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    try
    {
        Squad& s1 = squads.find(TreeKey(squadId1,squadId1));
        Squad& s2 = squads.find(TreeKey(squadId2,squadId2));
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
    if (squadId <= 0)
        return output_t<int>(StatusType::INVALID_INPUT);
    try
    {
        Squad& s = squads.find(TreeKey(squadId,squadId));
        return output_t<int>(s.experience);
    }
    catch (...)
    {
        return output_t<int>(StatusType::FAILURE);
    }
}

output_t<int> Huntech::get_ith_collective_aura_squad(int i) {
    /*if (i <= 0)
        return output_t<int>(StatusType::FAILURE);
    try
    {
        if (i > auraTree.getSize())
            return output_t<int>(StatusType::FAILURE);
        Squad foundSquad= auraTree.select(i);
        return output_t<int>(foundSquad.getId());
    }
    catch (...)
    {
        return output_t<int>(StatusType::FAILURE);
    }*/
    return StatusType::FAILURE;
}

output_t<NenAbility> Huntech::get_partial_nen_ability(int hunterId) {
    if (hunterId <= 0)
        return output_t<NenAbility>(StatusType::INVALID_INPUT);
    try
    {
        if (!hunters.contains(hunterId))
            return output_t<NenAbility>(StatusType::FAILURE);
        std::shared_ptr<MemberNode> node = hunters.get(hunterId);
        FindResult res = node->find();
        return output_t<NenAbility>(res.pathSum);
    }
    catch (...)
    {
        return output_t<NenAbility>(StatusType::FAILURE);
    }
}

StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    if (forcingSquadId <= 0 || forcedSquadId <= 0) {
        return StatusType::INVALID_INPUT;
    }

    Squad squad_a;
    Squad squad_b;
    try{
        squad_a = squads.find(TreeKey(forcingSquadId,forcingSquadId));
        squad_b = squads.find(TreeKey(forcedSquadId, forcedSquadId));
    } catch (...) {
        return StatusType::FAILURE;
    }
    MemberNode* root_a = squad_a.representative;
    MemberNode* root_b = squad_b.representative;

    TreeKey key_a = TreeKey(squad_a.getTotalAura(), forcingSquadId);
    //TreeKey key_b = TreeKey(squad_b.getTotalAura(), forcedSquadId);

    if (!squad_a.isEmpty() && (squad_b.isEmpty() || squad_a.getBattleValue() > squad_b.getBattleValue())) {
        if (root_a->size>=root_b->size) {
            root_b->parent = root_a;
            root_a->size += root_b->size;
            root_a->squad_sum_aura += root_b->squad_sum_aura;

            root_b->r_nen += root_a->squad_total_nen - root_b->r_nen;

            root_a->squad_total_nen += root_b->squad_total_nen;
        } else {
            root_a->parent = root_b;
            root_b->size += root_b->size;
            root_b->squad_sum_aura += root_b->squad_sum_aura;

            root_b->r_nen += root_a->squad_total_nen;
            root_a->r_nen -= root_b->r_nen;

            root_b->squad_total_nen += root_a->squad_total_nen;

            squad_a.representative = root_b;
        }

        remove_squad(forcedSquadId);
        auraTree.remove(key_a);
        auraTree.insert(squad_a, TreeKey(squad_a.getTotalAura(), forcingSquadId));

        cout << "" << endl;

        return StatusType::SUCCESS;
    }
    return StatusType::FAILURE;
}

