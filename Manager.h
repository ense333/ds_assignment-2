#pragma once
#include "SelectionTree.h"
#include "BpTree.h"
using namespace std;

class Manager
{
private:
	char* cmd;
	SelectionTree* selectTree;
	BpTree* bpTree;
public:
	Manager(int bpOrder) : cmd(nullptr), selectTree(nullptr), bpTree(nullptr) //constructor
	{
		selectTree;
		bpTree;
	}


	~Manager()//destructor
	{
		delete selectTree;
		delete bpTree;
	}

	ifstream fin;
	ofstream flog;
	

	void run(const char* command);
	bool LOAD(ofstream *fout, BpTree* bpT);
	bool ADD(SelectionTree* selT, LoanBookData* nData, BpTree* bpT);

	bool SEARCH_BP_BOOK(ofstream *fout, string book, BpTree* bpT);
	bool SEARCH_BP_RANGE(ofstream *fout, string s, string e, BpTree* bpT);

	bool PRINT_BP(ofstream *fout, BpTree* bpt);
	bool PRINT_ST(ofstream *fout, string code, SelectionTree* selT);

	bool DELETE(SelectionTree* selT);

	void printErrorCode(int n);
	void printSuccessCode(const char* cmd);

};

