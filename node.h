/*
 * node.h
 *
 *  Created on: 2014年11月21日
 *      Author: meetsea
 */

#ifndef EXACT_PE_NODE_H_
#define EXACT_PE_NODE_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "cpt.h"
#include "bn.h"

using namespace std;
class CPT;

class Node
{
public:
	/* constructor */
	Node(string);

	/* define the node as evidence node */
	bool setEvidence(bool);

	/* if the node is an evidence node */
	bool isEvidence();

	/* the observed value, only available on evidence node */
	bool setValue(string);

	/* set the node name */
	bool setName(string);

	/* return the node name */
	string getName();

	/* the observed value, only available on evidence node */
	string getValue();

	/* the corresponding cpt */
	CPT* getCPT();

	/* set the corresponding cpt */
	bool setCPT(CPT);

	/* add a node to parent list */
	bool addParent(Node*);

	/* remove a node from parent list */
	bool removeParent(Node*);

	/* parent list */
	map<string, Node*> getParents();

	/* add a node to child list */
	bool addChild(Node*);

	/* remove a node to child list */
	bool removeChild(Node*);

	/* child list */
	map<string, Node*> getChildren();

private:

	/* parent name, parent pointer */
	map<string, Node*> _parents;

	/* child name, child pointer */
	map<string, Node*> _children;

	/* if the node is an evidence node */
	bool _is_evidence = false;

	/* node value */
	string _value;

	/* node name */
	string _name;

	/**/
	CPT *_cpt;
};

#endif /* EXACT_PE_NODE_H_ */
