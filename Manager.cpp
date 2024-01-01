#include "Manager.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <cstring>
using namespace std;

void Manager::printErrorCode(int n) 		//ERROR CODE PRINT
{		
	flog << "=========ERROR=========" << endl;
	flog << n << endl;
	flog << "=======================" << endl << endl;
}

void Manager::printSuccessCode(const char* cmd)			//Success CODE PRINT
{
    flog << "===== " << cmd << " =====" << endl;
    flog << "Success" << endl;
    flog << "==================" << endl << endl;
}

void Manager::run(const char* command) 
{
	fin.open(command);
	if(!fin)
	{
		flog << "File Open Error" << endl << endl;
		return;
	}
	flog.open("log.txt", ios::app);
	if(!flog){  //if we can't open log.txt file
        flog << "Fail to open log file" << endl << endl;
        exit(-1);   //End program
    }
	
	LoanBookHeap* heapStore[8]; 	//for run data
	for(int i = 0; i < 8; i++){		//using array to store each heap
		heapStore[i] = new LoanBookHeap();
	}
	bpTree = new BpTree(&flog);    //create BpTree instance
	selectTree = new SelectionTree(&flog);		//create SelectionTree instance

	vector<SelectionTreeNode*> selStore(8);			//then create SelectionTree First and put data later from ADD or other function
	for(int i = 0; i < 8; i++){
		selStore[i] = new SelectionTreeNode(); 
		selStore[i]->setHeap(heapStore[i]);    //in order to connect run and selection tree's leaf node
	}

	while(selStore.size() > 1){	  //connect each heap with selection tree's leaf node
		vector<SelectionTreeNode*> nextLevel;     //using vector to store data
		for(size_t i = 0; i < selStore.size(); i+= 2){
			SelectionTreeNode* parent = new SelectionTreeNode();   //create each new leaf node
			parent->setLeftChild(selStore[i]);			//then set as child
			selStore[i]->setParent(parent);		//set parent
			if(i + 1 < selStore.size()){
				parent->setRightChild(selStore[i + 1]);			//connect run with leaf node
				selStore[i + 1]->setParent(parent);
			}
			nextLevel.push_back(parent);
		}
		selStore = nextLevel;    
	}
	selectTree->setRoot(selStore.front());     //set selection tree's node

	
	string commandLine = "";		
	while (!fin.eof())
	{
		getline(fin, commandLine);			//read each command.txt line
		if(commandLine.empty()){    //if line is empty
			continue;		//continue
		}
		istringstream iss(commandLine);
		string commandStart = "";
		iss >> commandStart;
		if(commandStart == "LOAD"){				//For Command LOAD
			if(LOAD(&flog, bpTree)){
				printSuccessCode("LOAD");
			}else{
				printErrorCode(100);
			}
		}else if(commandStart == "ADD"){		//For Command ADD
			string eachStr;
			getline(iss, eachStr, '\t');
			string addName, addAuthor, conCode, conYear = "";
			int addCode, addYear;
			if(!getline(iss, addName, '\t') || addName.empty()){		//Get data by Tab 
				printErrorCode(200);
				continue;   //if string is empty or failed to input
			}
			if(!getline(iss, conCode, '\t') || conCode.empty()){
				printErrorCode(200);
				continue;
			}else{
				addCode = stoi(conCode);		//Need to convert book code from string to int
			}
			if(!getline(iss, addAuthor, '\t') || addAuthor.empty()){
				printErrorCode(200);
				continue;
			}
			if(!getline(iss, conYear, '\t') || conYear.empty()){
				printErrorCode(200);
				continue;
			}else{
				addYear = stoi(conYear);		//Need to convert Year from string to int
			}
			LoanBookData* addData = new LoanBookData();		//Create new LoanBookData with given data
			addData->setName(addName);
			addData->setCode(addCode);
			addData->setAuthor(addAuthor);
			addData->setYear(addYear);
			if(ADD(selectTree, addData, bpTree)){		//If correct data is given
				flog << "========ADD========" << endl;
				flog << addName << "/";
				if(addCode == 0){
					flog << "000/";
				}else{
					flog << addCode << "/";
				}
				flog << addAuthor << "/" << addYear << endl;
				flog << "===================" << endl << endl;
			}else{
				printErrorCode(200);
			}
		}else if(commandStart == "SEARCH_BP"){		//For Command SEARCH_BP
			string eachStr;
			getline(iss, eachStr, '\t');
			string firstStr, secondStr;
			if(!getline(iss, firstStr, '\t')){
				printErrorCode(300);
			}
			if(getline(iss, secondStr, '\t')){		//If we get two input data
				if(!SEARCH_BP_RANGE(&flog, firstStr, secondStr, bpTree)){		//Call SEARCH_BP_RANGE function
					printErrorCode(300);	
				}
			}else{
				if(!SEARCH_BP_BOOK(&flog, firstStr, bpTree)){		//If we only get one string data, call SEARCH_BP_BOOK function
					printErrorCode(300);		
				}
			}
		}else if(commandStart == "PRINT_BP"){		//For Command PRINT_BP
			if(!PRINT_BP(&flog, bpTree)){
				printErrorCode(400);
			}
		}else if(commandStart == "PRINT_ST"){		//For Command PRINT_ST
			string eachStr;
			getline(iss, eachStr, '\t');
			string code = "";
			if(!getline(iss, code, '\t')){
				printErrorCode(500);
			}
			if(!PRINT_ST(&flog, code, selectTree)){
				printErrorCode(500);
			}
		}else if(commandStart == "DELETE"){			//For Command DELETE
			if(DELETE(selectTree)){
				printSuccessCode("DELETE");
			}else{
				printErrorCode(600);
			}
		}else if(commandStart == "EXIT"){		//For Command EXIT
			printSuccessCode("EXIT");
			fin.close();
			flog.close();   //close log.txt
			return;   //End Program
		}else{
			printErrorCode(700);   //Wrong command
		}
	}
	fin.close();
	flog.close();   //close log.txt
	return;
}

bool Manager::LOAD(ofstream *fout, BpTree* bpT)
{
	if(bpT->getRoot() != NULL){
		return false;
	}

	ifstream dataFile("loan_book.txt");   //to read data from loan_book.txt file
	if(!dataFile.is_open()){    //if we can't open loan_book.txt file
		*fout << "Error Opening loan_book.txt\n\n";
		return false;   //return false
	}

	vector<string> lines;   //in order to store each line
	string line;	
	while(getline(dataFile, line)){    //read from loan_book.txt by line
		lines.push_back(line);    //then push to vector
	}
	dataFile.close();    //then close file

	for (string each : lines) {    //for each line
		istringstream iss(each);			//using istringstream to split string
		string token;
		int count = 0;
		LoanBookData* newData = new LoanBookData();    //create new LoanBookData instance
		while (getline(iss, token, '\t')) {		//split by tab
			if(count == 0){    //to set Book Name
				newData->setName(token);
			}else if(count == 1){	//to set BookCode
				int code = stoi(token);		//turn string value to int
				newData->setCode(code);
			}else if(count == 2){		//to set Author name
				newData->setAuthor(token);
			}else if(count == 3){		//to set Year value
				int year = stoi(token);		//turn string to int
				newData->setYear(year);
			}else if(count == 4){			//since loan_Count initiaite to zero, we need to insert to BpTree
				bpT->Insert(newData);
			}
			count++;
		}
	}
	return true;
}

bool Manager::ADD(SelectionTree* selT, LoanBookData* nData, BpTree* bpT)
{
	bpT->Insert(nData);			//Insert data into BpTree
	BpTreeNode* tempVal = bpT->searchDataNode(nData->getName());		//Get Data from BpTree
	if(tempVal != NULL){
		BpTreeDataNode* dataNode = dynamic_cast<BpTreeDataNode*>(tempVal);		//Type Cast
		if(dataNode != NULL){	
			auto dataMap = dataNode->getDataMap();			//Get Data Map's iterator from dataNode
			auto it = dataMap->find(nData->getName());     //Find data from Data Map
			if(it != dataMap->end()){
				LoanBookData* bookData = it->second;
				int bookCode = bookData->getCode();
				int loanCount = bookData->getLoanCount();
				if(bookCode == 0 || bookCode == 100 || bookCode == 200){	//If book code is 0 ~ 200 
					if(loanCount == 3){		//If count is 3 then 
						//Delete from BpTree and move to Heap, Selection Tree
						LoanBookData* deletedData = bpT->Delete(bookData->getName()); //delete from BpTree
						selT->Insert(deletedData);
					}
				}else if(bookCode == 300 || bookCode == 400){			//If book code is 300 ~ 400
					if(loanCount == 4){			//If count is 4 then
						//Delete from BpTree and move to Heap, Selection Tree
						LoanBookData* deletedData = bpT->Delete(bookData->getName()); //delete from BpTree
						selT->Insert(deletedData);
					}
				}else if(bookCode == 500 || bookCode == 600 || bookCode == 700){		//If book code is 500 ~ 700
					if(loanCount == 2){		//If count is 2
						//Delete from BpTree and move to Heap, Selection Tree
						LoanBookData* deletedData = bpT->Delete(bookData->getName()); //delete from BpTree
						selT->Insert(deletedData);
					}
				}
			}
		}
	}
	return true;
}

bool Manager::SEARCH_BP_BOOK(ofstream *fout, string book, BpTree* bpT) 
{
	return bpT->searchBook(book, fout);	
}

bool Manager::SEARCH_BP_RANGE(ofstream *fout, string s, string e, BpTree* bpT) 
{
	return bpT->searchRange(s, e, fout);
}

bool Manager::PRINT_BP(ofstream *fout, BpTree* bpt) 
{
	return bpt->printAll(fout);
}

bool Manager::PRINT_ST(ofstream *fout, string code, SelectionTree* selT) 
{
	int result = 0;
	result = selT->printHeap(fout, code);
	if(result == 500){
		return false;
	}else{
		return true;
	}
}

bool Manager::DELETE(SelectionTree* selT) 
{
	return selT->Delete();
}



