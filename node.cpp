/*
 * Node.cpp
 *
 *  Created on: 2014年11月21日
 *      Author: xiaoxuliu
 */
#include <iostream>
#include <vector>
#include "node.h"
#include "cpt.h"
#include "bn.h"

using namespace std;

Node::Node(string name){
	this->_name = name;
	this->_is_evidence = false;
	this->_cpt = new CPT();
}

bool Node::setEvidence(bool is_evidence){
	this->_is_evidence = is_evidence;
	return true;
}

bool Node::isEvidence(){
	return this->_is_evidence;
}

bool Node::setValue(string value){
	this->_value = value;
	return true;
}

string Node::getValue(){
	return this->_value;
}

CPT* Node::getCPT(){
	return this->_cpt;
}

bool Node::setName(string name){
	this->_name = name;
	return true;
}

string Node::getName(){
	return this->_name;
}

bool Node::addParent(Node* p){
	this->_parents[p->getName()] = p;
	return true;
}

bool Node::addChild(Node* p){
	this->_children[p->getName()] = p;
	return true;
}

bool Node::removeParent(Node* p){
	this->_parents.erase(this->_parents.find(p->getName()));
	return true;
}

bool Node::removeChild(Node* p){
	this->_children.erase(this->_children.find(p->getName()));
	return true;
}

map<string, Node*> Node::getParents(){
	return this->_parents;
}

map<string, Node*> Node::getChildren(){
	return this->_children;
}

bool Node::setCPT(CPT cpt){
	this->_cpt = new CPT(cpt);
	return true;
}
