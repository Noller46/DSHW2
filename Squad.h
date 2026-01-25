//
// Created by ronca on 1/22/2026.
//

#ifndef SQUAD_H
#define SQUAD_H
#include "MemberNode.h"
#include "wet2util.h"

class Squad
{
public:
    int squadId;
    int experience;
    MemberNode* representative;

    explicit Squad(): squadId(0), experience(0), representative(nullptr){}
    explicit Squad(int id): squadId(id), experience(0), representative(nullptr){}
    explicit Squad(int id, int exp): squadId(id), experience(exp), representative(nullptr){}
    ~Squad() = default;

    int getTotalAura() const
    {
        if (representative == nullptr) return 0;
        return representative->squad_sum_aura;
    }

    const NenAbility& getTotalNen() const
    {
       if (representative == nullptr)
       {
           static NenAbility zero;
           return zero;
       }
        return representative->squad_total_nen;
    }

    int getBattleValue()const
    {
        int val = this->experience;
        if (representative != nullptr)
        {
            val += getTotalAura();
            val += getTotalNen().getEffectiveNenAbility();
        }
        return val;
    }

    bool isEmpty() const
    {
        return representative == nullptr;
    }

    void addExperience(int num)
    {
        experience += num;
    }
    int getId() const
    {
        return squadId;
    }
};

#endif //SQUAD_H
