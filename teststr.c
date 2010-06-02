#include <stdio.h>
#include <stdlib.h>
#include "str.h"

char* tmp;

#define T(a)	{ tmp = a; printf("%s\n", tmp); free(tmp); }

int main()
{
	T(cat2("a", "b"))
	T(cat3("a", NULL, "b"))
        T(cat4("a", NULL, "b", "--------"))
        T(cat5("a", NULL, "b", "--", "NULL"))
        T(cat6("a", NULL, "b", "NULL", "   ", NULL))
        T(cat7("a", NULL, "b", NULL, NULL, NULL, "7"))
        T(cat8("a", NULL, "b", "4", "5", NULL, NULL, "8"))
        T(cat9("a", NULL, "b", "     ", "5", "6", "7", NULL, "9"))

	return 0;
}
