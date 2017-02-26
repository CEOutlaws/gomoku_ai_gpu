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

	karthyNodeData_t* create_node(int x, int y, int type, double Q);
	karthynode_t* create_karthy(int x, int y, int type, double Q);
	void add2list(list<karthynode_t> *&tree, int x, int y, int type, double Q);
	void addnewlist(list<karthynode_t> *& tree);
	void add2list(list<karthynode_t> *& tree, karthynode_t *& newlist);
	void delete_list(list<karthynode_t> *& tree, list<karthynode_t> *& newlist);
};
#endif