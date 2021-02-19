#include "make_query_to_base_func.h"

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
			str[i] = '\n';
			break;
		}
	}
}


int make_query_to_base (
	const char * filename,
	const char * filename2,
	const char *s,
	const char *t,
	const char * op_type_name,
	int is_for_all)
{
	FILE * fin, *fout;
	char buf[LEN] = {'\0'};
	int count_right_strs = 0;
	Query query;
	
	query.parse (s, t, op_type_name, is_for_all);
	
	fin = fopen (filename, "r");
	if (!fin)
	{ return -ERROR_CANT_OPEN_FIN; }
	fout = fopen (filename2, "w");
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
			fprintf (fout, "%s", buf);
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
