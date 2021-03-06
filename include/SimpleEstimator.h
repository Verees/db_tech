//
// Created by Nikolay Yakovets on 2018-02-01.
//

#ifndef QS_SIMPLEESTIMATOR_H
#define QS_SIMPLEESTIMATOR_H

#include "Estimator.h"
#include "SimpleGraph.h"
#include <SimpleEvaluator.h>


#include <memory>
#include <cmath>
#include "SimpleGraph.h"
#include "RPQTree.h"
#include "Evaluator.h"
#include "Graph.h"

class SimpleFranzor : public Evaluator {
    std::shared_ptr<SimpleGraph> graph;

public:

    explicit SimpleFranzor(std::shared_ptr<SimpleGraph> &g);
    ~SimpleFranzor() = default;

    void prepare() override ;
    cardStat evaluate(RPQTree *query) override ;

    std::shared_ptr<SimpleGraph> evaluate_aux(RPQTree *q);
    static std::shared_ptr<SimpleGraph> project(uint32_t label, bool inverse, std::shared_ptr<SimpleGraph> &g);
    static std::shared_ptr<SimpleGraph> join(std::shared_ptr<SimpleGraph> &left, std::shared_ptr<SimpleGraph> &right);

    static cardStat computeStats(std::shared_ptr<SimpleGraph> &g);

};


class SimpleEstimator : public Estimator {

private:
    std::shared_ptr<SimpleGraph> graph;
    std::vector<std::vector<int> > CS = {};

    //[first][sec][{noOut, noPaths, noI}]
    //uint32_t *precal;
    uint32_t *precal;
    uint32_t precal_L2;

    inline uint32_t precal_noOut(uint32_t L1, uint32_t L2);
    inline uint32_t precal_noPaths(uint32_t L1, uint32_t L2);
    inline uint32_t precal_noIn(uint32_t L1, uint32_t L2);
    std::string precal_prep_label_to_string(uint32_t label);
    void precal_prep();


    uint32_t g_L, g_V, g_E;


    cardStat estimate_oude_stijl(int test[], int sizequery);

    int tree_ctr(RPQTree *q);
    int tree_to_array(RPQTree *q, int* ar, int *off);

public:


    explicit SimpleEstimator(std::shared_ptr<SimpleGraph> &g);
    ~SimpleEstimator();

    void prepare() override;
    cardStat estimate(RPQTree *q) override ;

};


#endif //QS_SIMPLEESTIMATOR_H
