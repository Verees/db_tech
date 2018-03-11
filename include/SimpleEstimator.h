//
// Created by Nikolay Yakovets on 2018-02-01.
//

#ifndef QS_SIMPLEESTIMATOR_H
#define QS_SIMPLEESTIMATOR_H

#include "Estimator.h"
#include "SimpleGraph.h"

class SimpleEstimator : public Estimator {

private:
    std::shared_ptr<SimpleGraph> graph;
    std::vector<std::vector<int> > CS = {};

    uint32_t g_L, g_V, g_E;



    int tree_ctr(RPQTree *q);
    int tree_to_array(RPQTree *q, int* ar, int *off);

public:


    explicit SimpleEstimator(std::shared_ptr<SimpleGraph> &g);
    ~SimpleEstimator() = default;

    void prepare() override;
    cardStat estimate(RPQTree *q) override ;

};


#endif //QS_SIMPLEESTIMATOR_H
