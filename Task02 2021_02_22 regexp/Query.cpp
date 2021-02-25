#include <cstdio>
#include <cstring>
#include "Query.h"
#include "Word_regexp.h"


int Query::apply_to (const char * str)
{	
	switch (qu_type)
	{
		case Query_type::type_find:
			return to_process_qu_find (str);
			break;
		case Query_type::type_regexp:
			return to_process_qu_regexp (str);
			break;
		default: 
			break;
	}
	return 0;
}

void Query::parse (const char * source, const char * tabs, Query_type type, Query_processing_order order, Query_operation arg_op_type)
{
	init_tabs_str (tabs);
	init_s_words_array (source);
	qu_type = type;
	pr_order = order;
	op_type = arg_op_type;
}

void Query::parse (const char * source, const char * tabs, const char * op_type_name, int is_for_all)
{
	flag_is_for_all = is_for_all; // Пока оставлено, дабы не переписывать обработку запроса Find, хотя по хорошему, его надо переделать под Query_processing_order.
	
	init_tabs_str (tabs);
	init_s_words_array (source);
	qu_type = Query_type::type_find;
	
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


void Query::clear_strs (void)
{
	int i;
	for (i = 0; i < s_words_length; i++)
	{
		delete[] s_words[i];
	}
	if (s_words != nullptr)
		delete[] s_words;
	if (str_tabs != nullptr)
		delete[] str_tabs;
}


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
	switch (qu_type)
	{
		case Query_type::type_regexp:
			return init_s_words_array_for_regexp (str);
			break;
		case Query_type::type_find:
			return init_s_words_array_for_find (str);
			break;
		default:
			return init_s_words_array_for_find (str);
			break;
	}
}


int Query::init_s_words_array_for_find (const char * str)
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


int Query::init_s_words_array_for_regexp (const char * str)
{
	int num_words = 0, ret;
	const char spec_symbols[] = "\\_%[]^";
	
	num_words = get_amount_of_words_for_s_array (str, str_tabs, spec_symbols);
	s_words_length = 0;
	s_words = new char* [num_words];
	if (s_words == nullptr)
		return -1;
	s_words_length = num_words;
	
	ret = to_fill_s_words_array (str, str_tabs, spec_symbols);
	if (ret)
		return ret;
	
	return 0;	
}


int Query::try_to_extract_word_into_s_words (const char * str, size_t& i, size_t& last_word_pos, size_t& m)
{
	size_t word_length = i - last_word_pos;//- 1;
	if (flag_word_or_tabs == FLAG_WORD_OR_TABS::WORD)
	{
		s_words[m] = new char [word_length + 1];
		if (s_words[m] == nullptr)
		{
			for (i = 0; i < m; i++)
				delete[] s_words[i];
			delete[] s_words;
			s_words_length = 0;
			return 1;
		}
		strncpy (s_words[m], str + last_word_pos, word_length);
		m++;
	}
	return 0;
}


int Query::get_amount_of_words_for_s_array (const char * str, const char * str_tabs, const char * spec_symbols)
{
	size_t i, j, k;
	int num_words = 0;
	
	flag_word_or_tabs = FLAG_WORD_OR_TABS::TABS;
	for (i = 0; i < strlen (str); i++)
	{
		if (str[i] == '\\')
		{
			i++;
			for (j = 0; j < strlen (str_tabs); j++)
				if (str_tabs[j] == str[i])
				{
					if (flag_word_or_tabs == FLAG_WORD_OR_TABS::WORD) // вынести в функцию
						num_words++;
					flag_word_or_tabs = FLAG_WORD_OR_TABS::TABS;
					break;
				}
			if (!(j < strlen (str_tabs)))
			{
				flag_word_or_tabs = FLAG_WORD_OR_TABS::WORD;
			}
		}
		else
		{
			for (j = 0; j < strlen (str_tabs); j++)
				if (str[i] == str_tabs[j]) 
				{
					for (k = 0; k < strlen (spec_symbols); k++)
					{
						if (str_tabs[j] == spec_symbols[k])
						{
							flag_word_or_tabs = FLAG_WORD_OR_TABS::WORD;
							break;
						}
					}
					if (!(k < strlen (spec_symbols)))
					{
						if (flag_word_or_tabs == FLAG_WORD_OR_TABS::WORD)
							num_words++;
						flag_word_or_tabs = FLAG_WORD_OR_TABS::TABS;
					}
					break;
				}
			if (!(j < strlen (str_tabs)))
				flag_word_or_tabs = FLAG_WORD_OR_TABS::WORD;
		}
	}
	if (flag_word_or_tabs == FLAG_WORD_OR_TABS::WORD)
		num_words++;
	
	return num_words;
}

int Query::to_fill_s_words_array (const char * str, const char * str_tabs, const char * spec_symbols)
{
	size_t last_word_pos = 0;
	size_t i, j, k, m;
	int ret = 0;
	
	flag_word_or_tabs = FLAG_WORD_OR_TABS::TABS;
	for (i = 0, m = 0; i < strlen (str); i++)
	{
		if (str[i] == '\\') // Экранирование, неважно какой символ - трактуется литерально.
		{
			i++;
			for (j = 0; j < strlen (str_tabs); j++)
			{
				if (str_tabs[j] == str[i])
				{
					ret = try_to_extract_word_into_s_words (str, i, last_word_pos, m);
					if (ret)
						return ret;
					last_word_pos = i + 1;
					flag_word_or_tabs = FLAG_WORD_OR_TABS::TABS;
					break;
				}
			}
			if (!(j < strlen (str_tabs)))
				flag_word_or_tabs = FLAG_WORD_OR_TABS::WORD;
		}
		else // Экранирования нет, если символ специальный, табуляционным он не считается.
		{
			for (j = 0; j < strlen (str_tabs); j++)
			{
				if (str[i] == str_tabs[j]) // Текущий символ табельный.
				{
					for (k = 0; k < strlen (spec_symbols); k++) // Если он специальный, то не считаем его табельным.
					{
						if (str_tabs[j] == spec_symbols[k])
						{
							flag_word_or_tabs = FLAG_WORD_OR_TABS::WORD;
							break;
						}
					}
					if (!(k < strlen (spec_symbols)))
					{
						ret = try_to_extract_word_into_s_words (str, i, last_word_pos, m);
						if (ret)
							return ret;
						last_word_pos = i + 1;
						flag_word_or_tabs = FLAG_WORD_OR_TABS::TABS;
					}
					break;
				}
			}
			if (!(j < strlen (str_tabs)))
				flag_word_or_tabs = FLAG_WORD_OR_TABS::WORD;
		}
	}
	
	ret = try_to_extract_word_into_s_words (str, i, last_word_pos, m);
	if (ret)
		return ret;

	return 0;
}



int Query::to_process_qu_find (const char * str)
{
	/*
	 * 
	 * TODO:
	 * Сейчас порядок обработки слов зависит от флага flag_is_for_all, в то
	 * время как нужна зависимость от pr_order.
	 * */
	Words_rider str_words (str, str_tabs);
	int str_word_length;
	int i, cmp_res;
	
	if ((qu_type == Query_type::type_find) && ((op_type == Query_operation::operation_invalid) || (op_type == Query_operation:: operation_none)))
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

int Query:: to_process_qu_regexp (const char * str)
{
	switch (pr_order)
	{
		case Query_processing_order::order_all:
			return processing_query_regexp_for_all (str);
		case Query_processing_order::order_once_enough:
			return processing_query_regexp_once_enough (str);
		case Query_processing_order::order_one_by_one:
			return processing_query_regexp_one_by_one (str);
		
		default:
			printf ("wrong pr_order\n");
			break;
	}
	return -1;
}

int Query::processing_query_regexp_for_all (const char * str)
{
	Words_rider str_words (str, str_tabs);
	int cmp_res;
	int i;
	
	while (str_words.get_word_length () != 0)
	{
		str_words.extract_word_to (str_word_buf, WORD_LEN);
		for (i = 0; i < s_words_length; i++)
		{
			cmp_res = Word_regexp::check_is_suitable (str_word_buf, s_words[i]);
			if (cmp_res == 1)
				break;
		}
		if (!(i < s_words_length)) // Т.е. попробовали все слова s, но ни одно не подошло.
			return 0;
		str_words.next();
	}
	return 1;
}

int Query::processing_query_regexp_once_enough (const char * str)
{
	Words_rider str_words (str, str_tabs);
	int cmp_res;
	int i;
	
	while (str_words.get_word_length () != 0)
	{
		str_words.extract_word_to (str_word_buf, WORD_LEN);
		for (i = 0; i < s_words_length; i++)
		{
			cmp_res = Word_regexp::check_is_suitable (str_word_buf, s_words[i]);
			if (cmp_res == 1)
				return 1;
		}
		str_words.next();
	}
	return 0;
}

int Query::processing_query_regexp_one_by_one (const char * str)
{
	Words_rider str_words (str, str_tabs);
	int cmp_res;
	int i;
	
	for (i = 0; str_words.get_word_length () != 0; str_words.next(), i++)
	{
		str_words.extract_word_to (str_word_buf, WORD_LEN);
		if (i < s_words_length)
		{
			cmp_res = Word_regexp::check_is_suitable (str_word_buf, s_words[i]);
			if (cmp_res != 1)
				return 0;
		}
		else 
			break; // Лишние слова строки из файла не рассматриваются.
	}
	return 1;
}

