// 
// 234218 Data Structures 1.
// Semester: 2026A (Winter).
// Wet Exercise #2 - Huntech.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef HUNTECH26A2_H_
#define HUNTECH26A2_H_
#include "wet2util.h"
#include "AVLTree.h"
#include "HashMap.h"
#include "Squad.h"
#include "MemberNode.h"
#include "Hunter.h"
#include <memory>


class Huntech {
private:
    //
    // Here you may add anything you need to implement your Huntech class
    //
    AVLTree<Squad> squads;
    HashMap<shared_ptr<MemberNode>> hunters;

public:
    // <DO-NOT-MODIFY> {

    Huntech();
    virtual ~Huntech();

    StatusType add_squad(int squadId);
    
    StatusType remove_squad(int squadId);
    
    StatusType add_hunter(int hunterId,
                          int squadId,
                          const NenAbility &nenType,
                          int aura,
                          int fightsHad) {
        ///add errors
        Hunter me = Hunter(hunterId, nenType, aura, fightsHad);
        Squad my_squad = squads.find(squadId);
        MemberNode* my_node = new MemberNode(me);
        if (my_squad.representative == nullptr) {
            my_squad.representative = my_node;
            my_node->squad_sum_aura = aura;
            my_node->squad_total_nen += me.getNenAbility();
            return StatusType::SUCCESS;
        }
        else {
            MemberNode* parent = my_squad.representative;
            my_node->parent = parent;
            parent->size += my_node->size;
            parent->squad_sum_aura += aura;
            parent->squad_total_nen += me.getNenAbility();
            my_node->squad_total_nen = parent->squad_total_nen-parent->hunter.getNenAbility();
            my_node->squad_fights_cnt = fightsHad-(parent->squad_fights_cnt);
            return StatusType::SUCCESS;
        }
        return StatusType::FAILURE;
    }

    output_t<int> squad_duel(int squadId1, int squadId2);

    output_t<int> get_hunter_fights_number(int hunterId);

    output_t<int> get_squad_experience(int squadId);

    output_t<int> get_ith_collective_aura_squad(int i);

    output_t<NenAbility> get_partial_nen_ability(int hunterId);

    StatusType force_join(int forcingSquadId, int forcedSquadId) {
        //acount for empty
        Squad squad_a = squads.find(forcingSquadId);
        MemberNode* root_a = squad_a.representative;
        Squad squad_b = squads.find(forcedSquadId);
        MemberNode* root_b = squad_b.representative;

        if (squad_a.getBattleValue()>squad_b.getBattleValue()) {
            if (root_a->size>=root_b->size) {
                
            } else {

            }
            return StatusType::SUCCESS;
        }
        return StatusType::FAILURE;
    }

    // } </DO-NOT-MODIFY>
};

#endif // HUNTECH26A2_H_
