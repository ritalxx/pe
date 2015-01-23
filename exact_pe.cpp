/*
 * bn.cpp
 *
 *  Created on: 2014年11月21日
 *      Author: xiaoxuliu
 */
#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include "bn.h"
#include "node.h"
#include "cpt.h"

using namespace std;
double pe_result = 1;
bool debug = false;
int max_cpt = 0;
int main(int argc, char *argv[]) {

	BayesianNetwork bn = BayesianNetwork();


	/* STEP 1: read data from file */
	bn.parseXbif(argv[1]);

	if(argv[2]) debug = true;

	/* STEP 2: removing evidence nodes */
	map<string, Node*> nodes = bn.getNodes();
	if(debug) cout<<"total nodes before instantiating by evidence:"<<bn.getNodes().size()<<endl;
	for(auto _node : nodes){
		if(!_node.second->isEvidence()) continue;
		if(debug) cout<<"removing evidence: "<<_node.second->getName()<<endl;
		vector<CPT*> _cpts = bn.searchCPT(_node.second);
		for(auto __cpt : _cpts){
			vector<Node*> nodes = __cpt->getNodes();
			if(nodes.size() == 1){
				pe_result = pe_result * __cpt->_table[_node.second->getValue()];
				bn.removeCPT(__cpt);
			}else{
				CPT c = __cpt->instantiate(_node.second,_node.second->getValue());
				bn.removeCPT(__cpt);
				bn.addCPT(new CPT(c));
			}
		}
		bn.removeNode(_node.second);
	}
	if(debug) cout<<"total nodes after instantiating by evidence:"<<bn.getNodes().size()<<endl;

	/* STEP 3: variable elimination by multiply and sum out */
	while(bn.getNodes().size() > 0){
		/* min fill heuristic */
		//Node *_node = bn.nextNodeByMinFill();

		/* min degree heuristic */
		Node *_node = bn.nextNodeByMinDegree();
		if(debug) cout<<"eliminating node: "<<_node->getName()<<endl;
		if(debug) cout<<"cpts left: "<<bn.getCPTs().size()<<endl;
		if(debug) cout<<"nodes left: "<<bn.getNodes().size()<<endl;
		vector<CPT*> _cpts = bn.searchCPT(_node);
		CPT *_new_cpt = CPT::sumOut(_cpts, _node);
		//cout<<"===== old cpts ====="<<endl;
		for(auto __cpt : _cpts){
			//cpt->printOut();
			bn.removeCPT(__cpt);
		}
		bn.addCPT(_new_cpt);
		//cout<<"===== new cpt ====="<<endl;
		//new_cpt->printOut();
		bn.removeNode(_node);
	}

	/* STEP 4: print result */
	cout<<pe_result<<" ln: "<<log(pe_result)<<endl;
	cout<<"max cpt size: "<<max_cpt<<endl;

	return 0;
};



