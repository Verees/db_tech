//
// Created by Nikolay Yakovets on 2018-02-01.
//

#ifndef QS_SIMPLEESTIMATOR_H
#define QS_SIMPLEESTIMATOR_H

#include "Estimator.h"
#include "SimpleGraph.h"
#include <SimpleEvaluator.h>

class SimpleEstimator : public Estimator {

private:
    std::shared_ptr<SimpleGraph> graph;
    std::vector<std::vector<int> > CS = {};

    //[first][sec][{noOut, noPaths, noI}]
    //uint32_t *precal;
    uint16_t *precal;
    uint32_t precal_L2;

    inline uint32_t precal_noOut(uint32_t L1, uint32_t L2);
    inline uint32_t precal_noPaths(uint32_t L1, uint32_t L2);
    inline uint32_t precal_noIn(uint32_t L1, uint32_t L2);
    std::string precal_prep_label_to_string(uint32_t label);
    void precal_prep();


    uint32_t g_L, g_V, g_E;



    int tree_ctr(RPQTree *q);
    int tree_to_array(RPQTree *q, int* ar, int *off);

public:


    explicit SimpleEstimator(std::shared_ptr<SimpleGraph> &g);
    ~SimpleEstimator();

    void prepare() override;
    cardStat estimate(RPQTree *q) override ;

};


#endif //QS_SIMPLEESTIMATOR_H
