//
//File TreeManager.cpp
//

#include "MaterialsAccounting.h"

//
//
//


void TreeManager::printTree(const char *filename){
	fstream f(filename, ios::out);
	tree->print(f);
	f.close();
}

void TreeManager::getFileTree(const char *filename){
	fstream f(filename, ios::out);
	tree->getFile(f);
	f.close();
}
void TreeManager::putFileTree(const char *filename){
	fstream f(filename, ios::out);
	tree->putFile(f);
	f.close();
}

//Create Singletone object
TreeManager & getManager(){

	static TreeManager tm; 
	return tm; 
}

void TreeManager::instIterator(){
	iterator.push_back(tree);
}

void TreeManager::addChild(const string &name, UINT64 amount){

	treeObj->getTree( iterator.back().get() )  ->add(name, amount);

}

void TreeManager::go(const string &name){

	iterator.push_back( (*treeObj)[name]);

}

void TreeManager::back(){

	iterator.pop_back();

}

void TreeManager::deleteChild(const string &name){
	treeObj->getTree( iterator.back().get() )  -> remove(name) ;
}

void TreeManager::deleteThis(){
	string name = treeObj->getTree( iterator.back().get() ) ->getName();
	iterator.pop_back();
	deleteChild(name);
}

