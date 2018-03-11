//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"
#include "SimpleFranzor.h"
#include <iostream>
#include <iomanip>
#include <string>

//time
#include <chrono>
#include <thread>

#define DEBUG false

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;

    precal_L2 = g->getNoLabels() * 2;

    precal = (uint32_t*) malloc(sizeof(uint32_t) * precal_L2 * precal_L2 * 3);

    if (precal == NULL) {
        std::cout << "could not grab mem, aboring.\n";
        exit(1);
    }
}

SimpleEstimator::~SimpleEstimator(){
    free(precal);
}


/*******************************
 * PRECAL BLOCK - start
 *******************************/

inline uint32_t SimpleEstimator::precal_noOut(uint32_t L1, uint32_t L2) {
    return precal[(3 * ((precal_L2 * L1) + L2)) + 0];
}

inline uint32_t SimpleEstimator::precal_noPaths(uint32_t L1, uint32_t L2) {
    return precal[(3 * ((precal_L2 * L1) + L2)) + 1];
}

inline uint32_t SimpleEstimator::precal_noIn(uint32_t L1, uint32_t L2) {
    return precal[(3 * ((precal_L2 * L1) + L2)) + 2];
}

void SimpleEstimator::precal_prep() {
    for (uint32_t L1 = 0; L1 < precal_L2; L1++) {
        for (uint32_t L2 = 0; L2 < precal_L2; L2++) {
            uint32_t off = (3 * ((precal_L2 * L1) + L2));

            bool werkt = false;

            ///*
            for (int r = 0; r < CS.size(); r++) {
                uint32_t stap_1 = (L1 + g_L) % precal_L2;

                if (CS[r][stap_1]) {
                    if (CS[r][L2]) {
                        werkt = true;
                        break;
                    }
                }
            }
            //*/

            if (werkt) {
                std::string query =
                        precal_prep_label_to_string(L1)
                        + "/" +
                        precal_prep_label_to_string(L2);

                //std::cout << query;

                RPQTree *queryTree = RPQTree::strToTree(query);

                // perform evaluation
                auto ev = std::make_unique<SimpleFranzor>(graph);
                ev->prepare();
                auto actual = ev->evaluate(queryTree);

                //std::cout << "Actual (noOut, noPaths, noIn) : ";
                //actual.print();

                precal[off + 0] = actual.noOut;
                precal[off + 1] = actual.noPaths;
                precal[off + 2] = actual.noIn;
            } else {
                precal[off + 0] = 0;
                precal[off + 1] = 0;
                precal[off + 2] = 0;
            }
        }
    }
}

std::string SimpleEstimator::precal_prep_label_to_string(uint32_t label) {
    if (label < g_L) {
        return std::to_string(label) + "+";
    } else {
        return std::to_string(label - g_L) + "-";
    }
}

/*******************************
 * PRECAL BLOCK - end
 *******************************/




void SimpleEstimator::prepare() {
    //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    //g_E = graph->getNoEdges();
    g_V = graph->getNoVertices();
    g_L = graph->getNoLabels();


    // do your prep here
    //std::vector<std::int [noLabels*2+1]> CS;

    std::vector<int> prop;

    for (int k=0; k<g_L*2+1; k++){
        prop.push_back(0);
    }




    //Run by each vertice
    for (int k=0; k < g_V; k++){

        if(!graph->adj[k].empty()){                                         //Check wether the vertice has any outgoing

            for (int j=0; j< graph->adj[k].size(); ++j){

                //std::int subject = graph.adj[i](j,0);

                prop[graph->adj[k][j].first] ++;

            }
        }
        if(!graph->reverse_adj[k].empty()){
            for (int j=0; j < graph->reverse_adj[k].size(); ++j){
                //std::int subject = graph.adj[i](j,0);
                prop[graph->reverse_adj[k][j].first + g_L ] ++;
            }
        }



        int i = 0;
        //for (; (same == 0) and (i < CS.size()); i++) {

        int same = 0;
        while(same == 0 and i<CS.size()){
            same = 1;
            for(int j = 0; j<g_L; j++){
                if(prop[j] != CS[i][j] and prop[j] == 0){
                    same = 0;
                    break;
                }
            }
            i++;
        }


        if (same == 1){
            for(int j = 0; j<g_L*2; j++){
                CS[i-1][j] = CS[i-1][j] + prop[j];
            }
            CS[i-1][g_L*2] ++;
        } else {
            CS.push_back(prop);
            CS[CS.size()-1][g_L*2] = 1;
        }


        prop.clear();
        for (int k=0; k<g_L*2+1; k++){
            prop.push_back(0);
        }

    }

    if (DEBUG) {
        for (const auto &row : CS) {
            for (const auto &s : row) {
                std::cout << std::setw(6) << s << ' ';
            }
            std::cout << std::endl;
        }


        std::cout << "\n\n" << CS.size() << "\n" ;

    }

    int total = 0;

    for (int i = 0; i<CS.size(); i++){
        total = total + CS[i][g_L*2];
    }

    if (DEBUG) {
        std::cout << total << ' ';
    }


    //gogogo
    precal_prep();

    /*
    std::cout << "\n xx-" << precal_noOut(0,1) << "-xx\n";
    std::cout << "\n xx-" << precal_noOut(0,5) << "-xx\n";
    std::cout << "\n xx-" << precal_noOut(1,2) << "-xx\n";
    */
}



int SimpleEstimator::tree_ctr(RPQTree *q) {
    if(q->left == nullptr && q->right == nullptr) {
        return 1;
    } else {
        return ((q->left != nullptr) ? tree_ctr(q->left) : 0) + ((q->right!= nullptr) ? tree_ctr(q->right) : 0);
    }
}

int SimpleEstimator::tree_to_array(RPQTree *q, int* ar, int *off) {
    if(q->left == nullptr && q->right == nullptr) {
        //std::cout << q->data;

        if (q->data.back() == '+') {
            //std::cout <<  q->data.substr(0, q->data.size() -1);
            ////std::cout << *off;
            ar[*off] = std::stoi(q->data.substr(0, q->data.size() -1));
            (*off)++;
        } else if (q->data.back() == '-') {
            ar[*off] = std::stoi(q->data.substr(0, q->data.size() -1)) \
                     + g_L;
            (*off)++;
        } else {
            std::cout << "broken.";
        }

    } else {
        if(q->left != nullptr) tree_to_array(q->left, ar, off);
        if(q->right != nullptr) tree_to_array(q->right, ar, off);
    }
}








cardStat SimpleEstimator::estimate(RPQTree *q) {
    int sizequery = this->tree_ctr(q);
    int test[sizequery];
    int tree_offset = 0;

    this->tree_to_array(q, test, &tree_offset);

    if (DEBUG) {
        std::cout << "\n" << "henk:";
        for (int asdasd = 0; asdasd < sizequery; asdasd++) {
            std::cout << test[asdasd] << ',';
        }
    }

    //IN CASE OF EMERGENCY
    //return estimate_oude_stijl(test, sizequery);


    int test_rev[sizequery];
    for (int i = 0; i < sizequery; i++){
        test_rev[i] = (test[sizequery - i - 1] + g_L) % precal_L2;
    }

    if (DEBUG) {
        std::cout << "query: \n";
        for (int i = 0; i < sizequery; i++) {
            std::cout << "X: " << test[i] << ", " << test_rev[i] << "\n";
        }
        std::cout << "query end.\n";
    }


    cardStat heen, terug, uitkomst;

    heen  = estimate_oude_stijl(test,     sizequery);
    terug = estimate_oude_stijl(test_rev, sizequery);

    uitkomst.noPaths = (heen.noPaths + terug.noPaths) /2;

    uitkomst.noIn = heen.noIn;
    uitkomst.noOut = terug.noIn;

    return uitkomst;
}

















cardStat SimpleEstimator::estimate_oude_stijl(int test[], int sizequery) {
    //q->print();



    //int test[sizequery] = {0,1,2,2,3};
    //int test[sizequery] = { 1, 8, 9, 8};

    /*
    std::uint32_t estimate = 0;
    std::uint32_t pre_estimate = 0;
    std::uint32_t in = 0;
    std::uint32_t out = 0;
*/
    double pre_estimate = 0;
    double in = 0;
    double out = 0;
    double prepre_estimate = 0;
    double pre_total = 0;
    double prepre_total = 0;

    int total = 0;

    for (int i = 0; i < CS.size(); i++) {
        total = total + CS[i][g_L * 2];
    }

    // std::cout << total << ' ';

/***********************************************
 * Out:
 */

    for (int i = 0; i < CS.size(); i++) {
        int colum;

        if (test[0] < g_L) {
            colum = test[0] + g_L;
        } else {
            colum = test[0] - g_L;
        }

        if ((sizequery > 1) and (CS[i][colum] != 0) and (CS[i][test[1]] != 0) or
                                                        (sizequery == 1) and (CS[i][colum] != 0)) {

            prepre_estimate = prepre_estimate + CS[i][colum];
        }
    }

    for (int i = 0; i < CS.size(); i++) {
        if (CS[i][test[0]] != 0) {
            pre_total = pre_total + CS[i][test[0]];
            prepre_total = prepre_total + CS[i][g_L * 2];
        }
    }

    out = prepre_estimate / pre_total * prepre_total;

    //asdfasdf
    //i = 0:
    //uitlezen tabel

/***********************************************
 * In + estimate:
 */
    double pre_in = 0;
    double estimate = 0;

    if (sizequery >= 2) {
        pre_in   =  precal_noIn(test[0], test[1]);
        estimate =  precal_noPaths(test[0], test[1]);

        /*
        for (int i = 0; i < CS.size(); i++) {
            int colum;

            if (test[0] < g_L) {
                colum = test[0] + g_L;
            } else {
                colum = test[0] - g_L;
            }

            if (CS[i][colum] != 0 and CS[i][test[1]] != 0) {
                pre_in += CS[i][colum];
                estimate += CS[i][colum];
            }
        }
        */
    }else {
        for (int i = 0; i < CS.size(); i++){

            if(CS[i][test[0]] != 0 ){
                pre_in += CS[i][0];
                estimate += CS[i][0];
            }
        }
    }

    //std::cout << "start: " << pre_in << ", ";

    for (int i = 3; i < sizequery; i++) {

        int colum_back;
        double pre = 0;
        double tot = 0;
        double ratio;
        double avg;
        double avg_tot = 0;
        int counter = 0;

        if (test[i-1] < g_L){
            colum_back = test[i-1] + g_L;
        } else{
            colum_back = test[i-1] - g_L;
        }

        for (int j = 0; j < CS.size(); j++){
            if(CS[j][colum_back] != 0){
                tot = tot + CS[j][colum_back];
            }
            if(CS[j][colum_back] != 0 and CS[j][test[i]] != 0 ){
                counter ++;
                double prepre = CS[j][colum_back];
                double entityTot = CS[j][g_L * 2];
                double pre_avg = prepre/entityTot;
                pre = pre + prepre;
                avg_tot += pre_avg;
            }
        }

        avg = avg_tot / counter;
        ratio = pre/tot;
        pre_in = pre_in * ratio;
        estimate = estimate * avg;

        if (DEBUG) {
            std::cout << "avg_tot: " << avg_tot << ", ";
            std::cout << "counter: " << counter << ", ";
            std::cout << "avg: " << avg << ", ";
            //std::cout << "r: " << ratio << ", ";
            //std::cout << "u: " << pre_in << "; ";
        }
    }

    in = pre_in;

    //sdsdf






    return cardStat {(uint32_t) out, (uint32_t) estimate, (uint32_t) in};
}