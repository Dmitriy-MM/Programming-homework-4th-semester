#include <cstdio>
#include "make_query_to_base_func.h"
#include "Query_operation_declares.h"
#include "Query_types_declares.h"
#include "Query_processing_order_declares.h"

#define LEN 1234

enum FUNC_RET_CODES 
{
	F_SUCCESS = 0,
	ERROR_CANT_REACH_EOF,
	ERROR_CANT_OPEN_FIN,
	ERROR_CANT_OPEN_FOUT
};

static void rm_line_feed (char * str, int max_length)
{
	int i;
	str[max_length-1] = '\0';
	for (i = 0; str[i] && (i < max_length); i++)
	{
		if (str[i] == '\n')
		{
			str[i] = '\0';
			break;
		}
	}
}


int make_query_to_base (
	const char * filename_in,
	const char * filename_out,
	const char *s,
	const char *t,
	Query_type type, 
	Query_processing_order order, 
	Query_operation arg_op_type)
{
	FILE * fin, *fout;
	char buf[LEN] = {'\0'};
	int count_right_strs = 0;
	Query query;
	
	query.parse (s, t, type, order, arg_op_type);
	
	fin = fopen (filename_in, "r");
	if (!fin)
	{ return -ERROR_CANT_OPEN_FIN; }
	fout = fopen (filename_out, "w");
	if (!fout)
	{
		fclose (fin);
		return -ERROR_CANT_OPEN_FOUT;
	}
	
	while (fgets (buf, LEN, fin))
	{
		rm_line_feed (buf, LEN);
		if (query.apply_to (buf))
		{		
			count_right_strs++;
			fprintf (fout, "%s\n", buf);
		}
	}
	if (!feof (fin))
	{
		fclose (fin), fclose (fout);
		return -ERROR_CANT_REACH_EOF;
	}
	fclose (fin), fclose (fout);
	return count_right_strs;
}
