/*
 * CPT.cpp
 *
 *  Created on: 2014年11月21日
 *      Author: xiaoxuliu
 */
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include "node.h"
#include "cpt.h"
#include "bn.h"

using namespace std;

CPT::CPT(){
	this->_fake = false;
};

bool CPT::setValue(pair<string, double> value){
	this->_table.insert(value);
	return true;
};

bool CPT::addNode(Node* p){
	this->_nodes.push_back(p);
	return true;
};

vector<Node*> CPT::getNodes(){
	return this->_nodes;
};

double CPT::getValue(map<string, string> assignment){
	string key = "";
	for(auto n : this->_nodes){
		if(assignment.find(n->getName()) == assignment.end()){
			return 0.0;
		}else{
			key += assignment[n->getName()];
		}
	}
	return this->_table[key];
};

CPT CPT::instantiate(Node* p, string value){
	CPT *cpt = new CPT();
	int i = 0;
	bool found = false;
	for(auto _node : this->_nodes){
		if(_node != p){
			if(!found) i++;
			cpt->addNode(_node);
		}else{
			found = true;
		}
	}
	for(auto v : this->_table){
		if(v.first[i] == value[0]){
			cpt->setValue(make_pair(v.first.substr(0,i) + v.first.substr(i + 1),v.second));
		}
	}
	return *cpt;
};

CPT CPT::multiply(CPT cpt){
	vector<Node*> sub;
	vector<Node*> nodes1 = this->_nodes;
	vector<Node*> nodes2 = cpt.getNodes();
	for(auto _node : nodes1){
		bool found = false;
		for(auto __node : nodes2){
			if(_node == __node){
				found = true;
				break;
			}
		}
		if(!found) nodes2.push_back(_node);
	}
    CPT *result = new CPT();
	for(auto _node : nodes2){
		result->addNode(_node);
	}
	if(debug) cout<<"current cpt size: "<<nodes2.size()<<endl;
	max_cpt = max(max_cpt, (int) nodes2.size());

    int size = pow(2, nodes2.size());
    for(int i = 0; i < size; i++){
    	string key = dec_to_bin(i, nodes2.size());
    	map<string, string> assignment;
    	int j = 0;
    	for(auto _node : nodes2){
    		string s;
    		s.push_back(key[j]);
    		assignment.insert(make_pair(_node->getName(), s));
    		j++;
    	}
    	result->setValue(make_pair(key,this->getValue(assignment)*cpt.getValue(assignment)));
    	//if(i > 0 && i % 1000 == 0) cout<<i<<" of "<<size<<" completed."<<endl;
    }
    return *result;
};

CPT* CPT::sumOut(vector<CPT*> cpts, Node* p){
	vector<CPT*> cpts0, cpts1;
	double factor0 = 1, factor1 = 1;
	for(auto _cpt : cpts){
		if(_cpt->getNodes().size() == 1){
			factor0 = factor0 * _cpt->_table["0"];
			factor1 = factor1 * _cpt->_table["1"];
			//pe_result = pe_result * (c->_table["0"] + c->_table["1"]);
			//cout<<"current result: "<<pe_result<<endl;
			continue;
		}
//		CPT *a = new CPT(c->instantiate(x->getName(),"0"));
//		cout<<"instantiate before: "<<x->getName()<<endl;
//		c->printOut();
//		cout<<"instantiate after: "<<x->getName()<<endl;
//		a->printOut();
		cpts0.push_back(new CPT(_cpt->instantiate(p,"0")));
		cpts1.push_back(new CPT(_cpt->instantiate(p,"1")));
	}
	CPT *result = new CPT();

	if(cpts0.size() == 0){
		pe_result = pe_result * (factor0 + factor1);
		//cout<<"current result: "<<pe_result<<endl;
		result->_fake = true;
		return result;
	}

	CPT *c0 = cpts0[0];
	CPT *c1 = cpts1[0];
	for(auto _cpt : cpts0){
		if(_cpt != cpts0[0]){
			*c0 = c0->multiply(*_cpt);
//			cout<<"====multiply===="<<endl;
//			c0->printOut();
//			cout<<"====multiply===="<<endl;
		}
	}
	for(auto _cpt : cpts1){
		if(_cpt != cpts1[0]){
			*c1 = c1->multiply(*_cpt);
//			cout<<"====multiply===="<<endl;
//			c1->printOut();
//			cout<<"====multiply===="<<endl;
		}
	}

//	cout<<"====multiply result===="<<endl;
//	c0->printOut();
//	cout<<"====multiply result===="<<endl;
//	cout<<"====multiply result===="<<endl;
//	c1->printOut();
//	cout<<"====multiply result===="<<endl;
	for ( auto it = c0->_table.begin(); it != c0->_table.end(); ++it ){
		result->setValue(make_pair(it->first,it->second * factor0 + c1->_table[it->first] * factor1));
	}
	vector<Node*> nodes = c0->getNodes();
	for(auto _node : nodes){
		result->addNode(_node);
	}
	return result;
};

void CPT::printOut(){
	for(auto _node : this->_nodes){
		cout<<_node->getName();
	}
	cout<<endl;
	for(auto _line : this->_table){
		cout<<_line.first<<" "<<_line.second<<endl;
	}
};
