#include "SelectionTree.h"
#include <fstream>
using namespace std;

bool SelectionTree::Insert(LoanBookData* newData) {

    SelectionTreeNode* node = NULL;
    if(newData->getCode() == 0){       //in order to go run[0]
        node = root->getLeftChild()->getLeftChild()->getLeftChild();  //go to most left child
        node->getHeap()->Insert(newData);       //insert data into min heap
        LoanBookHeapNode* topValue = node->getHeap()->getRoot();  //then get heap's top value
        node->setBookData(topValue->getBookData());     
        
    }else if(newData->getCode() == 100){        //in order to go run[1]
        node = root->getLeftChild()->getLeftChild()->getRightChild();
        node->getHeap()->Insert(newData);       //insert data into min heap
        LoanBookHeapNode* topValue = node->getHeap()->getRoot();    //then get heap's top value
        node->setBookData(topValue->getBookData());
        
    }else if(newData->getCode() == 200){        //in order to go run[2]
        node = root->getLeftChild()->getRightChild()->getLeftChild();
        node->getHeap()->Insert(newData);       //insert data into min heap
        LoanBookHeapNode* topValue = node->getHeap()->getRoot();    //then get heap's top value
        node->setBookData(topValue->getBookData());
       
    }else if(newData->getCode() == 300){        //in order to go run[3]
        node = root->getLeftChild()->getRightChild()->getRightChild();
        node->getHeap()->Insert(newData);       //insert data into min heap
        LoanBookHeapNode* topValue = node->getHeap()->getRoot();    //then get heap's top value
        node->setBookData(topValue->getBookData());
        
    }else if(newData->getCode() == 400){        //in order to go run[4]
        node = root->getRightChild()->getLeftChild()->getLeftChild();
        node->getHeap()->Insert(newData);       //insert data into min heap
        LoanBookHeapNode* topValue = node->getHeap()->getRoot();    //then get heap's top value
        node->setBookData(topValue->getBookData());

    }else if(newData->getCode() == 500){        //in order to go run[5]
        node = root->getRightChild()->getLeftChild()->getRightChild();
        node->getHeap()->Insert(newData);       //insert data into min heap
        LoanBookHeapNode* topValue = node->getHeap()->getRoot();    //then get heap's top value
        node->setBookData(topValue->getBookData());

        node->setBookData(newData);
    }else if(newData->getCode() == 600){        //in order to go run[6]
        node = root->getRightChild()->getRightChild()->getLeftChild();
        node->getHeap()->Insert(newData);       //insert data into min heap
        LoanBookHeapNode* topValue = node->getHeap()->getRoot();    //then get heap's top value
        node->setBookData(topValue->getBookData());

    }else if(newData->getCode() == 700){        //in order to go run[7]
        node = root->getRightChild()->getRightChild()->getRightChild();
        node->getHeap()->Insert(newData);       //insert data into min heap
        LoanBookHeapNode* topValue = node->getHeap()->getRoot();    //then get heap's top value
        node->setBookData(topValue->getBookData());
        
    }else{
        //can't find run
        return false;
    }
    if(node == NULL){
        //can't find run
        return false;
    }   
    updateTreeFromNode(node);   //then update selection tree with new run's value
    return true;
}

bool SelectionTree::Delete() {
    if(root == NULL){       //if selection tree doesn't have data
        return false;
    }
    int code = root->getBookData()->getCode();    //get book code of root node
    SelectionTreeNode* node = NULL;
    if(code == 0){  //then find correct heap with book code
        node = root->getLeftChild()->getLeftChild()->getLeftChild();
        
    }else if(code == 100){
        node = root->getLeftChild()->getLeftChild()->getRightChild();

    }else if(code == 200){
        node = root->getLeftChild()->getRightChild()->getLeftChild();

    }else if(code == 300){
        node = root->getLeftChild()->getRightChild()->getRightChild();

    }else if(code == 400){
        node = root->getRightChild()->getLeftChild()->getLeftChild();

    }else if(code == 500){
        node = root->getRightChild()->getLeftChild()->getRightChild();

    }else if(code == 600){
        node = root->getRightChild()->getRightChild()->getLeftChild();

    }else if(code == 700){
        node = root->getRightChild()->getRightChild()->getRightChild();

    }
    if(node == NULL){   //if correct node not found
        return false;
    }
    LoanBookHeap* eachHeap = node->getHeap();   //get run data
    if(!eachHeap->Pop()){
        return false;  //failed to Pop
    }
    if(eachHeap->getRoot() != NULL){
        node->setBookData(eachHeap->getRoot()->getBookData());   //then we need to set leaf node's new value
    }
    updateTreeFromNode(node);   //update selection tree
    return true;
}

bool SelectionTree::printBookData(int bookCode) {
    SelectionTreeNode* node = NULL;
    if(node == NULL){
        return false;  //can't find code's book
    }

    LoanBookData* bookData = node->getBookData();
    if(bookData != NULL){
        cout << bookData->getName() << endl;
        return true;
    }
    return false;
}

void SelectionTree::updateTreeFromNode(SelectionTreeNode* node){
    while(node != NULL){                //until node has data
        SelectionTreeNode* parent = node->getParent();          //get parent node

        if(parent == NULL){     //if parent node doesn't exists
            break;
        }

        LoanBookData* leftValue = NULL;
        LoanBookData* rightValue = NULL;
        if(parent->getLeftChild()){     //if leftchild exists
            leftValue = parent->getLeftChild()->getBookData();      //get leftvalue
        }
        if(parent->getRightChild()){       //if right child exists
            rightValue = parent->getRightChild()->getBookData();       //get rightValue
        }
        if(leftValue && (!rightValue || leftValue->getName() < rightValue->getName())){   //then we compare left and right value
            parent->setBookData(leftValue);
        }else if(rightValue){
            parent->setBookData(rightValue);
        }
        node = parent;      //going up the parent
    }
}

int SelectionTree::printHeap(ofstream *fout, string code){

    SelectionTreeNode* newT = new SelectionTreeNode();
    SelectionTreeNode* node = NULL;
    if(root == NULL){    //if selection tree has no data
        delete newT;
        return 500;     //return error code
    }
    if(code == "000"){      //find correct heap with given input
        node = root->getLeftChild()->getLeftChild()->getLeftChild();
        
    }else if(code == "100"){
        node = root->getLeftChild()->getLeftChild()->getRightChild();
        
    }else if(code == "200"){
        node = root->getLeftChild()->getRightChild()->getLeftChild();
        
    }else if(code == "300"){
        node = root->getLeftChild()->getRightChild()->getRightChild();
        
    }else if(code == "400"){
        node = root->getRightChild()->getLeftChild()->getLeftChild();
        
    }else if(code == "500"){
        node = root->getRightChild()->getLeftChild()->getRightChild();
        
    }else if(code == "600"){
        node = root->getRightChild()->getRightChild()->getLeftChild();
        
    }else if(code == "700"){
        node = root->getRightChild()->getRightChild()->getRightChild();
        
    }else{    //if wrong book code given
        return 500;
    }


    if(node->getHeap()->getRoot() == NULL){   //if heap doesn't have data
        delete newT;
        return 500;
    }

    LoanBookHeapNode* copiedRoot = newT->deepCopy(node->getHeap()->getRoot());    //using deepCopy function to copy Min heap root
    LoanBookHeap* copiedMinHeap = new LoanBookHeap();   //create new min heap
    copiedMinHeap->setRoot(copiedRoot);   //then set it's root value

    *fout << "========PRINT_ST========" << endl;
    while(copiedMinHeap->getRoot() != NULL){     //until heap has data
        LoanBookData* topData = copiedMinHeap->getRoot()->getBookData();    //get book data
        *fout << topData->getName() << "/";
        if(topData->getCode() == 0){
            *fout << "000/";
        }else{
            *fout << topData->getCode() << "/";
        }
        *fout << topData->getAuthor() << "/" << topData->getYear() << "/" << topData->getLoanCount() << endl;
        copiedMinHeap->Pop();
    }
    *fout << "========================" << endl << endl;
    delete newT;        //delete instances
    delete copiedMinHeap;
    return 1;
}

void SelectionTree::deleteTree(SelectionTreeNode* node){
    if (node != nullptr) {  // Recursively delete both subtrees
        deleteTree(node->getLeftChild());       //move to leftChild
        deleteTree(node->getRightChild());      //move to rightChild
        delete node;    // Finished deleting children, delete this node
    }
}
