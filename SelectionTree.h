#pragma once
#include "SelectionTreeNode.h"

#include <fstream>
#include <iostream>
using namespace std;

class SelectionTree
{
private:
    SelectionTreeNode* root;
    ofstream* fout;

public:
    SelectionTree(ofstream* fout) {
        this->root = NULL;
        this->fout = fout;
    }
    ~SelectionTree() {
        deleteTree(root);
        root = nullptr;
    }

    void setRoot(SelectionTreeNode* pN) { this->root = pN; }
    SelectionTreeNode* getRoot() { return root; }

    bool Insert(LoanBookData* newData);
    bool Delete();
    bool printBookData(int bookCode);

    void updateTreeFromNode(SelectionTreeNode* node);
    int printHeap(ofstream *fout, string code);
    void deleteTree(SelectionTreeNode* node);
};