#include <cstdio>
#include <cstring>
#include "Query.h"


int Query::init_tabs_str (const char * tabs)
{
	char * auxiliary_str = nullptr;
	char first_symbol = tabs[0];
	size_t tabs_length;
	size_t i, j;
	
	tabs_length = strlen (tabs);
	auxiliary_str = new char [tabs_length + 1];
	if (!auxiliary_str)
		return !0;
	strcpy (auxiliary_str, tabs);
	// The replacing all repeated characters with 'first_symbol'.
	// Example "FrranFdddnaFomsFandhitht" -> "FrFanFdFFFFFomsFFFFhitFF".
	for (i = 1; i < tabs_length; i++)
		for (j = i+1; j < tabs_length; j++)
			if (auxiliary_str[j] == auxiliary_str[i])
				auxiliary_str[j] = first_symbol;
	// The shifting others 'Non-first_symbol' characters to start of a line.
	for (i = 1; i < tabs_length; i++)
	{
		for (j = i; auxiliary_str[j] == first_symbol; j++)
		{ ;}
		if ((j < tabs_length) && ((i != j)))
		{
			auxiliary_str[i] = auxiliary_str[j];
			auxiliary_str[j] = first_symbol;
		}
	}
	// The string seems like "FrandomshitFFFFFFFFFFFFF" right now.
	// The replacing second entry of F (first_symbol) with '\0'.
	for (i = 1; i < tabs_length; i++)
	{
		if (auxiliary_str[i] == first_symbol)
			auxiliary_str[i] = '\0';
	}
	
	tabs_length = strlen (auxiliary_str);
	str_tabs = new char [tabs_length+1];
	if (str_tabs == nullptr)
	{
		delete[] auxiliary_str;
		return !0;
	}
	strcpy (str_tabs, auxiliary_str);
	delete[] auxiliary_str;
	return 0;
}


int Query::init_s_words_array (const char * str)
{
	Words_rider str_words (str, str_tabs);
	size_t word_length;
	int i, j;
	
	for (s_words_length = 0; str_words.get_word_length () != 0; str_words.next())
		s_words_length++;
	
	s_words = new char* [s_words_length];
	if (s_words == nullptr)
		return -1;
	str_words.reset_pos();
	for (i = 0; i < s_words_length; str_words.next(), i++)
	{
		word_length = str_words.get_word_length () + 1;
		s_words[i] = new char [word_length];
		if (s_words[i] == nullptr)
		{
			for (j = 0; j < i; j++)
				delete[] s_words[j];
			delete[] s_words;
			return -2;
		}
		str_words.extract_word_to (s_words[i], word_length);
	}
	
	return 0;
}


void Query::parse (const char * source, const char * tabs, const char * op_type_name, int is_for_all)
{
	flag_is_for_all = is_for_all;
	init_tabs_str (tabs);
	init_s_words_array (source);
	
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
	Words_rider str_words (str, str_tabs);
	int str_word_length;
	int i, cmp_res;
	
	if ((op_type == Query_operation::operation_invalid) || (op_type == Query_operation:: operation_none))
	{
		printf ("INVALID OPERATION!\n");
		return -1;
	}
	
	while ((str_word_length = str_words.get_word_length ()) != 0)
	{
		str_words.extract_word_to (str_word_buf, WORD_LEN);
		for (i = 0; i < s_words_length; i++)
		{
			cmp_res = strcmp (str_word_buf, s_words[i]);
			if (cmp_res > 0)
			{
				switch (op_type)
				{
					case Query_operation::operation_me:
					case Query_operation::operation_mr:
					case Query_operation::operation_ne:
						if (flag_is_for_all)
							break;
						else
							return 1;
					default: 
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
							break;
						else
							return 1;
					default: 
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
							break;
						else
							return 1;
					default: 
							continue;
				}
			}
			break;
		} // End for.
		if ((i == s_words_length) && flag_is_for_all)
			return 0;
		str_words.next ();
	}
	
	if (flag_is_for_all)
		return 1;
	else
		return 0;
}
