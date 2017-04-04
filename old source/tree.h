#pragma once
#ifndef TREE_H
#define TREE_H
#include <iostream>
#include <list>
#include "tree.h"

using namespace std;

struct karthyNodeData_t
{
	char x;
	char y;
	int type;
	double Q;
};


struct karthynode_t
{
	karthyNodeData_t *data;
	list<karthynode_t> *childList;
	list<karthynode_t> *parentList;

};

struct karthyTree_t
{
	karthynode_t* root;
};

class karthy
{
public:
	karthy()
	{
		karthyTree_t *root = NULL;
	}

	karthyNodeData_t* create_node(int x, int y, int type, double Q); //create a new data
	karthynode_t* create_karthy(int x, int y, int type, double Q); // create a new karthy node 
	void add2list(list<karthynode_t> *&tree, int x, int y, int type, double Q);// add new karthynode to list with input is data
	void add2list(list<karthynode_t> *& tree, karthynode_t *& newlist); // add new karthynode to list with input karthynode
	void addnewlist(list<karthynode_t> *& tree);// add new list child with no data
	void delete_list(list<karthynode_t> *& tree, list<karthynode_t> *& newlist);// detele brand from newlist with tree is father of newlist
};
#endif