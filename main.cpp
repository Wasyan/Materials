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

int main(int argc, char *argv[]){

	//*//temp version, simulation the program is started from console.
	char temp[]="console";
	*argv=temp;
	argc=2;
	////////////////////////////////////////////////////////*/
	
	if( argc > 1 && strcmp("console", *argv )==0 ){ // check for request terminale mode

#define T1EST_TREE_CODE

#ifdef TEST_TREE_CODE
		TreeMaterial tm("111");
		tm.add("1111", 3);
		tm.add("1113", 5);
		tm.add("1114", 7);
		tm.add("1112", 11);
		TreeMaterial tm2("11");
		tm2.add( tm );
		tm2.add("112", 13);
		tm2.add("113", 13);
		tm2.add("114", 13);
		tm2.add("115", 13);
		TreeMaterial tm3("1");
		tm3.add(tm2);

		tm3.print(cout);

		fstream f("file.dat", ios::binary | ios::out);
		tm3.putFile((fstream&)f,0);
		f.close();

		TreeMaterial t("n");

		f.open("file.dat", ios::binary | ios::in);
		t.getFile((fstream&)f,0);
		f.close();



		t.print(cout);
		
#else

		try{
			ConsoleInterface *ci=new ConsoleInterface();
			ci->getCommand();
		}
		catch(...){
			cout << "Unknown exception. Programm will be closed.\n";
		}
		//getManager();
#endif
	}
	else{ //terminale mode not selected

		//create window, after program works in windows mode
		//----------------------------------------------///
	}

	return 0;
}