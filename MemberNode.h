#ifndef MEMBERNODE_H
#define MEMBERNODE_H
#include <memory>
#include "Hunter.h"
#include "wet2util.h"



struct FindResult  //makeshift pair - find will return
{
    class MemberNode* root;
    NenAbility pathSum;
    int pathFights;
    bool isDead;

    FindResult(class MemberNode* r, const NenAbility& sum, int fights, bool isDead): root(r),pathSum(sum), pathFights(fights), isDead(isDead){}
};

class MemberNode
{
public:
    Hunter hunter;

    MemberNode* parent;


    //NenAbility r_nen;  //delta from node to parent, box problem logic/ if root partial sum
    //the rest are relevant if node is root
    NenAbility squad_total_nen;
    NenAbility r_nen;
    int size;
    int squad_sum_aura; //aura of all squad
    int squad_fights_cnt; //lazy logic
    bool squad_is_dead;

    explicit MemberNode(Hunter& h) : hunter(h), parent(nullptr), squad_total_nen(h.getNenAbility()),
    r_nen(h.getNenAbility()), size(1), squad_sum_aura(h.getAura()), squad_fights_cnt(0), squad_is_dead(false){}

    ~MemberNode() = default;

    FindResult find()
    {
        if (parent == nullptr)
            return FindResult(this, r_nen, squad_fights_cnt, squad_is_dead);

        FindResult res = parent->find();

        parent = res.root;

        squad_is_dead = res.isDead;

        r_nen += res.pathSum;
        NenAbility myTotalNen = r_nen; //total to root
        r_nen -= res.root->r_nen; //delta relative to root

        //same logic
        squad_fights_cnt += res.pathFights;
        int myTotalFights = squad_fights_cnt;
        squad_fights_cnt -= res.root->squad_fights_cnt;

        return FindResult(parent,myTotalNen, myTotalFights, squad_is_dead);
    }
};


#endif
