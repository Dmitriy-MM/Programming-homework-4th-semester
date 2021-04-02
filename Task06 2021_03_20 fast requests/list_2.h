#ifndef LIST_2
#define LIST_2

#include <stdio.h>
// added join, set_head, restore_prev_links
/*
*	@List2 realisation:
*	private - service:
*	List_2_node<T> * merge_with (List_2_node<T> * head_A, List_2_node<T> * head_B, List_2_node<T> * last, int compare (T&, T&));
*	List_2_node<T> * merge_with (List_2_node<T> * head_A, List_2_node<T> * head_B, List_2_node<T> * last, int (*compare) (T&, T&, void *), void * compare_arg);
*	void clear_list (void);
*	void restore_prev_links (void);
*
*	Methods:
*	List_2_node<T> * get_head () const;
*	int get_length (void) const;
*	List_2_node<T> * get_tail (void) const;
*	List_2_node<T> * insert (List_2_node<T> * el);
*	List_2_node<T> * insert (T& el);
*
*	void print (FILE *fp = stdout);
*	int read (FILE *fp);
*
*	void remove (List_2_node<T> * el);
*	void cut (List_2_node<T> * el);
*	void swap (List_2<T> * second_list);
*	
*	void sort (int compare (T&, T&));
*	void sort (int (compare) (T&, T&, void *), void * compare_arg);
*
*	static void set_m (int new_m);
*	static void set_r (int new_r);
*	
*	ToDo: 
*	
*	@List_2_node:
*	swap (List_2_node<T>&);
*	List_2_node * get_next (void) const;
*	List_2_node * get_prev (void) const;
*	void set_next (List_2_node * new_next);
*	void set_prev (List_2_node * new_prev);
*/

template <class T> class List_2;
template <class T> 
class List_2_node: public T
{
private:
	List_2_node * next = nullptr;
	List_2_node * prev = nullptr;
	void remove () { delete this; }
	~List_2_node ()	{ next = nullptr; prev = nullptr; }
public:
	List_2_node () = default;
	List_2_node (const List_2_node& x): T (x) { next = nullptr;	prev = nullptr;	}
	List_2_node (List_2_node&& x): T ((T&&) x) { next = x.next; x.next = nullptr; x.prev = nullptr; }
	List_2_node& operator = (const List_2_node&);
	List_2_node& operator = (List_2_node&&);

	List_2_node * get_next (void) const	{ return this->next; }
	List_2_node * get_prev (void) const	{ return this->prev; }
	void set_next (List_2_node * new_next) { next = new_next; }
	void set_prev (List_2_node * new_prev) { prev = new_prev; }
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
	
	List_2_node<T> * merge_with (List_2_node<T> * head_A, List_2_node<T> * head_B, List_2_node<T> * last, int compare (T&, T&))
	{
		List_2_node<T> * curr_A = head_A, * curr_B = head_B, *curr = nullptr;

		int is_not_head_A_nullptr = head_A != nullptr, is_not_head_B_nullptr = head_B != nullptr;
		//Выбор головы такой объемный, потому что рассмотреть случаи, когда:
		//изначальный список (this) не пуст,
		//оба (A и B) не пусты - т.е. нужно сравнить головы и выбрать меньшую,
		//хотя бы один не пуст и взять голову из него,
		//оба пусты тогда не важно откуда брать.
		//Причем если голова взята, то нужно сдвинуть итератор по тому списку,
		//откуда она.
		if (last == nullptr)
		{
			//Выбор головы для списка из А и Б.
			if (is_not_head_A_nullptr && is_not_head_B_nullptr)
			{
				if (compare (*head_A, *head_B) <= 0)
				{
					this->head = head_A;
					curr_A = head_A->next;
				}			
				else
				{
					this->head = head_B;
					curr_B = head_B->next;
				}
			}
			else if (is_not_head_A_nullptr)
			{
				this->head = head_A;
				curr_A = head_A->next;
			}
			else
			{
				this->head = head_B;
				curr_B = head_B->next;
			}
			curr = this->head;
		}
		else
			//Список изначально не пуст.
			curr = last;
		
		//Само слияние.
		while ((curr_A != nullptr) && (curr_B != nullptr))
		{
			if (compare (*curr_A, *curr_B) <= 0)
			{
				curr = curr->next = curr_A;
				curr_A = curr_A->next;
			}
			else
			{
				curr = curr->next = curr_B;
				curr_B = curr_B->next;
			}
		}
		while (curr_A != nullptr)
		{
			curr = curr->next = curr_A;
			curr_A = curr_A->next;
		}
		while (curr_B != nullptr)
		{
			curr = curr->next = curr_B;
			curr_B = curr_B->next;
		}
		curr->next = nullptr;
		return curr;
	}
	List_2_node<T> * merge_with (List_2_node<T> * head_A, List_2_node<T> * head_B, List_2_node<T> * last, int (*compare) (T&, T&, void *), void * compare_arg)
	{
		List_2_node<T> * curr_A = head_A, * curr_B = head_B, *curr = nullptr;
		int is_not_head_A_nullptr = head_A != nullptr, is_not_head_B_nullptr = head_B != nullptr;
		if (last == nullptr)
		{
			if (is_not_head_A_nullptr && is_not_head_B_nullptr)
			{
				if (compare(*head_A, *head_B, compare_arg) <= 0)
				{
					this->head = head_A;
					curr_A = head_A->next;
				}			
				else
				{
					this->head = head_B;
					curr_B = head_B->next;
				}
			}
			else if (is_not_head_A_nullptr)
			{
				this->head = head_A;
				curr_A = head_A->next;
			}
			else
			{
				this->head = head_B;
				curr_B = head_B->next;
			}
			curr = this->head;
		}
		else
			curr = last;
		
		while ((curr_A != nullptr) && (curr_B != nullptr))
		{
			if (compare (*curr_A, *curr_B, compare_arg) <= 0)
			{
				curr = curr->next = curr_A;
				curr_A = curr_A->next;
			}
			else
			{
				curr = curr->next = curr_B;
				curr_B = curr_B->next;
			}
		}
		while (curr_A != nullptr)
		{
			curr = curr->next = curr_A;
			curr_A = curr_A->next;
		}
		while (curr_B != nullptr)
		{
			curr = curr->next = curr_B;
			curr_B = curr_B->next;
		}
		curr->next = nullptr;
		return curr;
	}
	
public:
	enum RET_CODES {
		SUCCESS = 0,
		INPUT_NON_ZERO_PTR_ERROR,
		OPEN_FILE_ERROR,
		READ_HEAD_ERROR,
		NO_REACH_EOF_ERROR,
		BAD_FORMAT_ERROR,
		MEMORY_ERROR,
	};
	List_2 () = default;
	~List_2 () { clear_list ();	}
	List_2 (List_2 && list)	{ this->head = list.head; list.head = nullptr; }
	
	List_2_node<T> * get_head () const { return head; }
	void set_head (List_2_node<T> * new_head) { head = new_head; }
	void restore_prev_links (void)
	{
		List_2_node<T> * cur, * prev = nullptr;
		for (cur = head; cur != nullptr; cur = cur->next)
		{
			cur->prev = prev;
			prev = cur;
		}
	}
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
		if (head == nullptr)
			return MEMORY_ERROR;
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
			if (tmp == nullptr)
			{
				clear_list ();
				return MEMORY_ERROR;
			}
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

	List_2_node<T> * get_tail (void) const {
		List_2_node<T> * cur;
		for (cur = head; (cur != nullptr) && (cur->next != nullptr); cur = cur->next)
		{}
		return cur;
	}
	
	List_2_node<T> * insert (List_2_node<T> * el)
	{
		List_2_node<T> * tail, * curr;
		if (head == nullptr)
		{
			head = el;
			head->prev = nullptr;
			head->next = nullptr;
			return head;
		}
		for (curr = head; curr != nullptr; curr = curr->next)
		{
			if (((T&) *el) == ((T&) *curr))
				return nullptr;
			tail = curr;
		}
		tail->next = el;
		el->prev = tail;
		el->next = nullptr;
		return el;
	}
	List_2_node<T> * insert (T& el)
	{
		List_2_node<T> * tail, * curr;
		if (head == nullptr)
		{
			head = new List_2_node<T> ((List_2_node<T>&&)el);
			head->prev = nullptr;
			head->next = nullptr;
			return head;
		}
		for (curr = head; curr != nullptr; curr = curr->next)
		{
			if (((T&)el) == ((T&) *curr))
				return nullptr;
			tail = curr;
		}
		curr = new List_2_node<T> ((List_2_node<T>&&)el);
		tail->next = curr;
		curr->prev = tail;
		curr->next = nullptr;
		return curr;
	}
	void join (List_2_node<T> * el)
	{
		List_2_node<T> * tail, * curr;
		if (el == nullptr)
			return;
		if (head == nullptr)
		{
			head = el;
			head->prev = nullptr;
			return;
		}
		for (curr = head; (curr != nullptr) && (curr->next != nullptr); curr = curr->next)
		{}
		tail = curr;
		tail->next = el;
		el->prev = tail;
	}
	
	void remove (List_2_node<T> * el) 
	{ 
		if (el == nullptr)
			return;
		if (el->prev != nullptr)
			el->prev->next = el->next;
		if (el->next != nullptr)
			el->next->prev = el->prev;
		if (el == head)
			head = head->next;
		el->remove (); 
	}
	void cut (List_2_node<T> * el)
	{
		if (el == nullptr)
			return;
		if (el->prev != nullptr)
			el->prev->next = el->next;
		if (el->next != nullptr)
			el->next->prev = el->prev;
		if (el == head)
			head = head->next;
		el->prev = nullptr;
		el->next = nullptr;
	}

	void swap (List_2<T> * second_list)
	{
		List_2_node<T> * tmp = second_list->head;
		second_list->head = this->head;
		this->head = tmp;
	}

	//Сортировка слиянием.
	void sort (int compare (T&, T&))
	{
		List_2 * second_list = new List_2;
		List_2_node<T> * head_list_A = nullptr, * head_list_B = nullptr, *curr = nullptr, *curr_A, *curr_B;
		List_2_node<T> * last = nullptr;
		int length, block_size = 1, i;
		
		// (void) compare;
		length = this->get_length ();
		
		while (block_size < length)
		{
			curr = this->head;
			last = nullptr;
			while (curr != nullptr)
			{
				//Отрезаем два куска размера block_size от списка.
				head_list_A = curr_A = curr;
				if (curr != nullptr)
					curr = curr->next;

				for (i = 1; (i < block_size) && (curr != nullptr); i++)
				{
					curr_A = curr_A->next = curr;
					curr = curr->next;
				}
				if (curr_A != nullptr)
					curr_A->next = nullptr;
				
				head_list_B = curr_B = curr;
				if (curr != nullptr)
					curr = curr->next;
				for (i = 1; (i < block_size) && (curr != nullptr); i++)
				{
					curr_B = curr_B->next = curr;
					curr = curr->next;
				}
				if (curr_B != nullptr)
					curr_B->next = nullptr;
				this->head = curr;
				
				last = second_list->merge_with (head_list_A, head_list_B, last, compare);
			}
			this->swap (second_list);
			second_list->head = nullptr;
			block_size *= 2;
		}
		restore_prev_links ();
		delete second_list;
	}
			
	//Сортировка слиянием.
	void sort (int (compare) (T&, T&, void *), void * compare_arg)
	{
		List_2 * second_list = new List_2;
		List_2_node<T> * head_list_A = nullptr, * head_list_B = nullptr, *curr = nullptr, *curr_A, *curr_B;
		List_2_node<T> * last = nullptr;
		int length, block_size = 1, i;
		
		// (void) compare;
		length = this->get_length ();
		
		while (block_size < length)
		{
			curr = this->head;
			last = nullptr;
			while (curr != nullptr)
			{
				//Отрезаем два куска размера block_size от списка.
				head_list_A = curr_A = curr;
				if (curr != nullptr)
					curr = curr->next;

				for (i = 1; (i < block_size) && (curr != nullptr); i++)
				{
					curr_A = curr_A->next = curr;
					curr = curr->next;
				}
				if (curr_A != nullptr)
					curr_A->next = nullptr;
				
				head_list_B = curr_B = curr;
				if (curr != nullptr)
					curr = curr->next;
				for (i = 1; (i < block_size) && (curr != nullptr); i++)
				{
					curr_B = curr_B->next = curr;
					curr = curr->next;
				}
				if (curr_B != nullptr)
					curr_B->next = nullptr;
				this->head = curr;
				
				last = second_list->merge_with (head_list_A, head_list_B, last, compare, compare_arg);
			}
			this->swap (second_list);
			second_list->head = nullptr;
			block_size *= 2;
		}
		restore_prev_links ();
		delete second_list;
	}
};
#endif // LIST2.
