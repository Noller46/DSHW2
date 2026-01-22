//
// Created by ronca on 1/22/2026.
//

#ifndef SQUAD_H
#define SQUAD_H
#include "MemberNode.h"
#include "wet2util.h"

class Squad
{
    int squadId;
    int experience;
    MemberNode* representative;

    explicit Squad(int id): squadId(id), experience(0), representative(nullptr){}
    ~Squad() = default;

    int getTotalAura() const
    {
        if (representative == nullptr) return 0;
        return representative->squad_sum_aura;
    }

    const NenAbility& getTotalNen() const
    {
       
    }
};

#endif //SQUAD_H
