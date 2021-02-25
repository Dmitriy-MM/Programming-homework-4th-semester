#include <cstdio>
#include <ctime>

#include "Query.h"
#include "make_query_to_base_func.h"


enum MAIN_RET_CODES
{
	SUCCESS = 0,
	WRONG_INPUT
};


int main (int argc, char * argv[])
{
	int task = 5, res;
	clock_t t_elapsed;
	
	if (!(
		(argc == 5)
	))
	{
		printf ("Usage: %s input.txt output.txt \"input_str\" \"tab_symbols_str\"\n", argv[0]);
		return WRONG_INPUT;
	}
	
	t_elapsed = clock ();
	res = make_query_to_base (argv[1], argv[2], argv[3], argv[4], Query_type::type_regexp, Query_processing_order::order_once_enough, Query_operation::operation_none);
	t_elapsed = clock () - t_elapsed;
	printf ("%s : Task = %d Result = %d Elapsed = %.2lf\n", argv[0], task, res, (double) t_elapsed/CLOCKS_PER_SEC);
	
	return SUCCESS;
}
