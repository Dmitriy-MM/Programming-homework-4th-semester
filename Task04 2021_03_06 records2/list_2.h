#ifndef LIST_2
#define LIST_2

#include <stdio.h>

template <class T> class List_2;
template <class T> 
class List_2_node: public T
{
private:
	List_2_node * next = nullptr;
	List_2_node * prev = nullptr;
public:
	List_2_node () = default;
	List_2_node (const List_2_node& x): T (x) { next = nullptr;	prev = nullptr;	}
	List_2_node (List_2_node&& x): T ((T&&) x) { next = x.next; x.next = nullptr;	x.prev = nullptr; }
	~List_2_node ()	{ next = nullptr; prev = nullptr; }
	List_2_node& operator = (const List_2_node&);
	List_2_node& operator = (List_2_node&&);

	List_2_node * get_next (void) const	{ return this->next; }
	void set_next (List_2_node * new_next) { next = new_next; }
	
	friend class List_2<T>;
};

template <class T>
class List_2
{
private:
	List_2_node<T> * head = nullptr;
	static int m;
	static int r;
	
	void clear_list (void)
	{
		List_2_node<T> * next;
		while (head != nullptr)
		{
			next = head->get_next ();
			delete head;
			head = next;
		}
		head = nullptr;
	}
public:
	enum RET_CODES {
		SUCCESS = 0,
		INPUT_NON_ZERO_PTR_ERROR,
		OPEN_FILE_ERROR,
		READ_HEAD_ERROR,
		NO_REACH_EOF_ERROR,
		BAD_FORMAT_ERROR
	};
	List_2 () = default;
	~List_2 () { clear_list ();	}
	List_2 (List_2 && list)	{ this->head = list.head; list.head = nullptr; }
	
	List_2_node<T> * get_head () const { return head; }
	int get_length (void) const
	{
		List_2_node<T> * cur = head;
		int i;
		for (i = 0; cur != nullptr; cur = cur->next)
			i++;
		return i;
	}
	static void set_m (int new_m) {	m = new_m; }
	static void set_r (int new_r) {	r = new_r; }
	
	int read (FILE *fp)
	{
		List_2_node<T> *tail = nullptr, *tmp = nullptr;
		int ret_code, i = 0;
		
		if (head != nullptr)
			return RET_CODES::INPUT_NON_ZERO_PTR_ERROR;
		if (m < 0)
			return SUCCESS;
		
		head = new List_2_node<T>;
		ret_code = head->read (fp);
		if (ret_code != 0)
		{
			delete head;
			head = nullptr;
			return RET_CODES::READ_HEAD_ERROR;
		}
		i++;
		
		for (tail = head; (i < m) || (m == 0); tail = tail->next, i++)
		{
			tmp = new List_2_node<T>;
			ret_code = tmp->read (fp);
			if (ret_code != 0)
			{
				delete tmp;
				if (feof (fp))
					break;
				clear_list ();
				return RET_CODES::BAD_FORMAT_ERROR;
			} 
			tail->next = tmp, tmp->prev = tail;
		}
		tmp = nullptr;

		if ((!feof (fp)) && (i < m))
		{
			clear_list ();
			return RET_CODES::NO_REACH_EOF_ERROR;
		}
		return RET_CODES::SUCCESS;
	}
	
	void print (FILE *fp = stdout)
	{
		List_2_node<T> * cur = head;
		int i;
		for (i = 0; (cur != nullptr) && ((r != 0) && (i < r)); cur = cur->get_next(), i++)
		{
			fprintf (fp, "%d) ", i);
			cur->print (fp);
		}
		if ((cur != nullptr) && (r != 0))
		{
			fprintf (fp, "...\n");
			for ( ; (cur->get_next() != nullptr); i++, cur = cur->get_next()) {}
			fprintf (fp, "%d) ", i);
			cur->print (fp);
		}
	}
};
#endif // LIST2.
