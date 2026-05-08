#pragma once
#include "Relic.h"

class RelicPool {
public:
    static Relic* getRandomRelic();
    static Relic* getRandomCommonRelic();
    static Relic* getRandomRareRelic();
    static Relic* getRandomLegendaryRelic();
};