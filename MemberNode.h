#ifndef MEMBERNODE_H
#define MEMBERNODE_H
#include <memory>
#include "Hunter.h"
#include "wet2util.h"



struct FindResult  //makeshift pair - find will return
{
    class MemberNode* root;
    NenAbility pathSum;

    FindResult(class MemberNode* r, const NenAbility& sum): root(r),pathSum(sum){}
};

class MemberNode
{
public:
    std::shared_ptr<Hunter> hunter;

    MemberNode* parent;


    NenAbility r_nen;  //delta from node to parent, box problem logic/ if root partial sum
    //the rest are relavant if node is root
    NenAbility squad_total_nen;
    int size;
    int squad_sum_aura; //aura of all squad
    int squad_fights_cnt; //lazy logic

    explicit MemberNode(const std::shared_ptr<Hunter>& h) : hunter(h), parent(nullptr),r_nen(h->getNenAbility())
    , squad_total_nen(h->getNenAbility()), size(1), squad_sum_aura(h->getAura()), squad_fights_cnt(0){}

    ~MemberNode() = default;

    FindResult find()
    {
        if (parent == nullptr)
            return FindResult(this, r_nen);

        FindResult res = parent->find();

        parent = res.root;

        r_nen += res.pathSum;
        NenAbility myTotal = r_nen; //total to root
        r_nen = r_nen - res.root->r_nen; //delta relarive to root

        return FindResult(parent,myTotal);
    }
};


#endif
