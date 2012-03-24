//============================================================================
// Name        : MyNode.cpp
// Author      : Dzidorius Martinaitis 
// Version     :
// Copyright   : MIT
// Description : Simple implementation of a tree
//============================================================================

#include<iostream>
#include<fstream>
#include <string>
#include "MyNode.h"

using namespace std;
	MyNode::MyNode(char c) {
		this->letter = c;
		down.reset();// = NULL;
		next.reset();// = NULL;
		count = 0;
	}
	MyNode::MyNode() {
		down.reset();// = NULL;
		next.reset();// = NULL;
		count++;
	}
	MyNode::~MyNode()
	{
	}

        void MyNode::startLevenshtein(boost::shared_ptr<MyNode> node, string target, int errorCost)
        {
		int column [target.size()+1];
		string builtWord;
		for(int i=0;i<target.size()+1;i++)
		{
			column[i]=i;
		}
		char letter =node->letter;
		node->searchRecursive(node, target, letter, column, errorCost, builtWord);
	}

	void MyNode::searchRecursive(boost::shared_ptr<MyNode> node, string target, char letter,int previousRow[], int errorCost, string builtWord)
	{
		bool topLevel = true;
                for(int i=0;i<target.size()+1;i++)
                {
	               if(previousRow[i] !=i)
		               topLevel = false;
		}
		if(topLevel)
			builtWord = "";

		int currentRow [target.size()+1];
		currentRow[0]=previousRow[0]+1;
                int minCurrentRow = currentRow[0];
                for(int i=1;i<target.size()+1;i++)
                {
			int insertCost = currentRow[i-1]+1;
                        int deleteCost = previousRow[i]+1;
                        int replaceCost = 0;
                        if(target[i-1]!=letter)
	                        replaceCost = previousRow[i-1]+1;
	                else
		                replaceCost = previousRow[i-1];
		        currentRow[i] = min(replaceCost,min(insertCost,deleteCost));
		        if(minCurrentRow>currentRow[i])
			        minCurrentRow = currentRow[i];

		}
		if(currentRow[target.size()]<=errorCost && node->count !=0)
			cout<<"item: "<<builtWord+letter<<endl;
		if(minCurrentRow<errorCost)
                {
                        string copyBuiltWord=builtWord+letter;
			if(node->down!=0)
				node->searchRecursive(node->down, target, node->down->letter, currentRow, errorCost, copyBuiltWord);
			if(node->next !=0)
			        node->searchRecursive(node->next, target, node->next->letter, previousRow, errorCost, builtWord);

		}
	}

	int MyNode::size() const {
		return count;
	}

	/*
	 * Function which prints words and counter
	 */
	void MyNode::printDown(boost::shared_ptr<MyNode> down, string s)
	{
		if(down->next !=0)
		{
			down->printDown(down->next, s);
		}

		char test = down->letter;
		s+=down->letter;

		if(down->count!=0 && down->down != 0)
		{
			cout<<s<<endl;//" "<<down->count<<endl;
		}

		if(down->down != 0)
		{
			down->printDown(down->down, s);
		}

		else
		{
			cout<<s<<endl;//" "<<down->count<<endl;
		}

	}


	boost::shared_ptr<MyNode>  MyNode::insertLevel(char c, boost::shared_ptr<MyNode> node) {
		if (node->next != 0)
		{
			while (node->next != 0)
			{
				if (node->next->letter != c)
				{
					node = node->next;
				} else {
					node = node->next;
					return node;
				}

			}
		}
		boost::shared_ptr<MyNode> n (new MyNode(c));
		node->next = n;
		node = n;
		return node;

	}

	boost::shared_ptr<MyNode> MyNode::parse(boost::shared_ptr<MyNode> header, string line)
	{
		boost::shared_ptr<MyNode> n = header;
		for (int y = 0; y < line.size(); y++)
		{
			if (n->down != NULL)
			{
				if(n->down->letter==line[y])
				{
					n=n->down;
				}
				else
				{
					n=header->insertLevel(line[y], n->down);
				}
			}
			else
			{
				boost::shared_ptr<MyNode> h(new MyNode(line[y]));
				h->letter = line[y];

				n->down = h;
				n = h;
			}
		}
		return n;
	}

