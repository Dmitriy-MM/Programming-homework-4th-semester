#ifndef QUERY
#define QUERY

#include <cstring>

#include "Words_rider.h"
#include "Query_operation_declares.h"
#include "Query_types_declares.h"
#include "Query_processing_order_declares.h"

#define WORD_LEN 1234


class Query {
private:
	Query_operation op_type = Query_operation::operation_none;
	Query_type qu_type = Query_type::type_none;
	Query_processing_order pr_order = Query_processing_order::order_once_enough;

	int flag_is_for_all = 0;
	char str_word_buf[WORD_LEN] = {'\0'};
	char * str_tabs = nullptr;
	char ** s_words = nullptr;
	int s_words_length = 0;
	enum class FLAG_WORD_OR_TABS {
		WORD,
		TABS
	};
	FLAG_WORD_OR_TABS flag_word_or_tabs = FLAG_WORD_OR_TABS::TABS;
	
	void clear_strs (void);
	
	// Strings service functions.
	int init_tabs_str (const char *);
	
	
	int init_s_words_array (const char *);
	int init_s_words_array_for_find (const char *);
	int init_s_words_array_for_regexp (const char *);
	int try_to_extract_word_into_s_words (const char * str, size_t& i, size_t& last_word_pos, size_t& m);
	int get_amount_of_words_for_s_array (const char * str, const char * str_tabs, const char * spec_symbols);
	int to_fill_s_words_array (const char * str, const char * str_tabs, const char * spec_symbols);
	
	// Braching of query.
	int to_process_qu_regexp (const char * str);
	int processing_query_regexp_for_all (const char * str);
	int processing_query_regexp_once_enough (const char * str);
	int processing_query_regexp_one_by_one (const char * str);
	
	int to_process_qu_find (const char * str);
	
public:
	Query () = default;
	~Query () {
		clear_strs ();
	}
	
	void parse (const char * source, const char * tabs, Query_type type, Query_processing_order order, Query_operation arg_op_type = Query_operation::operation_none);
	void parse (const char * source, const char * tabs, const char * op_type, int is_for_all = 0);
	void set_processing_order (Query_processing_order order)
	{
		pr_order = order;
	}
	int apply_to (const char * str);
};
#endif // QUERY.
