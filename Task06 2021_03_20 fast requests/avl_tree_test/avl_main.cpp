#include <stdio.h>

#include "avl.h"
#include "record.h"

static int compare (Record& r1, Record& r2, void * cmp_arg = nullptr)
{
	(void) cmp_arg;
	return r1.get_phone() - r2.get_phone();
	//return strcmp (r1.get_name(), r2.get_name ());
}

int main (int argc, char * argv[]) 
{
	AVL_tree<Record> base (compare);
	Record dima;
	dima.init ("dima", 925, 208);
	if (!(argc == 2))
	{
		printf ("Usage: %s filename\n", argv[0]);
		return -1;
	}
	base.read (argv[1]);
	base.print (3);
	base.remove (base.find (dima));
	base.print (3);
	return 0;
}
