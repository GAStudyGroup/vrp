#ifndef EXTRA_CPP
#define EXTRA_CPP

#include "Population.hpp"

namespace Extra{
    void applyMutation(Tour&);
    void applyMutation(Population &pop);

    void popReset(Population &);    

    void applyRepair(Tour&);
    void applyRepair(Population&);

    void applyRepairV4(Tour&);
    void applyRepairV4(Population&);

    void applyCombined(Tour&);
    void applyCombined(Population&);

    void applyOptInPop(Population&);
    void applyOptInSubs(Tour&);

    void applyTrim(Tour&);
    void applyTrim(Population&);

    void applyKmeans(Tour&);
    void applyKmeans(Population&);
}
#endif