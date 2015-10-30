//
//
//
//
//
//

#include "MaterialsAccounting.h"

//
//
//

bool operator == (const Material &a, const Material &b){
	return *a == *b;
}
bool operator > (const Material &a, const Material &b){
	return *a > *b;
}
bool operator < (const Material &a, const Material &b){
	return *b > *a;
}

set<Material>::iterator TreeMaterial::search(string name){

	TempMaterial m(name);
	Material tmp(&m);
	return children.find(tmp);

}

void TreeMaterial::remove(string name){

	auto iter( search(name) );
	remove(*iter);

}

void TreeMaterial::print(ostream & out)const{
	string str( getName() );
	print(out, str );
}

void TreeMaterial::print(ostream & out, string &full_name)const{

	for( auto i=children.begin()  ; i!=children.end() ; ++i ){
		try{
			TreeMaterial *tree=getTree( i->get() );
			tree->print( out, full_name+' '+tree->getName() );
		}
		catch(NotCorrectCast &){
			out << full_name+' ';
			(*i)->print(out);
			out << '\n';
		}
	}
}

TreeMaterial * TreeMaterial::getTree(MaterialAccounting *ma)const{
	TreeMaterial *tm=getTree_(ma);
	if( tm==0 ){
		throw NotCorrectCast();
	}
	return tm;
}
SingleMaterial * TreeMaterial::getSingle(MaterialAccounting *ma)const{
	SingleMaterial *sm=getSingle_(ma);
	if( sm==0 ){
		throw NotCorrectCast();
	}
	return sm;
}



UINT64 MaterialAccounting::putFile(fstream &out, UINT64 loc)const{

	out.seekp(loc, ios::beg);
	
	unsigned char l= name.size();
	//string str=char*l+getName();
	out.write( (char*)&l , sizeof(l) );
	out.write(name.c_str(), l*sizeof(char) ); //Put name current item
	return l+1;
}
UINT64 MaterialAccounting::getFile(fstream &in, UINT64 loc){

	char tmp[257];
	unsigned char l;
	UINT64 nByte=0;
	in.seekg(loc, ios::beg);
	in.read( (char*)&l, sizeof(l));
	in.read( (char*)tmp, l);
	//cout << "name:" << (int)l << "\n";
	tmp[l]=0;
	name=tmp;
	return l+1;

}

UINT64 TreeMaterial::putFile(fstream & out, UINT64 loc)const{
	//this function do writing children TreeMaterial in file out
	UINT64 nByte=MaterialAccounting::putFile(out,loc);

	//put count children info
	auto count_children=children.size();
	out.seekp(loc+nByte, ios::beg);
	out.write( (char*)&count_children, sizeof(count_children));
	//
	nByte+=sizeof(count_children);

	UINT64 posChildren=nByte;
	bool typeChildren;	//true - child - tree, false - child - single

	//"reserve" memory for byte children and type indicator
	nByte+= ( sizeof(nByte)+sizeof(typeChildren) ) * children.size();
	//posChildren - position children byte
	//nByte - position first child

	UINT64 j=0;
	for( auto i(children.begin())  ; i!=children.end() ; ++i, ++j ){
	
		auto adr=loc+nByte;
		out.seekp( loc+posChildren + ( sizeof(nByte) + sizeof(typeChildren) ) *  j, ios::beg);
		out.write( (char*)&adr, sizeof(nByte) );
		typeChildren= (  getTree_( i->get() ) != 0 );
		out.write( (char*)&typeChildren, sizeof(typeChildren) );
		nByte+= (*i)->putFile(out,loc+nByte);
		//nByte - position next child
	}
	return nByte;
}
UINT64 TreeMaterial::getFile(fstream &in, UINT64 loc){

	UINT64 nByte;
	in.seekg(loc, ios::beg);
	nByte=MaterialAccounting::getFile(in, loc);
	auto count_children=children.size();
	
	//get count children info
	in.seekg(loc+nByte, ios::beg);
	in.read( (char*)&count_children, sizeof(count_children));
	nByte+=sizeof(count_children);
	//nByte - pos byte info children
	auto pos_children=nByte;

	for(  unsigned int i = 0 ; i<count_children ;  ++i ){
		//look for type child
		bool type;
		auto nByteChild=nByte;
		in.seekg( loc + pos_children + i*( sizeof(nByte)+sizeof(type) ) , ios::beg);
		in.read((char*)&nByteChild, sizeof(nByteChild) );
		in.read((char*)&type, sizeof(type) );
		MaterialAccounting *ma= ( type ) ? (new TreeMaterial(" ")) : (MaterialAccounting *)(new SingleMaterial(" ",0));

		nByte+=ma->getFile(in, nByteChild);
		children.insert( ma );
		
	}

	return nByte + (sizeof(nByte)+sizeof(bool) )*children.size();
}


UINT64 SingleMaterial::putFile(fstream &out, UINT64 loc)const{

	UINT64 nByte=MaterialAccounting::putFile(out, loc);
	out.seekp(loc+nByte, ios::beg);
	out.write( (char*)&amount , sizeof(amount));

	return nByte+=sizeof(amount);
}
UINT64 SingleMaterial::getFile(fstream &in, UINT64 loc){

	UINT64 nByte=MaterialAccounting::getFile(in, loc);
	in.seekg(loc+nByte, ios::beg);
	in.read( (char*)&amount , sizeof(amount));
	
	//cout << " get amount: " << amount << '\n';

	return nByte+=sizeof(amount);
}

/*
void TreeMaterial::doGroup(string name){

	//
	auto tmp( children.find( static_cast<Material>(StringToMaterial(name) ) ) );
	if( tmp!=children.end() ){
		SingleMaterial *single=getSingle( tmp->get() );
			// if convert in tree material failure then this item single material
		if(single){
			//do group
			TreeMaterial* tree= new TreeMaterial(name);
			tree->add(" ", single->getAmount() );
			children.erase(tmp);
			children.insert( tree );
		}

	}else{
		
	}

}

void TreeMaterial::print(ostream & out){
		print(getName(),out);
}

void TreeMaterial::print(string full_name, ostream & out){

	for( auto i=children.begin()  ; i!=children.end() ; ++i ){
		TreeMaterial *tree=getTree( i->get() );
		if(tree!=0){
			
			tree->print(full_name+' '+tree->getName(),out);
			
		}else{
			out << full_name+' ';
			(*i)->print(out);
			out << '\n';
		}
	}
}

Material & TreeMaterial::operator[](string name){

	auto i=children.find( static_cast<Material>(StringToMaterial(name) ) );

	if(i!=children.end()){
		
		return Material(*i);

		//return const_cast<Material &> (*i);
	}
	else{
		//throw
		//later
		cout << "Not found this items\n";
		//ERROR!
		Material m;
		return m;
		////return static_cast<Material&> (*this);
	}
}

void TreeManager::printTree(char *filename){
	fstream f(filename, ios::out);
	tree.print(f);
	f.close();
}

TreeManager & getManager(){

	static TreeManager tm; 
	return tm; 
}

MaterialAccounting * StringToMaterial(string name){

	TempMaterial *tmp=new TempMaterial(name);

	return tmp;

}



UINT64 TreeMaterial::putFile(fstream & out, UINT64 loc){
	//this function do writing children TreeMaterial in file out
	UINT64 nByte=MaterialAccounting::putFile(out,loc);

	//put count children info
	auto count_children=children.size();
	out.seekp(loc+nByte, ios::beg);
	out.write( (char*)&count_children, sizeof(count_children));
	//
	nByte+=sizeof(count_children);

	UINT64 posChildren=nByte;
	bool typeChildren;	//true - child - tree, false - child - single

	//"reserve" memory for byte children and type indicator
	nByte+= ( sizeof(nByte)+sizeof(typeChildren) ) * children.size();
	//posChildren - position children byte
	//nByte - position first child
	
	UINT64 j=0;
	for( auto i(children.begin())  ; i!=children.end() ; ++i, ++j ){
	
		auto adr=loc+nByte;
		out.seekp( loc+posChildren + ( sizeof(nByte) + sizeof(typeChildren) ) *  j, ios::beg);
		out.write( (char*)&adr, sizeof(nByte) );
		typeChildren= (  getTree( i->get() ) != 0 );
		out.write( (char*)&typeChildren, sizeof(typeChildren) );
		nByte+= (*i)->putFile(out,loc+nByte);
		//nByte - position next child
	}
	return nByte;
}
UINT64 TreeMaterial::getFile(fstream &in, UINT64 loc, int g){

	UINT64 nByte;
	in.seekg(loc, ios::beg);
	nByte=MaterialAccounting::getFile(in, loc,g);
	auto count_children=children.size();
	
	//get count children info
	in.seekg(loc+nByte, ios::beg);
	in.read( (char*)&count_children, sizeof(count_children));
	nByte+=sizeof(count_children);
	//nByte - pos byte info children
	auto pos_children=nByte;

	for(  unsigned int i = 0 ; i<count_children ;  ++i ){
		//look for type child
		bool type;
		auto nByteChild=nByte;
		in.seekg( loc + pos_children + i*( sizeof(nByte)+sizeof(type) ) , ios::beg);
		in.read((char*)&nByteChild, sizeof(nByteChild) );
		in.read((char*)&type, sizeof(type) );
		MaterialAccounting *ma= ( type ) ? (new TreeMaterial(" ")) : (MaterialAccounting *)(new SingleMaterial(" ",0));

		nByte+=ma->getFile(in, nByteChild,g+1);
		children.insert( ma );
		
	}

	return nByte + (sizeof(nByte)+sizeof(bool) )*children.size();
}

void TreeManager::putFileTree(char *filename){

	fstream f(filename, ios::binary | ios::out);
	tree.putFile(f, 0);
	f.close();
}
//
//
//*/