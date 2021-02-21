#include <cstdio>
#include "Query.h"

void Query::parse (const char * source, const char * tabs, const char * op_type_name, int is_for_all)
{
	query_words.init (source, tabs);
	flag_is_for_all = is_for_all;
	
	if (!strcmp (op_type_name, "="))
		op_type = Query_operation::operation_eq;
	else if (!strcmp (op_type_name, "<>"))
		op_type = Query_operation::operation_ne;
	else if (!strcmp (op_type_name, ">="))
		op_type = Query_operation::operation_me;
	else if (!strcmp (op_type_name, ">"))
		op_type = Query_operation::operation_mr;
	else if (!strcmp (op_type_name, "<="))
		op_type = Query_operation::operation_le;
	else if (!strcmp (op_type_name, "<"))
		op_type = Query_operation::operation_ls;
	else
		op_type = Query_operation::operation_invalid;
}

int Query::apply_to (const char * str)
{
	Words_rider str_words (str, query_words.str_tabs); // Те же пробельные символы, что и для строки запроса,
	// написано так по-уродски, чтобы не плодить свойства-указатели, которые не дай бог ещё станут разными для Query и Words_rider.
	
	int query_word_length, str_word_length;
	int cmp_res;
	
	if (!query_words.is_initiated)
	{
		printf ("Apply was called before \"parse\" function\n");
		return -1;
	}
	if (op_type == Query_operation::operation_invalid)
	{
		printf ("INVALID OPERATION!\n");
		return -1;
	}
	
	while ((str_word_length = str_words.get_word_length ()) != 0)
	{
		str_words.extract_word_to (str_word_buf, WORD_LEN);
		query_words.reset_pos ();
		while ((query_word_length = query_words.get_word_length ()) != 0)
		{
			query_words.extract_word_to (query_word_buf, WORD_LEN);
			cmp_res = strcmp (str_word_buf, query_word_buf);
			if (cmp_res > 0)
			{
				switch (op_type)
				{
					case Query_operation::operation_me:
					case Query_operation::operation_mr:
					case Query_operation::operation_ne:
						if (flag_is_for_all)
						{
							break;
						}
						else
							return 1;
					default: 
							query_words.next ();
							continue;
				}
			}
			else if (cmp_res == 0)
			{
				switch (op_type)
				{
					case Query_operation::operation_eq:
					case Query_operation::operation_le:
					case Query_operation::operation_me:
						if (flag_is_for_all)
						{
							break;
						}
						else
							return 1;
					default: 
							query_words.next ();
							continue;
				}
			}
			else
			{
				switch (op_type)
				{
					case Query_operation::operation_le:
					case Query_operation::operation_ls:
					case Query_operation::operation_ne:
						if (flag_is_for_all)
						{
							break;
						}
						else
							return 1;
					default: 
							query_words.next ();
							continue;
				}
			}
			break;
		}
		if ((query_words.get_word_length () == 0) && flag_is_for_all)
		{ return 0;	}
		str_words.next ();
	}
	
	if (flag_is_for_all)
		return 1;
	else
		return 0;
}
