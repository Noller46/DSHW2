// You can edit anything you want in this file.
// However, you need to implement all public Huntech functions, which are provided below as a template.

#include "Huntech26a2.h"
#include <cassert>
#include <iostream>


Huntech::Huntech() = default;

Huntech::~Huntech() = default;

StatusType Huntech::add_squad(int squadId) {
    if (squadId <= 0)
        return StatusType::INVALID_INPUT;

    Squad newSquad(squadId);

    if (squads.contains(TreeKey(squadId, squadId))) {
        return StatusType::FAILURE;
    }

    try {
        // 1. Insert into auraTree first (safe: no rollback needed if it fails)
        auraTree.insert(make_shared<Squad>(newSquad), TreeKey(0, squadId));

        // 2. Now insert into squads
        squads.insert(make_shared<Squad>(newSquad), TreeKey(squadId, squadId));

        return StatusType::SUCCESS;
    } catch (...) {
        // If squads.insert throws, auraTree already contains the squad.
        // We must roll it back.
        try {
            auraTree.remove(TreeKey(0, squadId));
        } catch (...) {
            // If rollback fails, we still return FAILURE.
        }
        return StatusType::FAILURE;
    }
}


StatusType Huntech::remove_squad(int squadId) {
    if (squadId <= 0)
        return StatusType::INVALID_INPUT;

    try {
        // 1. Find the squad
        shared_ptr<Squad> s = squads.find(TreeKey(squadId, squadId));
        MemberNode* root = s->representative.get();
        if (root != nullptr){root->squad_is_dead = true;}
        int currentAura = s->getTotalAura();

        // 2. Remove from squads first
        squads.remove(TreeKey(squadId, squadId));

        // 3. Now remove from auraTree
        try {
            auraTree.remove(TreeKey(currentAura, squadId));
        }
        catch (...) {
            // rollback squads.remove
            try {
                squads.insert(s, TreeKey(squadId, squadId));
            } catch (...) {
                // catastrophic but nothing more we can do
            }
            cout << "check 1 : " << currentAura << ": ";
            return StatusType::FAILURE;
        }
        return StatusType::SUCCESS;
    }
    catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    }
    catch (...) {
        //cout << "check 2 you are pagar: ";
        return StatusType::FAILURE;
    }
}


StatusType Huntech::add_hunter(int hunterId,
                               int squadId,
                               const NenAbility &nenType,
                               int aura,
                               int fightsHad)
{
    if (hunterId <= 0 || squadId <= 0 || fightsHad < 0 || aura < 0) {
        return StatusType::INVALID_INPUT;
    }
    if (hunters.contains(hunterId)) {
        return StatusType::FAILURE;
    }
    if (!squads.contains(TreeKey(squadId, squadId))) {
        return StatusType::FAILURE;
    }

    Hunter h(hunterId, nenType, aura, fightsHad);
    auto my_node = std::make_shared<MemberNode>(h);

    shared_ptr<Squad> my_squad = nullptr;
    TreeKey old_key;

    try {
        my_squad = squads.find(TreeKey(squadId, squadId));
        old_key = TreeKey(my_squad->getTotalAura(), squadId);
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    } catch (...) {
        return StatusType::FAILURE;
    }

    try {
        hunters.put(my_node, hunterId);
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    } catch (...) {
        return StatusType::FAILURE;
    }

    // Step 2: Add the hunter to the squad
    if (my_squad->representative == nullptr) {
        my_squad->representative = my_node;
        my_node->squad_sum_aura = aura;
        my_node->squad_total_nen = nenType;
    } else {
        MemberNode* parent = my_squad->representative.get();
        if (!parent) {
            return StatusType::FAILURE;
        }

        my_node->parent = parent;

        parent->size += 1;
        parent->squad_sum_aura += aura;

        my_node->squad_fights_cnt = -parent->squad_fights_cnt;
        my_node->r_nen = nenType + parent->squad_total_nen - parent->r_nen;
        parent->squad_total_nen += nenType;
    }

    // Step 3: Update aura tree safely
    try {
        auraTree.insert(my_squad, TreeKey(my_squad->getTotalAura(), squadId));
        auraTree.remove(old_key);
    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    } catch (...) {
        return StatusType::FAILURE;
    }

    return StatusType::SUCCESS;
}


output_t<int> Huntech::squad_duel(int squadId1, int squadId2) {
    if (squadId1 <= 0 || squadId2 <= 0 || squadId1 == squadId2) {
        return StatusType::INVALID_INPUT;
    }
    if (!squads.contains(TreeKey(squadId1, squadId1)) || !squads.contains(TreeKey(squadId2, squadId2))) {
        return StatusType::FAILURE;
    }

    try
    {
        shared_ptr<Squad> s1 = squads.find(TreeKey(squadId1,squadId1));
        shared_ptr<Squad> s2 = squads.find(TreeKey(squadId2,squadId2));
        if (s1->isEmpty()||s2->isEmpty())
            return StatusType::FAILURE;
        int score1 = s1->representative->squad_sum_aura + s1->experience;
        int score2 = s2->representative->squad_sum_aura + s2->experience;
        int result = 0;
        if (score1 > score2)
        {
            s1->experience += 3;
            result = 1;
        }
        else if (score2 > score1){
        s2->experience += 3;
        result = 3;
        }
        else
        {
            const NenAbility& nen1 = s1->representative->squad_total_nen;
            const NenAbility& nen2 = s2->representative->squad_total_nen;

            if (nen1 > nen2)
            {
                s1->experience += 3;
                result = 2;
            }
            else if (nen2 > nen1)
            {
                s2->experience += 3;
                result = 4;
            }
            else
            {
                s1->experience++;
                s2->experience++;
            }
        }
        s1->representative->squad_fights_cnt++;
        s2->representative->squad_fights_cnt++;

        return output_t<int>(result);
    }
    catch (...)
    {
        return StatusType::FAILURE;
    }
}

output_t<int> Huntech::get_hunter_fights_number(int hunterId) {
    if (hunterId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        if (!hunters.contains(hunterId))
            return StatusType::FAILURE;
        shared_ptr<MemberNode> node = hunters.get(hunterId);
        FindResult res = node->find();

        return output_t<int>(res.pathFights + node->hunter.getFightsHad());
    }
    catch (...)
    {
        return StatusType::FAILURE;
    }
}

output_t<int> Huntech::get_squad_experience(int squadId) {
    if (squadId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        shared_ptr<Squad> s = squads.find(TreeKey(squadId,squadId));
        return output_t<int>(s->experience);
    }
    catch (...)
    {
        return StatusType::FAILURE;
    }
}

output_t<int> Huntech::get_ith_collective_aura_squad(int i) {
    if (i <= 0)
        return StatusType::FAILURE;
    try
    {
        if (i > auraTree.getSize())
            return StatusType::FAILURE;
        shared_ptr<Squad> foundSquad= auraTree.select(i);
        return output_t<int>(foundSquad->getId());
    }
    catch (...)
    {
        return StatusType::FAILURE;
    }
    return StatusType::FAILURE;
}

output_t<NenAbility> Huntech::get_partial_nen_ability(int hunterId) {
    if (hunterId <= 0)
        return StatusType::INVALID_INPUT;
    try
    {
        if (!hunters.contains(hunterId)) {
            return StatusType::FAILURE;
        }
        std::shared_ptr<MemberNode> node = hunters.get(hunterId);
        FindResult res = node->find();
        if (res.isDead) {
            //sdfghjkjhgfdsdfghjk
            return StatusType::FAILURE;
        }
        return output_t<NenAbility>(res.pathSum);
    }
    catch (...)
    {
        return StatusType::FAILURE;
    }
}

StatusType Huntech::force_join(int forcingSquadId, int forcedSquadId) {
    if (forcingSquadId <= 0 || forcedSquadId <= 0 || forcingSquadId == forcedSquadId) {
        return StatusType::INVALID_INPUT;
    }

    shared_ptr<Squad> squad_a = nullptr; // forcing
    shared_ptr<Squad> squad_b = nullptr; // forced

    try {
        squad_a = squads.find(TreeKey(forcingSquadId, forcingSquadId));
        squad_b = squads.find(TreeKey(forcedSquadId,  forcedSquadId));
    } catch (...) {
        return StatusType::FAILURE;
    }

    /*if (forcingSquadId == 3311 || forcedSquadId == 2412) {
        cout << "fighting power: " << squad_a->getBattleValue() << " " << squad_b->getBattleValue() << endl;
        cout << "ID: " << forcingSquadId << " " << forcedSquadId << endl;
        cout << "squad exp " << squad_a->experience << " " << squad_b->experience << endl;
        cout << "squad aura " << squad_a->getTotalAura() << " " << squad_b->getTotalAura() << endl;
        cout << "squad nen " << squad_a->getTotalNen().getEffectiveNenAbility() << " " << squad_b->getTotalNen().getEffectiveNenAbility() << endl;

    }*/

    // Battle condition
    if (squad_a->isEmpty() ||
        (!squad_b->isEmpty() && squad_a->getBattleValue() <= squad_b->getBattleValue())) {
        return StatusType::FAILURE;
    }

    // Save forcing squad's old aura for later auraTree update
    int oldAuraA = squad_a->getTotalAura();

    // Save representatives (shared_ptr) before removing forced squad
    std::shared_ptr<MemberNode> rep_a = squad_a->representative;
    std::shared_ptr<MemberNode> rep_b = squad_b->representative; // may be null if empty


    squad_a->experience += squad_b->experience;
    // Remove forced squad from trees while its aura key is still correct
    StatusType rem = remove_squad(forcedSquadId);
    if (rem != StatusType::SUCCESS) {
        return StatusType::FAILURE;
    }

    // If forced squad was non-empty, merge the DSU trees
    if (rep_b) {
        rep_b->squad_is_dead = false;
        MemberNode* root_a = rep_a.get();
        MemberNode* root_b = rep_b.get();

        if (!root_a || !root_b) {
            return StatusType::FAILURE;
        }

        // Union by size
        if (root_a->size >= root_b->size) {
            // root_a stays root
            root_b->parent = root_a;
            root_a->size += root_b->size;
            root_a->squad_sum_aura += root_b->squad_sum_aura;

            // weighted nen update
            root_b->r_nen += root_a->squad_total_nen;
            root_b->r_nen -= root_a->r_nen;
            root_a->squad_total_nen += root_b->squad_total_nen;
            root_b->squad_fights_cnt -= root_a->squad_fights_cnt;
        } else {
            // root_b becomes root
            root_a->parent = root_b;
            root_b->size += root_a->size;
            root_b->squad_sum_aura += root_a->squad_sum_aura;

            // symmetric weighted nen update
            root_b->r_nen += root_a->squad_total_nen;
            root_a->r_nen -= root_b->r_nen;
            root_b->squad_total_nen += root_a->squad_total_nen;
            root_a->squad_fights_cnt -= root_b->squad_fights_cnt;

            squad_a->representative = rep_b;
        }
    }

    // Update auraTree entry for the forcing squad
    try {
        ///--------------------------------------
        auraTree.remove(TreeKey(oldAuraA, forcingSquadId));
        rep_a->squad_is_dead = false;
        auraTree.insert(squad_a, TreeKey(squad_a->getTotalAura(), forcingSquadId));


    } catch (const std::bad_alloc&) {
        return StatusType::ALLOCATION_ERROR;
    } catch (...) {
        return StatusType::FAILURE;
    }


    return StatusType::SUCCESS;
}
