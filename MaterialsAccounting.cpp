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
		auto iter( children.find(tmp) );

		if(iter==children.end()){
			throw NotCorrectAdress();
		}
		else{
			return iter;
		}
}

void TreeMaterial::remove(const Material &m){

	children.erase(m);

}

void TreeMaterial::remove(string name){

	auto iter( search(name) );
	if(iter!=children.end()){
		remove(*iter);
		return;
	}
	else{
		throw NotDeletedObject();
	}

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
