//
// Created by ronca on 1/20/2026.
//
#include "wet2util.h"
#ifndef HUNTER_H
#define HUNTER_H
class Hunter
{
    int id;
    NenAbility nenA;
    int aura;
    int fightsHad;
public:
    Hunter(int id, const NenAbility& nen, int aura, int fights): id(id), nenA(nen), aura(aura), fightsHad(fights){}

    int getAura() const {return aura;}
    NenAbility getNenAbility(){return nenA;}
    int getFightsHad(){return fightsHad;}
};
#endif //HUNTER_H
