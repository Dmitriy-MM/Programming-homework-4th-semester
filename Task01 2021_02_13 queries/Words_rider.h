#ifndef WORDS_RIDER
#define WORDS_RIDER

#include <cstring>

class Words_rider {
private:

	const char * str_source;
	const char * str_tabs;
	char * position;
	int num_of_cur_word;
	int is_initiated;
	
	void init (const char * str_source, const char * str_tabs)
	{
		this->str_source = str_source;
		this->str_tabs = str_tabs;
		position = (char *) str_source + (int) (strspn (str_source, str_tabs));
		num_of_cur_word = 0;
		is_initiated = 1;
	}
public:
	Words_rider ()
	{
		position = nullptr;
		num_of_cur_word = -1;
		is_initiated = 0;
	}
	Words_rider (const char * arg_str_source, const char * arg_str_tabs)
	{
		init (arg_str_source, arg_str_tabs);
	}
	~Words_rider()
	{
		str_source = str_tabs = nullptr;
		position = nullptr;
		num_of_cur_word = 0;
	}
	
	inline char * get_pos () const { return position; }
	inline int get_word_length () const { return (int) (strcspn (position, str_tabs)); }
	inline int get_cur_num () const { return num_of_cur_word; }
	
	void next ()
	{
		position = position + get_word_length () + (int) (strspn (position + get_word_length (), str_tabs));
		num_of_cur_word++;
	}
	void reset_pos ()
	{
		position = (char *) str_source + (int) (strspn (str_source, str_tabs));
		num_of_cur_word = 0;
	}
	void extract_word_to (char * buf, int buf_len)
	{
		int word_length = get_word_length ();
		word_length = (buf_len < word_length)? buf_len: word_length;
		strncpy (buf, position, word_length);
		buf[word_length] = '\0';
	}
	void extract_N_word_to (int n, char * buf, int buf_len)
	{
		Words_rider rider (str_source, str_tabs);
		int i;
		for (i = 0; i < n; i++)
			rider.next ();
		rider.extract_word_to (buf, buf_len);
	}
	void extract_tabs_after_word (char * buf, int buf_len)
	{
		char * tabs_position = position + strcspn (position, str_tabs);
		int tabs_length = strspn (tabs_position, str_tabs);
		tabs_length = (buf_len < tabs_length)? buf_len: tabs_length;
		
		strncpy (buf, tabs_position, tabs_length);
		buf[tabs_length] = '\0';
	}
	friend class Query;
};

#endif // WORDS_RIDER.