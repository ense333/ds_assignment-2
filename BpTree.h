#ifndef _BpTree_H_
#define _BpTree_H_

#include "BpTreeDataNode.h"
#include "BpTreeIndexNode.h"
#include "LoanBookData.h"
#include <fstream>
#include <iostream>
using namespace std;

class BpTree {
private:
	BpTreeNode* root;
	int			order;		// m children
	ofstream* fout;
public:
	BpTree(ofstream *fout, int order = 3) {
		root = NULL;
		this->order = order;
		this->fout = fout;
	}
	~BpTree(){
		
	}
	/* essential */
	bool		Insert(LoanBookData* newData);
	bool		excessDataNode(BpTreeNode* pDataNode);
	bool		excessIndexNode(BpTreeNode* pIndexNode);
	void		splitDataNode(BpTreeNode* pDataNode);
	void		splitIndexNode(BpTreeNode* pIndexNode);
	BpTreeNode* getRoot() { return root; }
	BpTreeNode* searchDataNode(string name);
	
	LoanBookData* Delete(string key);
	void		balanceDataNode(BpTreeNode* pDataNode);
	void		redistributeFromNext(BpTreeNode* pDataNode);
	void 		redistributeFromPrev(BpTreeNode* pDataNode);
	void		mergeWithDataNode(BpTreeNode* pDataNode);
	void		mergeWithIndexNode(BpTreeNode* pIndexNode);
	void		mergeWithPrevIndexNode(BpTreeNode* pIndexNode);
	void		mergeWithNextIndexNode(BpTreeNode* pIndexNode);
	void		balanceIndexNode(BpTreeNode* pIndexNode);

	bool printAll(ofstream *fout);
	bool searchBook(string name, ofstream *fout);
	bool searchRange(string start, string end, ofstream *fout);

};

#endif
