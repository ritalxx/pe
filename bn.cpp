/*
 * BayesianNetwork.cpp
 *
 *  Created on: 2014��11��16��
 *      Author: xiaoxuliu
 */
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <boost/regex.hpp>
#include "node.h"
#include "cpt.h"
#include "bn.h"

using namespace std;

BayesianNetwork::BayesianNetwork(){

};

bool BayesianNetwork::parseXbif(string filename){

	ifstream ifs(filename);
	if(!ifs.good()){
		cout<<"find not found."<<endl;
		return false;
	}

	string line;
	bool in_variable = false;
	bool in_probability = false;
	bool in_table = false;
	string last_node_name = "";
	int i;
	int parent_count;
	Node node("init");
	node.setEvidence(false);

	const boost::regex variable_start ("^<VARIABLE[^>]*>$", boost::regex::icase);
	const boost::regex variable_end ("^</VARIABLE>$", boost::regex::icase);
	const boost::regex name_field ("^<NAME>(.*)</NAME>$", boost::regex::icase);
	const boost::regex observed_field ("^<OBSERVED>(.*)</OBSERVED>$", boost::regex::icase);
	const boost::regex probability_start ("^<PROBABILITY[^>]*>$", boost::regex::icase);
	const boost::regex probability_end ("^</PROBABILITY>$", boost::regex::icase);
	const boost::regex table_start ("^<TABLE[^>]*>$", boost::regex::icase);
	const boost::regex table_end ("^</TABLE>$", boost::regex::icase);
	const boost::regex for_field ("<FOR>(.*)</FOR>", boost::regex::icase);
	const boost::regex given_field ("<GIVEN>(.*)</GIVEN>", boost::regex::icase);
	const boost::regex value_field ("^([\\.0-9]+) ([\\.0-9]+)$");

	getline(ifs, line);
	while(ifs.good()){
		line = trim_copy(line);
		boost::smatch sm;
		if(in_variable){
			if(boost::regex_match(line, sm, name_field) && sm.size() == 2){
				//cout<<"node name: "<<sm.str(1)<<endl;
				node.setName(sm.str(1));
			}
			if(boost::regex_match(line, sm, observed_field) && sm.size() == 2){
				if(debug) cout<<node.getName()<<" is evidence: true"<<endl;
				//cout<<"value: "<<sm.str(1)<<endl;
				node.setEvidence(true);
				node.setValue(sm.str(1));
			}else{
				//cout<<node.getName()<<"is evience: false"<<endl;
				//node.setEvidence(false);
				//node.setValue("");
			}
		}

		if(in_probability){
			if(boost::regex_match(line, table_end)){
				in_table = false;
			}
			if(boost::regex_match(line, sm, for_field) && sm.size() == 2){
				last_node_name = sm.str(1);
				parent_count = 0;
				//cout<<"node name: "<<sm.str(1)<<endl;
			}

			if(boost::regex_match(line, sm, given_field) && sm.size() == 2){

				if(debug) cout<<"node "<<sm.str(1)<<" is parent of "<<last_node_name<<endl;
				this->searchNode(last_node_name)->addParent(this->searchNode(sm.str(1)));
				this->searchNode(last_node_name)->getCPT()->addNode(this->searchNode(sm.str(1)));
				this->searchNode(sm.str(1))->addChild(this->searchNode(last_node_name));
				parent_count++;
			}

			if(in_table){
				if(debug) if(parent_count == 0) cout<<last_node_name<<" has no parents"<<endl;
				if(boost::regex_match(line, sm, value_field) && sm.size() == 3){
					if(i == 0) this->searchNode(last_node_name)->getCPT()->addNode(this->searchNode(last_node_name));
					this->searchNode(last_node_name)->getCPT()->setValue(make_pair(dec_to_bin(i,parent_count)+"0",strtod(sm.str(1).c_str(),NULL)));
					this->searchNode(last_node_name)->getCPT()->setValue(make_pair(dec_to_bin(i,parent_count)+"1",strtod(sm.str(2).c_str(),NULL)));
					if(debug) cout<<dec_to_bin(i,parent_count)+"0"<<": "<<sm.str(1)<<endl;
					if(debug) cout<<dec_to_bin(i,parent_count)+"1"<<": "<<sm.str(2)<<endl;
					i++;
				}
			}

			if(boost::regex_match(line, table_start)){
				in_table = true;
				i = 0;
			}
		}

		if(boost::regex_match(line, variable_start)){
			in_variable = true;
		}
		if(boost::regex_match(line, variable_end)){
			CPT cpt = CPT();
			node.setCPT(cpt);
			this->addNode(node);
			this->addCPT(node.getCPT());
			node.setEvidence(false);
			in_variable = false;
		}

		if(boost::regex_match(line, probability_start)){
			in_probability = true;
		}
		if(boost::regex_match(line, probability_end)){
			in_probability = false;
		}

		//cout<<line<<endl;
		getline(ifs, line);
	}

	return true;
};

bool BayesianNetwork::addNode(Node x){
	this->_nodes[x.getName()] = new Node(x);
	return true;
};

bool BayesianNetwork::removeNode(Node* p){
	this->_nodes.erase(this->_nodes.find(p->getName()));
	map<string, Node*> parents = p->getParents();
	for(auto _p : parents){
		_p.second->removeChild(p);
	}
	map<string, Node*> children = p->getChildren();
	for(auto _p : children){
		_p.second->removeParent(p);
	}
	return true;
};

map<string, Node*> BayesianNetwork::getNodes(){
	return this->_nodes;
};

vector<CPT*> BayesianNetwork::getCPTs(){
	return this->_cpts;
};

bool BayesianNetwork::addCPT(CPT* cpt){
	if(cpt->_fake == true) return true;
	this->_cpts.push_back(cpt);
	return true;
};

bool BayesianNetwork::removeCPT(CPT* cpt){
	this->_cpts.erase(remove(this->_cpts.begin(), this->_cpts.end(), cpt), this->_cpts.end());
	delete cpt;
	return true;
};

Node* BayesianNetwork::searchNode(string name){
	return this->_nodes[name];
};

vector<CPT*> BayesianNetwork::searchCPT(Node* p){
	vector<CPT *> rs;
	for(auto _cpt : this->_cpts){
		vector<Node *> _nodes = _cpt->getNodes();
		for(auto __node : _nodes){
			if(__node->getName() == p->getName()){
				rs.push_back(_cpt);
				break;
			}
		}
	}
	return rs;
}

/* TODO: refactor */
Node* BayesianNetwork::nextNodeByMinFill(){
	map<Node*,vector<Node*>> pgraph;
	for(auto _cpt : this->_cpts){
		vector<Node*> _nodes1 = _cpt->getNodes();
		for(auto _node1 : _nodes1){
			vector<Node*> _nodes2 = _cpt->getNodes();
			for(auto _node2 : _nodes2){
				if(_node1 != _node2) pgraph[_node1].push_back(_node2);
			}
		}
	}

	Node* min_node = this->_nodes.begin()->second;

	int min = pgraph.size();
	for(auto _pnode : pgraph){
		int _pmin = (_pnode.second.size() * (_pnode.second.size() - 1));
		for(auto _node1 : _pnode.second){
			for(auto _node2 : _pnode.second){
				if(_node1 == _node2) continue;
				bool _found = false;
				for(auto _node3 : pgraph[_node1]){
					if(_node2 == _node3){
						_found = true;
						break;
					}
				}
				if(_found) _pmin -= 1;
			}
		}
		_pmin = _pmin / 2;
		if(_pmin < min){
			min = _pmin;
			min_node = _pnode.first;
		}
	}
	return min_node;
};

Node* BayesianNetwork::nextNodeByMinDegree(){
	map<Node*,vector<Node*>> pgraph;
	for(auto _cpt : this->_cpts){
		vector<Node*> _nodes1 = _cpt->getNodes();
		for(auto _node1 : _nodes1){
			vector<Node*> _nodes2 = _cpt->getNodes();
			for(auto _node2 : _nodes2){
				if(_node1 != _node2) pgraph[_node1].push_back(_node2);
			}
		}
	}

	Node* min_node = this->_nodes.begin()->second;

	int min = pgraph[this->_nodes.begin()->second].size();
	for(auto _pnode : pgraph){
		if( (int) _pnode.second.size() < min){
			min = _pnode.second.size();
			min_node = _pnode.first;
		}
	}
	return min_node;
};
