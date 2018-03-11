//
// Created by Nikolay Yakovets on 2018-02-01.
//

#include "SimpleGraph.h"
#include "SimpleEstimator.h"
#include <iostream>
#include <iomanip>

SimpleEstimator::SimpleEstimator(std::shared_ptr<SimpleGraph> &g){

    // works only with SimpleGraph
    graph = g;

}

void SimpleEstimator::prepare() {
    //g_E = graph->getNoEdges();
    g_V = graph->getNoVertices();
    g_L = graph->getNoLabels();



    std::cout << "3.1" << std::endl;
    // do your prep here
    //std::vector<std::int [noLabels*2+1]> CS;

    std::vector<int> prop;

    for (int k=0; k<g_L*2+1; k++){
        prop.push_back(0);
    }



    std::cout << "3.2" << std::endl;

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

    for ( const auto &row : CS )
    {
        for ( const auto &s : row ) {
            std::cout << std::setw(6) << s << ' ';
        }
        std::cout << std::endl;
    }

    int total = 0;

    for (int i = 0; i<CS.size(); i++){
        total = total + CS[i][g_L*2];
    }

    std::cout << total << ' ';

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
    //q->print();

    // perform your estimation here

    int sizequery = tree_ctr(q);
    int test[sizequery];
    int tree_offset = 0;

    tree_to_array(q, test, &tree_offset);

    std::cout << "\n" << "henk:";
    for (int asdasd=0; asdasd<sizequery; asdasd++) {
        std::cout << test[asdasd] << ',';
    }

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

    for (int i = 0; i<CS.size(); i++){
        total = total + CS[i][g_L*2];
    }

   // std::cout << total << ' ';

/***********************************************
 * Out:
 */

    for (int i = 0; i<CS.size(); i++){
        int colum ;

        if (test[0] < g_L){
            colum = test[0] + g_L;
        } else{
            colum = test[0] - g_L;
        }

        if((sizequery >  1) and (CS[i][colum] != 0) and (CS[i][test[1]] != 0) or
           (sizequery == 1) and (CS[i][colum] != 0)){

            prepre_estimate = prepre_estimate + CS[i][colum];
        }
    }

    for (int i = 0; i<CS.size(); i++){
        if(CS[i][test[0]] != 0){
            pre_total = pre_total + CS[i][test[0]];
            prepre_total = prepre_total + CS[i][g_L*2];
        }
    }

    out = prepre_estimate/pre_total * prepre_total;

    //asdfasdf
    //i = 0:
        //uitlezen tabel

/***********************************************
 * In + estimate:
 */
    double pre_in = 0 ;
    double estimate = 0;

    if(sizequery > 1){
        for (int i = 0; i < CS.size(); i++){
            int colum;

            if (test[0] < g_L){
                colum = test[0] + g_L;
            } else{
                colum = test[0] - g_L;
            }

            if(CS[i][colum] != 0 and CS[i][test[1]] != 0){
                pre_in += CS[i][colum];
                estimate += CS[i][colum];
            }
        }
    } else {
        for (int i = 0; i < CS.size(); i++){

            if(CS[i][test[0]] != 0 ){
                pre_in += CS[i][0];
                estimate += CS[i][0];
            }
        }
    }

    //std::cout << "start: " << pre_in << ", ";

    for (int i = 2; i < sizequery; i++) {

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
                double entityTot = CS[j][g_L*2];
                double pre_avg = prepre/entityTot;
                pre = pre + prepre;
                avg_tot += pre_avg;
            }
        }

        avg = avg_tot / counter;
        ratio = pre/tot;
        pre_in = pre_in * ratio;
        estimate = estimate * avg;
        std::cout << "avg_tot: " << avg_tot << ", ";
        std::cout << "counter: " << counter << ", ";
        std::cout << "avg: " << avg << ", ";
        //std::cout << "r: " << ratio << ", ";
        //std::cout << "u: " << pre_in << "; ";
    }

    in = pre_in;

    //sdsdf


/***********************************************
 * Estimate:
 */



    return cardStat {(uint32_t) out, (uint32_t) estimate, (uint32_t) in};
}