#ifndef QUERY
#define QUERY

#include <cstring>

#include "Words_rider.h"
#include "Query_operation_declares.h"

#define WORD_LEN 1234

class Query {
private:
	Query_operation op_type;
	Words_rider query_words;
	int flag_is_for_all;
	char str_word_buf[WORD_LEN] = {'\0'};
	char query_word_buf[WORD_LEN] = {'\0'};
public:
	Query () { op_type = Query_operation::operation_none; }
	~Query () { op_type = Query_operation::operation_none; }
	void parse (const char * source, const char * tabs, const char * op_type, int is_for_all = 0);
	int apply_to (const char * str);
};
#endif // QUERY.