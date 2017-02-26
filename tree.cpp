#include "tree.h"

karthyNodeData_t* karthy::create_node(int x, int y, int type, double Q)
{
	karthyNodeData_t *data;
	data = new karthyNodeData_t();
	data->x = x;
	data->y = y;
	data->type = type;
	data->Q = Q;
	return data;
}
karthynode_t* karthy::create_karthy(int x, int y, int type, double Q)
{
	karthyNodeData_t *data;
	data = new karthyNodeData_t();
	data->x = x;
	data->y = y;
	data->type = type;
	data->Q = Q;

	karthynode_t* newnode = new karthynode_t();
	newnode->data = data;
	newnode->childList = NULL;
	newnode->parentList = NULL;
	return newnode;
}
void karthy::addnewlist(list<karthynode_t> *&tree)
{
	list<karthynode_t> * temp=new list<karthynode_t>();
	tree->begin()->childList = temp;
}
void karthy::add2list(list<karthynode_t> *& tree, karthynode_t *& newlist) //tree is list parent, newlist is node of childlist
{
	if (tree->begin()->childList == NULL) tree->begin()->childList = new list<karthynode_t>;
	newlist->parentList = tree;
	tree->begin()->childList->push_back(*newlist);
}
void karthy::add2list(list<karthynode_t> *&tree, int x, int y, int type, double Q)
{
	if (tree->begin()->childList == NULL) tree->begin()->childList = new list<karthynode_t>;
	karthynode_t* temp;
	temp = create_karthy(x, y, type, Q);
	temp->parentList = tree;
	temp->childList = NULL;
	tree->begin()->childList->push_back(*temp);
	delete temp;
}
void delete_karthynode(list<karthynode_t> *&parent, list<karthynode_t> *&node)
{
	if (node->begin()->childList != NULL)
	{
		delete_karthynode(node, node->begin()->childList);
	}
	if (node->empty() == false)
	{
		delete node->begin()->data;
		node->erase(node->begin());
		if (node->empty() == true) {
			delete node;
			parent->begin()->childList = NULL;
			return;
		}
		delete_karthynode(parent, node);
	}
}
void karthy::delete_list(list<karthynode_t> *&tree, list<karthynode_t> *&node_detete) //tree is list parent, node_delete is node of childlist
{
	delete_karthynode(tree, node_detete);
}