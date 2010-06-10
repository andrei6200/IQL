#include <iostream>

extern "C"
{
    extern int errorCount;
    void yyparse();
    char* hqlQueries;
}


using namespace std;

int main()
{
	yyparse();
//        printf("\n\n\nHQL QUERIES : \n%s\n", hqlQueries);
        cout << "\n\n\n" << "HQL QUERIES\n" << hqlQueries << endl << flush;
	return errorCount;
}
