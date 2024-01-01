#include "BpTree.h"
#include <map>
#include <fstream>
#include <algorithm>
#include <cmath>
using namespace std;

bool BpTree::Insert(LoanBookData* newData) {

    if(root == NULL){       //if value doesn't exists
        BpTreeNode* newDataNode = new BpTreeDataNode;    //create new Data Node
        newDataNode->insertDataMap(newData->getName(), newData);  //insert data into map
        root = newDataNode;   //set root
        return true;     //end function
    }
    else {       //if tree has value
        BpTreeNode* currNode = searchDataNode(newData->getName());    //find node using given input
        map<string, LoanBookData*>::iterator iter = currNode->getDataMap()->begin();    //using iterator to iterate through Data map
        for(; iter != currNode->getDataMap()->end(); iter++){     //loop Data map
            if(iter->first == newData->getName()){    //if value already exists
                iter->second->setCode(newData->getCode());      //update values
                iter->second->setAuthor(newData->getAuthor());
                iter->second->setYear(newData->getYear());
                iter->second->updateCount();            //also update count
                return true;
            }
        }
        currNode->insertDataMap(newData->getName(), newData);  //if same data doesn't exists, insert into Data Node
        if(excessDataNode(currNode)){       //if Node number exceeds in Data node
            splitDataNode(currNode);    //Need to split data Node
        }  
    }
    return true;
}

bool BpTree::excessDataNode(BpTreeNode* pDataNode) {   //in order to check number of nodes in data node
    if (pDataNode->getDataMap()->size() > order - 1) return true;//order is equal to the number of elements 
    else return false;
}

bool BpTree::excessIndexNode(BpTreeNode* pIndexNode) {  //in order to check number of nodes in index node
    if (pIndexNode->getIndexMap()->size() > order - 1)return true;//order is equal to the number of elements 
    else return false;
}

void BpTree::splitDataNode(BpTreeNode* pDataNode) {
    int tempOrder = ceil((order - 1)/2.0) + 1;       // Calculate the middle in order to split the data node
    BpTreeNode* newNode = new BpTreeDataNode;  // Create a new data node that will contain the split-off values
    map<string, LoanBookData*>::iterator iter;

    while(1){
        iter = pDataNode->getDataMap()->begin();  //set iter as current data node's first iterator
        for(int m = 1; m < tempOrder; m++){   //move to next iterator
            iter++;
        }
        if(iter == pDataNode->getDataMap()->end()){     //if we reach the end
            break;      //end loop
        }
        newNode->insertDataMap(iter->first, iter->second);   //insert new data into Map
        pDataNode->getDataMap()->erase(iter->first);    //then delete data in current data Node
    }
    newNode->setPrev(pDataNode);     //then set new Node's prev Pointer as pDataNode
    newNode->setNext(pDataNode->getNext());     //also set new Node's next pointer 
    if(pDataNode->getNext() != NULL){       //if current data node's next data node exists
        pDataNode->getNext()->setPrev(newNode);         //set prev pointer
    }
    pDataNode->setNext(newNode);    //set next pointer to new Node
    if(pDataNode->getParent() == NULL){         //if pDataNode doesn't have parent
        BpTreeNode* newiNode = new BpTreeIndexNode;     //create new node
        newiNode->insertIndexMap(newNode->getDataMap()->begin()->first, newNode);    //insert into new index
        newiNode->setMostLeftChild(pDataNode);      //set newiNode's most left child since new root has been created
        pDataNode->setParent(newiNode);     
        newNode->setParent(newiNode);
        root = newiNode;
    }else if(pDataNode->getParent() != NULL){       //id parent exists
        pDataNode->getParent()->insertIndexMap(newNode->getDataMap()->begin()->first, newNode);
        newNode->setParent(pDataNode->getParent());
        if(excessIndexNode(pDataNode->getParent())){            //if we need to split index node
            splitIndexNode(pDataNode->getParent());
        }
    }
}


void BpTree::splitIndexNode(BpTreeNode* pIndexNode) {
    int tempOrder = ceil((order - 1)/2.0) + 1;          // Calculate the middle in order to split the data node
    string key;
    BpTreeNode* newiNode = new BpTreeIndexNode;     //create new index node
    map <string, BpTreeNode*>::iterator sp, iter;
    sp = pIndexNode->getIndexMap()->begin();        //get iterator as current index node
    for(int i = 1; i < tempOrder; i++){
        sp++;           //increase iterator as tempOrder we get from above
    }
    while(1){
        iter = pIndexNode->getIndexMap()->end();  //set iter as current node's last
        iter--;
        if(iter == sp){
            key = iter->first;   //then save the book name as key
            newiNode->insertIndexMap(iter->first, iter->second);    //insert into index map
            newiNode->setMostLeftChild(newiNode->getIndexMap()->begin()->second);       //set most left child
            iter->second->setParent(newiNode);   //set parent
            pIndexNode->deleteMap(iter->first);   //then delete
            break;
        }
        newiNode->insertIndexMap(iter->first, iter->second);
        pIndexNode->deleteMap(iter->first);
    }
    if(pIndexNode->getParent() == NULL){   //if current indexnode doesnt have parrent
        BpTreeNode* newp = new BpTreeIndexNode;
        newp->setMostLeftChild(pIndexNode);
        newp->insertIndexMap(key, newiNode);
        newiNode->setParent(newp);
        pIndexNode->setParent(newp);
        root = newp;
    }
    else{   //if it has parent
        pIndexNode->getParent()->insertIndexMap(key, newiNode); //insert into index
        pIndexNode->getParent()->setMostLeftChild(pIndexNode);  //set most left child
        newiNode->setParent(pIndexNode->getParent());
        if(excessIndexNode(pIndexNode->getParent())){   //if we need to split
            splitIndexNode(pIndexNode->getParent());
        }
    }
}

BpTreeNode* BpTree::searchDataNode(string name) {
    BpTreeNode* pCur = root;
    while(1){   //move to dataNode
        if(pCur->getMostLeftChild() == NULL){
            break;
        }
        pCur = pCur->getMostLeftChild();
    }
    map<string, LoanBookData*>::iterator iter = pCur->getDataMap()->begin();  //get map's iterator
    while(1){
        //string key = iter->first;
        if(iter->first < name){
            if(pCur->getNext() == NULL || pCur->getNext()->getDataMap()->begin()->first > name){
                return pCur;
            }
            pCur = pCur->getNext();
            iter = pCur->getDataMap()->begin();
        }
        else{   //key == name
            return pCur;
        }
    }
    return NULL;
}

bool BpTree::printAll(ofstream *fout) {
    int check = 0;
    BpTreeNode* currNode = getRoot();
    while(1){       //Move to Data node
        if(currNode->getMostLeftChild() == NULL){
            break;
        }
        currNode = currNode->getMostLeftChild();
    }
    while(currNode){        //If data exists
        map<string, LoanBookData*>::iterator iter = currNode->getDataMap()->begin();       //Get current node's iterator
        for(iter; iter != currNode->getDataMap()->end(); iter++){   //Then print all Data in Data Map
            check++;
            if(check == 1){
                *fout << "========PRINT_BP========" << endl;
            }
            *fout << iter->second->getName() << "/";
            if(iter->second->getCode() == 0){
                *fout << "000" << "/";
            }else{
                *fout << iter->second->getCode() << "/";
            }
            *fout << iter->second->getAuthor() << "/" << iter->second->getYear() << "/" << iter->second->getLoanCount() << endl;
        }
        currNode = currNode->getNext();
    }   
    if(check == 0){
        cout << "Couldn't find\n";
        return false;
    }else{
        *fout << "========================" << endl << endl;
        return true;
    }
}

bool BpTree::searchBook(string name, ofstream *fout){
    BpTreeNode* currNode = getRoot();           //get root
    bool found = false;
    int count = 0;
    while(currNode != NULL && currNode->getMostLeftChild() != NULL){   //move to data node
        currNode = currNode->getMostLeftChild();
    }

    while(currNode != NULL && !found){          //start searching from data node
        map<string, LoanBookData*>::iterator iter = currNode->getDataMap()->begin();

        while(iter != currNode->getDataMap()->end()){
            if(iter->first == name){    //If we found the book
                count += 1;         //in order to print once
                found = true;
                LoanBookData* resultData = iter->second;
                if(count == 1){
                    *fout << "========SEARCH_BP========" << endl;
                }
                *fout << resultData->getName() << "/";
                if(resultData->getCode() == 0){
                    *fout << "000" << "/";
                }else{
                    *fout << resultData->getCode() << "/";
                }
                *fout << resultData->getAuthor() << "/" << resultData->getYear() << "/" << resultData->getLoanCount() << "\n";
                break; 
            }
            iter++;         //increment iterator to move to next data
        }
        currNode = currNode->getNext();   //move to next data node
    }
    if(!found){
        cout << "not found.\n";
    }
    *fout << "=========================" << endl << endl;
    return found;
}


bool BpTree::searchRange(string start, string end, ofstream *fout){
    BpTreeNode* currNode = getRoot();
    int count = 0;
    while(currNode != NULL && currNode->getMostLeftChild() != NULL){   //move to data node
        currNode = currNode->getMostLeftChild();
    }

    bool found = false;
    while(currNode != NULL){    //start searching from data node
        map<string, LoanBookData*>::iterator iter = currNode->getDataMap()->begin();

        while(iter != currNode->getDataMap()->end()){   //print data only within range
            if(iter->first >= start && (iter->first.compare(0, end.size(), end) <= 0)){   //since we need to include end string
                count += 1;
                if(count == 1){
                    *fout << "========SEARCH_BP========" << endl;
                }
                found = true;
                LoanBookData* resultData = iter->second;
                *fout << resultData->getName() << "/";
                if(resultData->getCode() == 0){  //since 000 recognized as 0
                    *fout << "000" << "/";
                }else{
                    *fout << resultData->getCode() << "/";
                }   
                *fout << resultData->getAuthor() << "/" << resultData->getYear() << "/" << resultData->getLoanCount() << "\n";
            }
            iter++;
        }
        currNode = currNode->getNext(); //move to next node
    }

    if(!found){
        cout << "No results found in the specified range.\n";
    }
    *fout << "=========================" << endl << endl;
    return found;
}


LoanBookData* BpTree::Delete(string key){
    BpTreeNode* dataNode = searchDataNode(key);  //first find the data node that contains key
    if (dataNode == NULL) {  //if we can't find data node
        cout << "Key not found\n";
        return NULL;
    }

    map<string, LoanBookData*>::iterator dataIter = dataNode->getDataMap()->find(key);

    if (dataIter == dataNode->getDataMap()->end()) {
        cout << "Key not found\n";
        return NULL;
    }

    LoanBookData* deletedData = dataIter->second; //Retrieve data associated with key
    dataNode->deleteMap(key); // Delete key from data node

    if (dataNode->getDataMap()->size() < ceil((order - 1) / 2.0)) { // Rebalance the tree if the number of keys in the node is below minimum
        balanceDataNode(dataNode);
    }

    // Handle the case where the root node changes
    if (dataNode == root && root->getDataMap()->empty()) {
        BpTreeNode* newRoot = root->getMostLeftChild();
        if (newRoot) {
            newRoot->setParent(NULL);
            delete root;
            root = newRoot;
        } else {
            // Handle the case where the tree becomes empty
            delete root;
            root = NULL;
        }
    }

    return deletedData;
}


void BpTree::balanceDataNode(BpTreeNode* pDataNode){   // Redistribution or merging based on the size of the sibling nodes
    if (pDataNode->getNext() && pDataNode->getNext()->getParent() == pDataNode->getParent() &&
        pDataNode->getNext()->getDataMap()->size() > ceil((order - 1) / 2.0)) { //need to redistriubte next node
        redistributeFromNext(pDataNode);
    } else if (pDataNode->getPrev() && pDataNode->getPrev()->getParent() == pDataNode->getParent() &&
        pDataNode->getPrev()->getDataMap()->size() > ceil((order - 1) / 2.0)) {  //need to redistribute prev node
        redistributeFromPrev(pDataNode);
    } else {  //need to merge data node
        mergeWithDataNode(pDataNode);
    }
}

void BpTree::redistributeFromNext(BpTreeNode* pDataNode) {
    BpTreeNode* nextNode = pDataNode->getNext();        // Get the next sibling node
    int keyToRedistribute = (nextNode->getDataMap()->size() - pDataNode->getDataMap()->size()) / 2;
    // Calculate the number of keys to redistribute based on the size difference

    // Moving keys from next node to current node
    for (int i = 0; i < keyToRedistribute; ++i) {
        map<string, LoanBookData*>::iterator iter = nextNode->getDataMap()->begin();    // Get the first key-value pair from the next node
        pDataNode->insertDataMap(iter->first, iter->second);    // Add it to the current node
        nextNode->deleteMap(iter->first);       // Remove it from the next node
    }

    // Adjust the first key in the index node
    if (pDataNode->getParent()) {
        //from parent node's index map, find nextnode and update key
        string newFirstKeyInNext = nextNode->getDataMap()->begin()->first;
        BpTreeNode* parent = pDataNode->getParent();
        for (auto& entry : *(parent->getIndexMap())) {   // Iterate through the parent's index map to find and update the entry for the next node
            if (entry.second == nextNode) {
                parent->deleteMap(entry.first);  //delete the existing key
                parent->insertIndexMap(newFirstKeyInNext, nextNode); //insert new key
                break;
            }
        }
    }
}

void BpTree::redistributeFromPrev(BpTreeNode* pDataNode) {
    BpTreeNode* prevNode = pDataNode->getPrev();    // Get the previous sibling node
    int keyToRedistribute = (prevNode->getDataMap()->size() - pDataNode->getDataMap()->size()) / 2;
    // Calculate the number of keys to redistribute based on the size difference

    // Move keys from previous node to the current node
    auto iter = prevNode->getDataMap()->rbegin();       // Start from the last key in the previous node
    while (keyToRedistribute-- > 0 && iter != prevNode->getDataMap()->rend()) {
        pDataNode->insertDataMap(iter->first, iter->second);        // Add key-value pair to the current node
        prevNode->deleteMap(iter->first);       // Remove it from the previous node
        ++iter;         // Move to the next key
    }

    // Adjust the first key in the index node
    if (pDataNode->getParent()) {
        string newFirstKeyInCurrent = pDataNode->getDataMap()->begin()->first;
        BpTreeNode* parent = pDataNode->getParent();
        //update parent node's index map
        for (auto& entry : *(parent->getIndexMap())) {
            if (entry.second == pDataNode) {
                parent->deleteMap(entry.first);  // Delete the existing key
                parent->insertIndexMap(newFirstKeyInCurrent, pDataNode); // Insert the new key
                break;
            }
        }
    }
}


void BpTree::mergeWithDataNode(BpTreeNode* pDataNode) {
    BpTreeNode* nextNode = pDataNode->getNext();    // Get the next sibling node
    BpTreeNode* prevNode = pDataNode->getPrev();    // Get the previous sibling node

    // Merge with next node
    if (nextNode && nextNode->getParent() == pDataNode->getParent()) { 
        for (auto& entry : *(nextNode->getDataMap())) {     // Merge all keys from the next node into the current node
            pDataNode->insertDataMap(entry.first, entry.second);
        }
        pDataNode->setNext(nextNode->getNext());       // Adjust the next pointer of the current node
        if (nextNode->getNext()) {       // Adjust the previous pointer of the next node's next sibling
            nextNode->getNext()->setPrev(pDataNode);
        }
        delete nextNode;        // Delete the merged next node
    } else if (prevNode && prevNode->getParent() == pDataNode->getParent()) {
        // Moving keys from the current node to previous node
        for (auto& entry : *(pDataNode->getDataMap())) {    // Merge all keys from the current node into the previous node
            prevNode->insertDataMap(entry.first, entry.second);  
        }
        prevNode->setNext(pDataNode->getNext());    // Adjust the next pointer of the previous node
        if (pDataNode->getNext()) {
            pDataNode->getNext()->setPrev(prevNode);     // Adjust the previous pointer of the current node's next sibling
        }
        delete pDataNode;   // Delete the merged current node
    }

    // Update parent index node if necessary
    if (pDataNode->getParent()) {
        balanceIndexNode(pDataNode->getParent());
    }
}



void BpTree::balanceIndexNode(BpTreeNode* pIndexNode) {
    if (pIndexNode == NULL || pIndexNode->getIndexMap()->size() >= ceil((order - 1) / 2.0)) { //if the node is NULL or already balanced
        return; // No need to balance
    }

    BpTreeNode* parent = pIndexNode->getParent();   // Get the parent node of the current index node
    if (!parent) {  // Handle the case where the index node is the root
        // If index node is root and has only one child, change the root
        if (pIndexNode->getIndexMap()->size() == 1) {   // If the root index node has only one child, make that child the new root
            root = pIndexNode->getMostLeftChild();
            root->setParent(NULL);
            delete pIndexNode;      // Delete the old root
        }
        return;
    }

    // Try to find siblings
    map<string, BpTreeNode*>::iterator iter = parent->getIndexMap()->find(pIndexNode->getIndexMap()->begin()->first);
    BpTreeNode* prevSibling = NULL;
    BpTreeNode* nextSibling = NULL;

    if(iter != parent->getIndexMap()->begin()){ 
        map<string, BpTreeNode*>::iterator prevIter = prev(iter);       //Using prev function to get prev Iterator
        prevSibling = prevIter->second;
    }

    map<string, BpTreeNode*>::iterator nextIter = next(iter);           //using next function to get next iterator
    if(nextIter != parent->getIndexMap()->end()){
        nextSibling = nextIter->second;
    }

    // Check if redistribution is possible
    if (prevSibling && prevSibling->getIndexMap()->size() > ceil((order - 1) / 2.0)) {
        redistributeFromPrev(pIndexNode);   // Redistribute from the previous sibling
    } else if (nextSibling && nextSibling->getIndexMap()->size() > ceil((order - 1) / 2.0)) {
        redistributeFromNext(pIndexNode);   // Redistribute from the next sibling
    } else {
        // Merge with sibling
        if (prevSibling) {
            mergeWithPrevIndexNode(pIndexNode); // Merge with the previous sibling
        } else if (nextSibling) {
            mergeWithNextIndexNode(pIndexNode); // Merge with the next sibling
        }
    }
}


void BpTree::mergeWithIndexNode(BpTreeNode* pIndexNode) {
    BpTreeNode* parent = pIndexNode->getParent();  // Retrieve the parent node of the index node
    map<string, BpTreeNode*>::iterator iter = parent->getIndexMap()->find(pIndexNode->getIndexMap()->begin()->first); // Find the position of the index node in its parent's index map

    BpTreeNode* prevSibling = NULL;
    BpTreeNode* nextSibling = NULL;

    if (iter != parent->getIndexMap()->begin()) {   // Check and assign previous sibling if it exists
        prevSibling = prev(iter)->second;
    }

    if (next(iter) != parent->getIndexMap()->end()) {  // Check and assign next sibling if it exists
        nextSibling = next(iter)->second;
    }

    if (prevSibling && prevSibling->getParent() == parent) {    // Decide which sibling to merge with based on their availability and relationship to the parent
        mergeWithPrevIndexNode(pIndexNode);     // Merge with the previous sibling
    } else if (nextSibling && nextSibling->getParent() == parent) {
        mergeWithNextIndexNode(pIndexNode);     // Merge with the next sibling
    }
}


void BpTree::mergeWithPrevIndexNode(BpTreeNode* pIndexNode) {
    BpTreeNode* parent = pIndexNode->getParent(); // Access the parent of the index node
    map<string, BpTreeNode*>::iterator iter = parent->getIndexMap()->find(pIndexNode->getIndexMap()->begin()->first); // Locate the position of the index node in the parent's index map
    BpTreeNode* prevSibling = prev(iter)->second;      // Retrieve the previous sibling of the index node

    for (auto& entry : *(pIndexNode->getIndexMap())) {   // Iterate through the index node's entries and transfer them to the previous sibling
        prevSibling->insertIndexMap(entry.first, entry.second);
    }

    prevSibling->setNext(pIndexNode->getNext());     // Update the sibling links: set the previous sibling's next to the current node's next
    if (pIndexNode->getNext()) {        // If the current node has a next sibling, update its previous link to point to the previous sibling
        pIndexNode->getNext()->setPrev(prevSibling);
    }

    parent->getIndexMap()->erase(iter);  // Remove the current index node from the parent's index map and deallocate it
    delete pIndexNode;

    if (parent->getParent() && parent->getIndexMap()->size() < ceil((order - 1) / 2.0)) {   // Check if the parent needs to be balanced after the merge
        balanceIndexNode(parent);
    }
}


void BpTree::mergeWithNextIndexNode(BpTreeNode* pIndexNode) {
    BpTreeNode* parent = pIndexNode->getParent();       // Access the parent of the index node
    map<string, BpTreeNode*>::iterator iter = parent->getIndexMap()->find(pIndexNode->getIndexMap()->begin()->first); // Locate the position of the index node in the parent's index map
    BpTreeNode* nextSibling = next(iter)->second;   // Retrieve the next sibling of the index node

    for (auto& entry : *(nextSibling->getIndexMap())) {      // Iterate through the next sibling's entries and transfer them to the current node
        pIndexNode->insertIndexMap(entry.first, entry.second);
    }

    pIndexNode->setNext(nextSibling->getNext());    // Update the sibling links: set the current node's next to the next sibling's next
    if (nextSibling->getNext()) {   // If the next sibling has a next sibling, update its previous link to point to the current node
        nextSibling->getNext()->setPrev(pIndexNode);
    }

    parent->getIndexMap()->erase(next(iter));   // Remove the next sibling from the parent's index map and deallocate it
    delete nextSibling;

    if (parent->getParent() && parent->getIndexMap()->size() < ceil((order - 1) / 2.0)) {   // Check if the parent needs to be balanced after the merge
        balanceIndexNode(parent);
    }
}



