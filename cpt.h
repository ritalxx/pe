/*
 * cpt.h
 *
 *  Created on: 2014年11月21日
 *      Author: xiaoxuliu
 */

#ifndef EXACT_PE_CPT_H_
#define EXACT_PE_CPT_H_

#include <iostream>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>
#include "node.h"
#include "bn.h"

using namespace std;
class Node;

class CPT
{
public:

	/* constructor */
	CPT();

	/* instantiate cpt with given variable and value */
	CPT instantiate(Node*, string);

	/* query the table */
	double getValue(map<string, string>);

	/* write the table */
	bool setValue(pair<string, double>);

	/* push node to the table */
	bool addNode(Node*);

	/* all nodes */
	vector<Node*> getNodes();

	/* multiply another cpt and return the result cpt */
	CPT multiply(CPT);

	/* for debug */
	void printOut();

	/* sum out */
	static CPT* sumOut(vector<CPT*>, Node*);

	/* the table */
	unordered_map<string, double> _table;

	/* suming out the last variable in cpt results in a double value rather than a cpt */
	bool _fake;

private:
	/* the order of nodes is correspond to the table */
	vector<Node*> _nodes;
};

#endif /* EXACT_PE_CPT_H_ */
