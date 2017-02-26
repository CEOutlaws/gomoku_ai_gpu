/*
* C++ Program To Implement BST
*/
#define SIZE 4

# include <iostream>
# include <cstdlib>
#include<cstdio>
#include <time.h>
#include "linklist.h"
#include "tree.h"
#include "CommonTools.h"

using namespace std;

karthy *kar;

bool check_caro_board(list<karthynode_t> *root,int x,int y) 
{
	list<karthynode_t>::iterator it;
	for (it = root->begin(); it != root->end(); ++it) 
	{
		if (it->data->x == x && it->data->y ==y) 
		{
			return 0;
		}
	}
	return 1;
}
void one_step_future(list<karthynode_t> *root, int &count)
{
	if (count >= 10) return;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (check_caro_board(root,i,j)==true)
			{
				kar->add2list(root, i, j, 1, 1);
				if (count < 10) one_step_future(root->begin()->childList, ++count);
			}
		}
	}
	--count;
}
void future_move(list<karthynode_t> *root) 
{
	int count = 0;
	if (root->begin()->childList == NULL) 
	{
		one_step_future(root, count);
	}
}

int main() {
	clock_t start_t, end_t;
	
	cout << "begin" << endl;
	//karthy *kar;
	kar = new karthy();
	list<karthynode_t> *root, *head;
	root = new list<karthynode_t>();
	karthynode_t* state0;
	state0 = kar->create_karthy(0, 0, 0, 0);
	state0->parentList = NULL;
	state0->childList = NULL;
	root->push_back(*state0);
	delete state0;

	
	head = root;
	start_t = clock();
	future_move(root);
	end_t = clock();
	kar->delete_list(root, root->begin()->childList);
	delete root->begin()->data;
	root->erase(root->begin());
	delete root;
	delete kar;
	system("pause");
	return 0;
}