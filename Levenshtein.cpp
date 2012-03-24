/*
 * =====================================================================================
 *
 *       Filename:  Levenshtein.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/10/2012 10:39:05 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Dzidorius Martinaitis (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <iostream>
#include<fstream>
#include<string.h>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/algorithm/string.hpp>
#include "MyNode.h"
#include <time.h>
#include <sys/time.h>

using namespace std;

/* 
 * Full implementation of Levenshtein algorithm 
 */
int levenshtein_distance(std::string target, std::string source, int kMaxCost)
{
	int distance = 0;
	boost::numeric::ublas::matrix<int> m(source.length()+1,target.length()+1);
	for(int i=0;i<source.length()+1;i++)
	{
		m(i,0)=i;
	}
	for(int y=0;y<target.length()+1;y++)
	{
		m(0,y)=y;
	}
	
	for(int i=0;i<source.length();i++)
        {
		int cost=0;
		for(int y=0;y<target.length();y++)
		{
			
			int cell_cost =1;
			if(target[y]==source[i])
			{
				cell_cost=0;
			}
			int min_cost = m(i,y);
			if(min_cost>m(i,y+1))
				min_cost=m(i,y+1);
			if(min_cost>m(i+1,y))
				min_cost=m(i+1,y);
			cell_cost+=min_cost;
			m(i+1,y+1)=cell_cost;
			if(y==0)
				cost=cell_cost;
			else
			{
				if(cell_cost<cost)
					cost=cell_cost;
			}
		}	
		if(kMaxCost<cost)
			return kMaxCost;
	}	
	return (int)(m(source.length(),target.length()));
}


/*
 * Func. is used fo code profiling, when the term is scanned through whole dictonary 
 * */
void testDict()
{
	std::ifstream myfile("only_terms.csv2");
	if (myfile.is_open())
		while (myfile.good()) {
			std::string target="";
			getline(myfile, target);
			std::ifstream dict("only_terms.csv");
			timespec ts;
  			clock_gettime(CLOCK_REALTIME, &ts);
			int error_rate=target.length()/4+1;
			while (dict.good()) {
				std::string line="";
	        		getline(dict, line);
				int dist = levenshtein_distance(target, line, error_rate);

			}
			timespec end;
			clock_gettime(CLOCK_REALTIME, &end);
			double result =0;
			if(end.tv_nsec-ts.tv_nsec<0)
				result=(double)(end.tv_nsec-ts.tv_nsec+1000000000)/1000000000;
			else
				result=(double)(end.tv_nsec-ts.tv_nsec)/1000000000;
       			cout<<target<<";"<<(double)(end.tv_sec-ts.tv_sec)+result<<endl;
				
			dict.close();

		}
}

void loadDictIntoTrie(boost::shared_ptr<MyNode> header)
{
	std::ifstream myfile("only_terms.csv");

	if (myfile.is_open()) 
		while (myfile.good()) {
			std::string line="";
			getline(myfile, line);
			boost::algorithm::to_lower(line);
			boost::shared_ptr<MyNode> n = header->parse(header,line);
			n->count++;

		}
	myfile.close();

}

/*
 * This func. is used for code profiling to determine time necessary to retrieve the term from the tree. 
 */
void testNode(boost::shared_ptr<MyNode> header)
{
	std::ifstream dict("only_terms.csv");
  
	while (dict.good()) {
		std::string line="";
	        getline(dict, line);
		string target = line;
		boost::algorithm::to_lower(target);
		int error_rate=target.length()/4+1;
		if(error_rate>5)
			error_rate = 5;
		timespec ts;
  		clock_gettime(CLOCK_REALTIME, &ts);
		boost::shared_ptr<MyNode> n = header->down;

			
		header->startLevenshtein(n, target, error_rate);
		timespec end;
		clock_gettime(CLOCK_REALTIME, &end);
		double result =0;
		if(end.tv_nsec-ts.tv_nsec<0)
			result=(double)(end.tv_nsec-ts.tv_nsec+1000000000)/1000000000;
		else
			result=(double)(end.tv_nsec-ts.tv_nsec)/1000000000;
	        cout<<(double)(end.tv_sec-ts.tv_sec)+result<<";"<<line<<endl;
	}
}

/* 
 *Reads the input from console and runs the query against the tree
 */
int runLookup(int argc, const char* argv[], boost::shared_ptr<MyNode> header)
{
	std::string target="";
	cout<<"Please enter the term: "<<endl;
	while(1>0)
	{
		getline(cin, target);
		if(target.compare("exit")==0)
			return 0;
		boost::algorithm::to_lower(target);
		int error_rate=target.length()/4+1;
		if(error_rate>5)
			error_rate = 5;
		timespec ts;
  		clock_gettime(CLOCK_REALTIME, &ts);
		boost::shared_ptr<MyNode> n = header->down;

		
		header->startLevenshtein(n, target, error_rate);
		timespec end;
		clock_gettime(CLOCK_REALTIME, &end);
		double result =0;
		if(end.tv_nsec-ts.tv_nsec<0)
			result=(double)(end.tv_nsec-ts.tv_nsec+1000000000)/1000000000;
		else
			result=(double)(end.tv_nsec-ts.tv_nsec)/1000000000;
        	cout<<(double)(end.tv_sec-ts.tv_sec)+result<<endl;
	}

}

int main(int argc, const char* argv[])
{
	boost::shared_ptr<MyNode> header(new MyNode);
	loadDictIntoTrie( header);
//	header->printDown(header->down,"");
	//return runLookup(argc, argv, header);
	//testDict();
	//testNode(header);
	return 0;
return 0;
}
