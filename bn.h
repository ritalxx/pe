
#ifndef EXACT_PE_BN_H_
#define EXACT_PE_BN_H_

#include <iostream>
#include <string>
#include <vector>
#include "node.h"
#include "cpt.h"

using namespace std;
class CPT;
class Node;
extern double pe_result;
extern bool debug;
extern int max_cpt;

inline string trim_right_copy(
  const string& s,
  const string& delimiters = " \f\n\r\t\v" )
{
  if(!s.length()) return "";
  return s.substr( 0, s.find_last_not_of( delimiters ) + 1 );
}

inline string trim_left_copy(
  const string& s,
  const string& delimiters = " \f\n\r\t\v" )
{
  if(!s.length()) return "";
  return s.substr( s.find_first_not_of( delimiters ) );
}

inline string trim_copy(
  const string& s,
  const string& delimiters = " \f\n\r\t\v" )
{
  return trim_left_copy( trim_right_copy( s, delimiters ), delimiters );
}

inline string dec_to_bin(int number,int parent_count)
{
	if(parent_count <= 0) return "";
	string leading_zero = "";
	for(int i = 0; i < parent_count - 1; i++){
		leading_zero += "0";
	}
    if ( number == 0 ) return leading_zero + "0";
    if ( number == 1 ) return leading_zero + "1";

    if ( number % 2 == 0 )
        return dec_to_bin(number / 2, parent_count - 1) + "0";
    else
        return dec_to_bin(number / 2, parent_count - 1) + "1";
}

class BayesianNetwork
{
public:

	/* constructor */
	BayesianNetwork();

	/* TODO */
	bool removeNonEvidenceLeaves();

	/* read data from file */
	bool parseXbif(string);

	/* search all cpts containing the given node */
	vector<CPT*> searchCPT(Node*);

	/* add node to the network */
	bool addNode(Node);

	/* remove node from the network */
	bool removeNode(Node*);

	/* search node by name */
	Node* searchNode(string);

	/* all nodes */
	map<string, Node*> getNodes();

	/* add cpt to the network */
	bool addCPT(CPT*);

	/* remove cpt from the network */
	bool removeCPT(CPT*);

	/* all cpts */
	vector<CPT*> getCPTs();

	/* min-fill heuristic */
	Node* nextNodeByMinFill();

	/* min-degree heuristic */
	Node* nextNodeByMinDegree();

private:

	/* all nodes */
	map<string, Node*> _nodes;

	/* all cpts */
	vector<CPT*> _cpts;
};

#endif
