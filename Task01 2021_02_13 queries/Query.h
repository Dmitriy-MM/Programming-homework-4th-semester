#ifndef QUERY
#define QUERY

#include <cstring>

#include "Words_rider.h"
#include "Query_operation_declares.h"

#define WORD_LEN 1234


class Query {
private:
	Query_operation op_type = Query_operation::operation_none;
	int flag_is_for_all = 0;
	char str_word_buf[WORD_LEN] = {'\0'};
	char * str_tabs = nullptr;
	char ** s_words = nullptr;
	int s_words_length = 0;
	
	int init_tabs_str (const char *);
	int init_s_words_array (const char *);
public:
	Query () = default;
	~Query () {
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
	
	void parse (const char * source, const char * tabs, const char * op_type, int is_for_all = 0);
	int apply_to (const char * str);
};
#endif // QUERY.
