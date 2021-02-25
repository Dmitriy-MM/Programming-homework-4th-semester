#ifndef WORDS_RIDER
#define WORDS_RIDER

#include <cstring>

class Words_rider {
private:
	const char * str_source = nullptr;
	const char * str_tabs = nullptr;
	const char * position = nullptr;
	const char * spec_symbols = nullptr;
	int num_of_cur_word = -1;
	
	
	Words_rider () = default; // Explicit prohibition of the default constructor.
	void init (const char * str_source, const char * str_tabs)
	{
		this->str_source = str_source;
		this->str_tabs = str_tabs;
		position = str_source + (int) (strspn (str_source, str_tabs));
		num_of_cur_word = 0;
	}
public:
	Words_rider (const char * arg_str_source, const char * arg_str_tabs, const char * arg_spec_symbols) : Words_rider (arg_str_source, arg_str_tabs)
	{
		set_spec_symbols (arg_spec_symbols);
	}
	Words_rider (const char * arg_str_source, const char * arg_str_tabs) : Words_rider ()
	{
		init (arg_str_source, arg_str_tabs);
	}
	~Words_rider()
	{
		str_source = str_tabs = nullptr;
		position = nullptr;
		num_of_cur_word = -1;
	}
	
	void set_spec_symbols (const char * s) { spec_symbols = s; }
	const char * get_pos () const { return position; }
	int get_word_length () const { return (int) (strcspn (position, str_tabs)); }
	int get_cur_num () const { return num_of_cur_word; }
	
	void next ()
	{
		position = position + get_word_length () + (int) (strspn (position + get_word_length (), str_tabs));
		num_of_cur_word++;
	}
	void reset_pos ()
	{
		position = str_source + (int) (strspn (str_source, str_tabs));
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
		const char * tabs_position = position + strcspn (position, str_tabs);
		int tabs_length = strspn (tabs_position, str_tabs);
		tabs_length = (buf_len < tabs_length)? buf_len: tabs_length;
		
		strncpy (buf, tabs_position, tabs_length);
		buf[tabs_length] = '\0';
	}
	friend class Query;
};

#endif // WORDS_RIDER.
