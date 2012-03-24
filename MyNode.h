
#include <iostream>
#include<fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <map>
#include <boost/shared_ptr.hpp>

using namespace std;

class MyNode {
public:
	char letter;
	boost::shared_ptr<MyNode> next;
	boost::shared_ptr<MyNode> down;
	int count;


	MyNode(char c);
	MyNode();
	~MyNode();

	int size() const;
	void printLeft(MyNode node);
	void printDown(boost::shared_ptr<MyNode> down, string s);
	boost::shared_ptr<MyNode> insertLevel(char c, boost::shared_ptr<MyNode> node);
	boost::shared_ptr<MyNode> parse(boost::shared_ptr<MyNode> header, string line);
	
	void startLevenshtein(boost::shared_ptr<MyNode> node, string target, int errorCost);
	void searchRecursive(boost::shared_ptr<MyNode> node, string target, char letter,int previousRow[], int errorCost, string builtWord);
};

