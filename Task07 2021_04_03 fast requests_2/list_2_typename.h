#ifndef LIST_2_TYPENAME
#define LIST_2_TYPENAME

#include <stdio.h>

/*
 * List_2t container for built-in types.
 * 
 */

template <typename T> class List_2t;
template <typename T> 
class List_2t_node
{
private:
	// Properties.
	T data;
	List_2t_node * next = nullptr;
	List_2t_node * prev = nullptr;
	
	// Constructors.
	~List_2t_node ()	{ next = nullptr; prev = nullptr; }
	
	// Methods.
	void remove () { delete this; }

public:
	// Properties.
	// Constructors.
	List_2t_node () = default;
	List_2t_node (const List_2t_node& x) : List_2t_node () { data = x.data; }
	List_2t_node (T data) : List_2t_node () { this->data = data; }
	List_2t_node (List_2t_node&& x) {
		data = x.data;
		next = x.next;
		prev = x.prev; 
		x.next = nullptr; 
		x.prev = nullptr;
	}
	
	// Operators.
	List_2t_node& operator = (const List_2t_node& x) {
		data = x.data;
		return *this;
	}
	List_2t_node& operator = (List_2t_node&& x) {
		data = x.data;
		next = x.next;
		prev = x.prev; 
		x.next = nullptr; 
		x.prev = nullptr;
		return *this;
	}
	List_2t_node& operator = (const T& new_data) { data = new_data; return *this; }
	//List_2t_node& operator = (const T& new_data) = delete;

	// Methods.
	List_2t_node * get_next (void) const	{ return this->next; }
	List_2t_node * get_prev (void) const	{ return this->prev; }
	T get (void) const { return data; }
	void set (T new_data) { data = new_data; }
	void set_next (List_2t_node * new_next) { next = new_next; }
	void set_prev (List_2t_node * new_prev) { prev = new_prev; }
	friend class List_2t<T>;
};

template <typename T>
class List_2t
{
private:
	List_2t_node<T> * head = nullptr;
	
	void clear_list (void)
	{
		List_2t_node<T> * next;
		while (head != nullptr)
		{
			next = head->get_next ();
			delete head;
			head = next;
		}
		head = nullptr;
	}
	
	List_2t_node<T> * merge_with (List_2t_node<T> * head_A, List_2t_node<T> * head_B, List_2t_node<T> * last, int (*compare) (T, T, void *), void * compare_arg)
	{
		List_2t_node<T> * curr_A = head_A, * curr_B = head_B, *curr = nullptr;
		int is_not_head_A_nullptr = head_A != nullptr, is_not_head_B_nullptr = head_B != nullptr;
		if (last == nullptr)
		{
			if (is_not_head_A_nullptr && is_not_head_B_nullptr)
			{
				if (compare (head_A->get (), head_B->get(), compare_arg) <= 0)
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
			if (compare (curr_A->get (), curr_B->get (), compare_arg) <= 0)
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
	// Properties.
	enum RET_CODES {
		SUCCESS = 0,
		INPUT_NON_ZERO_PTR_ERROR,
		OPEN_FILE_ERROR,
		READ_HEAD_ERROR,
		NO_REACH_EOF_ERROR,
		BAD_FORMAT_ERROR,
		MEMORY_ERROR,
	};
	
	// Constructors.
	List_2t () = default;
	~List_2t () { clear_list ();	}
	List_2t (List_2t && list)	{ this->head = list.head; list.head = nullptr; }
	
	// Methods.
	List_2t_node<T> * get_head () const { return head; }
	void set_head (List_2t_node<T> * new_head) { head = new_head; }
	
	void restore_prev_links (void)
	{
		List_2t_node<T> * cur, * prev = nullptr;
		for (cur = head; cur != nullptr; cur = cur->next)
		{
			cur->prev = prev;
			prev = cur;
		}
	}
	
	int get_length (void) const
	{
		List_2t_node<T> * cur = head;
		int i;
		for (i = 0; cur != nullptr; cur = cur->next)
			i++;
		return i;
	}
	


	List_2t_node<T> * get_tail (void) const {
		List_2t_node<T> * cur;
		for (cur = head; (cur != nullptr) && (cur->next != nullptr); cur = cur->next)
		{}
		return cur;
	}
	
	List_2t_node<T> * uniq_insert (List_2t_node<T> * el)
	{
		List_2t_node<T> * tail, * curr;
		if (head == nullptr)
		{
			head = el;
			head->prev = nullptr;
			head->next = nullptr;
			return head;
		}
		for (curr = head; curr != nullptr; curr = curr->next)
		{
			if (el->get () == curr->get ())
				return nullptr;
			tail = curr;
		}
		tail->next = el;
		el->prev = tail;
		el->next = nullptr;
		return el;
	}
	List_2t_node<T> * uniq_insert (T el)
	{
		List_2t_node<T> * tail, * curr;
		if (head == nullptr)
		{
			head = new List_2t_node<T> (el);
			if (head == nullptr)
				return nullptr;
			head->prev = nullptr;
			head->next = nullptr;
			return head;
		}
		for (curr = head; curr != nullptr; curr = curr->next)
		{
			if (el == curr->get ())
				return nullptr;
			tail = curr;
		}
		curr = new List_2t_node<T> (el);
		if (curr == nullptr)
			return nullptr;
		tail->next = curr;
		curr->prev = tail;
		curr->next = nullptr;
		return curr;
	}
	
	List_2t_node<T> * insert (List_2t_node<T> * el)
	{
		List_2t_node<T> * tail, * curr;
		if (head == nullptr)
		{
			head = el;
			head->prev = nullptr;
			head->next = nullptr;
			return head;
		}
		for (curr = head; (curr != nullptr) && (curr->next != nullptr); curr = curr->next) 
		{}
		tail = curr;
		tail->next = el;
		el->prev = tail;
		el->next = nullptr;
		return el;
	}
	List_2t_node<T> * insert (T el)
	{
		List_2t_node<T> * tail, * curr;
		if (head == nullptr)
		{
			head = new List_2t_node<T> (el);
			if (head == nullptr)
				return nullptr;
			head->prev = nullptr;
			head->next = nullptr;
			return head;
		}
		for (curr = head; (curr != nullptr) && (curr->next != nullptr); curr = curr->next) 
		{}
		tail = curr;
		curr = new List_2t_node<T> (el);
		if (curr == nullptr)
			return nullptr;
		tail->next = curr;
		curr->prev = tail;
		curr->next = nullptr;
		return curr;
	}
	
	void join (List_2t_node<T> * el)
	{
		List_2t_node<T> * tail, * curr;
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
	
	void remove (List_2t_node<T> * el) 
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
	void cut (List_2t_node<T> * el)
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

	void swap (List_2t<T> * second_list)
	{
		List_2t_node<T> * tmp = second_list->head;
		second_list->head = this->head;
		this->head = tmp;
	}
	
	void sort (int (compare) (T, T, void *), void * compare_arg)
	{
		List_2t * second_list = new List_2t;
		List_2t_node<T> * head_list_A = nullptr, * head_list_B = nullptr, *curr = nullptr, *curr_A, *curr_B;
		List_2t_node<T> * last = nullptr;
		int length, block_size = 1, i;
		
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
#endif // LIST_2_TYPENAME.
