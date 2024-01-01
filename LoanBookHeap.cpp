#include "LoanBookHeap.h"
#include <queue>
#include <algorithm>
using namespace std;

void LoanBookHeap::heapifyUp(LoanBookHeapNode* pN) {

    //If currentNode is bigger than parent node then swap
    while(pN->getParent() != NULL && pN->getBookData()->getName() < pN->getParent()->getBookData()->getName()){
        LoanBookData* temp = pN->getBookData();     //Get Book data
        pN->setBookData(pN->getParent()->getBookData());    //Put data into Heap Node
        pN->getParent()->setBookData(temp);         //Then Swap with parent data
        pN = pN->getParent();   //Move to parent Heap
    }

    if(pN->getParent() == NULL){    //If we reach the root
        root = pN;
    }

}

void LoanBookHeap::heapifyDown(LoanBookHeapNode* pN) {  
    while(pN != NULL){
        LoanBookHeapNode* maxChild = NULL;
        LoanBookHeapNode* leftChild = pN->getLeftChild();    //Get Left Child
        LoanBookHeapNode* rightChild = pN->getRightChild();     //Get Right Child

        if(leftChild == NULL && rightChild == NULL){    //If data doesn't exists
            maxChild = NULL;        //There is no data to compare
        }else if(leftChild != NULL && rightChild == NULL){      //If  left Child exists
            maxChild = leftChild;       //Set max Child as left Child
        }else if(leftChild == NULL && rightChild != NULL){      //If   rightChild exists
            maxChild = rightChild;      //Set max Child as right Child
        }else{
            if(leftChild->getBookData()->getName() > rightChild->getBookData()->getName()){     //Compare left Child and right Child
                maxChild = leftChild;       //max Child = left Child
            }else{
                maxChild = rightChild;      //max child = right Child
            }
        }

        if(maxChild != NULL && pN->getBookData()->getName() < maxChild->getBookData()->getName()){      //If max child exists, and compare with parent node value and compare
            LoanBookData* temp = pN->getBookData();  //set data
            pN->setBookData(maxChild->getBookData());
            maxChild->setBookData(temp);
            pN = maxChild;
        }else{
            break;
        }
    }
}

bool LoanBookHeap::Insert(LoanBookData* data) {
    if(root == NULL){       //If heap is empty
        LoanBookHeapNode* newNode = new LoanBookHeapNode();    //Create new LoanBookHeapNode
        newNode->setBookData(data);
        setRoot(newNode);
        return true;
    }

    LoanBookHeapNode* parent = NULL;    
    queue<LoanBookHeapNode*> queues;            //Using queue to get location where to put new Queue 
    queues.push(root);   

    while(!queues.empty()){    //Store in queue 
        parent = queues.front();
        queues.pop();

        if(parent->getLeftChild() == NULL || parent->getRightChild() == NULL){
            break;
        }
        if(parent->getLeftChild() != NULL){
            queues.push(parent->getLeftChild());
        }
        if(parent->getRightChild() != NULL){
            queues.push(parent->getRightChild());
        }
    }

    LoanBookHeapNode* newNode = new LoanBookHeapNode();         //Then create new LoanBookHeapNode
    newNode->setBookData(data);     //Set Data
    newNode->setParent(parent);     //Set Parent
    
    if(parent->getLeftChild() == NULL){        //If left Child exists
        parent->setLeftChild(newNode);      //Push data into left child
    }else{          //If right Child exists
        parent->setRightChild(newNode);     //Push data into right child
    }

    heapifyUp(newNode);     //Then call heapifyup to sort
    return true;
}

bool LoanBookHeap::Pop(){
    if(root == NULL){       //If Heap is empty
        return false;
    }

    LoanBookHeapNode* removedNode = root;       //Remove from root
    queue<LoanBookHeapNode*> queues;           //Store LoanBookHeapNode into queue
    queues.push(removedNode);
    LoanBookHeapNode* lastNode = NULL;
    while(!queues.empty()){
        lastNode = queues.front();
        queues.pop();

        if(lastNode->getLeftChild() != NULL){
            queues.push(lastNode->getLeftChild());
        }
        if(lastNode->getRightChild() != NULL){
            queues.push(lastNode->getRightChild());
        }
    }

    //find last node and move to root node
    if(lastNode != root){
        root->setBookData(lastNode->getBookData());
        LoanBookHeapNode* parent = lastNode->getParent();
        if(parent->getLeftChild() == lastNode){
            parent->setLeftChild(NULL);
        }else{
            parent->setRightChild(NULL);
        }
        delete lastNode;
    }else{
        //if last node and root is same, left node is one, so delete root node
        delete root;
        root = NULL;
    }

    if(root != NULL){       //Call HeapifyDown function to sort
        heapifyDown(root);
    }
    return true;
}

