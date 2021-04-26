#ifndef DATABASE
#define DATABASE
#include <stdio.h>
//#include "avl.h"
#include "avl_t.h"
#include "list_2.h"
#include "record.h"

#define CHANGES 1



static unsigned long int avl_called = 0;

class Database_link_node {
private:
	void clean (void) {
		name_avl_node = nullptr;
		name_avl_container = nullptr;
		phone_avl_node = nullptr;
		phone_avl_container = nullptr;
		owner = nullptr;
		owner_container = nullptr;
		myself = nullptr;
		myself_container = nullptr;
	}
public:
	// Properties.
	List_2_node<Record> * owner = nullptr;
	List_2<Record> * owner_container = nullptr;
	
	AVLt_tree_node<Database_link_node*> * name_avl_node = nullptr;
	AVLt_tree<Database_link_node*> * name_avl_container = nullptr;
	
	AVLt_tree_node<Database_link_node*> * phone_avl_node = nullptr;
	AVLt_tree<Database_link_node*> * phone_avl_container = nullptr;
	
	List_2_node<Database_link_node> * myself = nullptr;
	List_2<Database_link_node> * myself_container = nullptr;
	// Construtors.
	Database_link_node () = default;
	~Database_link_node () { clean ();}
	int operator== (Database_link_node x) {
		return (myself == x.myself);
	}
	
	void check (void) { //Отладочная.
		if (name_avl_node->get() != myself) {
			printf ("name_avl_node->get() != myself\n");
			owner->print (nullptr, stdout);
		}
	}
	void remove (void) {
		if (phone_avl_node == nullptr) {
			printf ("AVL_NODE_PHONE == NULLPTR");
		}
		else {
			if (phone_avl_container == nullptr) {
				printf ("phone avl container nullptr");
			}
			else {
				phone_avl_container->remove (phone_avl_node);
			}
		}
		if (name_avl_node == nullptr) {
			printf ("AVL_NODE == NULLPTR");
		}
		else {
			if (name_avl_container == nullptr) {
				printf ("name avl container nullptr");
			}
			else {
				name_avl_container->remove (name_avl_node);
			}
		}
		if (owner == nullptr) {
			printf ("OWNER == NULLPTR!!");
		}
		else {
			if (owner_container == nullptr) {
				printf ("owner container == nullptr");
			}
			else {
				owner_container->remove (owner);
			}
		}
		myself_container->remove (myself);
		//clean ();
	}
};


class Database {
private:
	// Properties.
	List_2<Record> * base = nullptr;
	AVLt_tree<Database_link_node*> ** find_name_struct = nullptr;
	AVLt_tree<Database_link_node*> ** find_phone_struct = nullptr;
	List_2<Database_link_node> * cross_refs = nullptr;

	int hash_k = 0; // Сумма букв имени модулю k.
	int hash_p = 0; // p первых цифр телефона.
			
	// Constructors.
	Database () = default; // Explicit prohibition.
	
	// Methods.
	static int func_hash_name (const char * name, int k) {
		int res = 0;
		if (name == nullptr)
			return res;
		for (int i = 0; name[i]; i++)
			res = (res + name[i]) % k;
		return res;
	}
	static int func_hash_phone (int phone, int k) {
		// Return res from [0; 10^k-1].
		const int Digit = 10;
		int res = 0, denominator = 1;
		for (int i = 0; i < k; i++) {
			denominator *= Digit;
		}
		res = phone % denominator;
		return res;
	}
	
	static int calc_dimension_hash_p (int hash_p) {
		int res = 1;
		for (int i = 0; i < hash_p; i++) {
			res *= 10;
		}
		return res;
	}
	
	int clear_all_ptrs (int error_code = 0) {
		int dimension_hash_p = 0;
		dimension_hash_p = calc_dimension_hash_p (hash_p);
		if (cross_refs != nullptr) {
			delete cross_refs;
			cross_refs = nullptr;
		}
		if (base != nullptr) {
			delete base;
			base = nullptr;
		}
		if (find_name_struct != nullptr) {
			for (int i = 0; i < hash_k; i++)
				if (find_name_struct[i] != nullptr) {
					delete find_name_struct[i];
					find_name_struct[i] = nullptr;
				}
			delete[] find_name_struct;
			find_name_struct = nullptr;
		}
		if (find_phone_struct != nullptr) {
			for (int i = 0; i < dimension_hash_p; i++)
				if (find_phone_struct[i] != nullptr) {
					delete find_phone_struct[i];
					find_phone_struct[i] = nullptr;
				}
			delete[] find_phone_struct;
			find_phone_struct = nullptr;
		}
		
		return error_code;
	}
public:
	// Properties.
	void * arg_func_avl_cmp = nullptr;
	void * arg_func_avl_phone_cmp = nullptr;
	// Constructors.
	Database (int k, int p): Database () {
		this->hash_k = k;
		this->hash_p = p;
	}
	~Database () {
		clear_all_ptrs ();
	}
	
	// Methods.
	static int func_avl_cmp (Database_link_node* p1, Database_link_node* p2, void * cmp_arg = nullptr) {
		(void) cmp_arg;
		avl_called++;
		if (!(p1->owner->get_name () != nullptr && p2->owner->get_name () != nullptr))
			return 0;
		return strcmp (p1->owner->get_name (), p2->owner->get_name ());
	}
	
	static int func_phone_avl_cmp (Database_link_node* p1, Database_link_node* p2, void * cmp_arg = nullptr) {
		(void) cmp_arg;
		return p1->owner->get_phone () - p2->owner->get_phone ();
	}
	
	static unsigned long int get_called () { return avl_called; }
	int read (FILE *fp) {
		AVLt_tree_node<Database_link_node*> * tmp_n = nullptr, * tmp_p = nullptr;
		int ret_code, index_n, index_p;
		int dimension_hash_p;
		enum {
			SUCCESS = 0,
			MEMORY_ERROR,
		};
		dimension_hash_p = calc_dimension_hash_p (hash_p);
		
		this->base = new List_2<Record>;
		if (this->base == nullptr)
			return clear_all_ptrs (MEMORY_ERROR);
		ret_code = base->read (fp);
		if (ret_code)
			return clear_all_ptrs (ret_code);
		this->cross_refs = new List_2<Database_link_node>;
		if (this->cross_refs == nullptr)
			return clear_all_ptrs (MEMORY_ERROR);
		
		
		this->find_name_struct = new AVLt_tree<Database_link_node*>* [hash_k];
		if (find_name_struct == nullptr)
			return clear_all_ptrs (MEMORY_ERROR);
		for (int i = 0; i < hash_k; i++)
			this->find_name_struct[i] = nullptr;
		for (int i = 0; i < hash_k; i++)
		{
			this->find_name_struct[i] = new AVLt_tree<Database_link_node*> (func_avl_cmp, arg_func_avl_cmp);
			if (this->find_name_struct[i] == nullptr)
				return clear_all_ptrs (MEMORY_ERROR);
		}
		
		this->find_phone_struct = new AVLt_tree<Database_link_node*>* [dimension_hash_p];
		if (find_phone_struct == nullptr)
			return clear_all_ptrs (MEMORY_ERROR);
		for (int i = 0; i < dimension_hash_p; i++)
			this->find_phone_struct[i] = nullptr;
		for (int i = 0; i < dimension_hash_p; i++)
		{
			this->find_phone_struct[i] = new AVLt_tree<Database_link_node*> (func_phone_avl_cmp, arg_func_avl_phone_cmp);
			if (this->find_phone_struct[i] == nullptr)
				return clear_all_ptrs (MEMORY_ERROR);
		}
		
		for (List_2_node<Record> * cur = base->get_head (); cur != nullptr; cur = cur->get_next ())
		{
			List_2_node<Database_link_node> * link_node_tmp = new List_2_node<Database_link_node>;
			if (link_node_tmp == nullptr) 
				return clear_all_ptrs (MEMORY_ERROR);
			
			index_n = func_hash_name (cur->get_name (), hash_k);
			index_p = func_hash_phone (cur->get_phone (), hash_p);
			tmp_n = new AVLt_tree_node<Database_link_node*>;
			if (tmp_n == nullptr)
				return clear_all_ptrs (MEMORY_ERROR);
			tmp_n->set (link_node_tmp);
			tmp_p = new AVLt_tree_node<Database_link_node*>;
			if (tmp_p == nullptr)
				return clear_all_ptrs (MEMORY_ERROR);
			tmp_p->set (link_node_tmp);
			
			// Filling link_node_tmp_n.
			link_node_tmp->owner = cur;
			link_node_tmp->owner_container = this->base;
			link_node_tmp->name_avl_node = tmp_n;
			link_node_tmp->name_avl_container = this->find_name_struct[index_n];
			link_node_tmp->phone_avl_node = tmp_p;
			link_node_tmp->phone_avl_container = this->find_phone_struct[index_p];
			link_node_tmp->myself = link_node_tmp;
			link_node_tmp->myself_container = cross_refs;
			
			
			// ОТЛАДКА ЧЕК
			//link_node_tmp_n->check ();
			this->cross_refs->inject (link_node_tmp);
			this->find_name_struct[index_n]->insert (tmp_n);
			this->find_phone_struct[index_p]->insert (tmp_p);
			// ОТЛАДКА ЧЕК
			//link_node_tmp_n->check ();
			
			link_node_tmp = nullptr;
			tmp_n = nullptr;
			tmp_p = nullptr;
		}
		return ret_code;
	}

	List_2<Record> * get_base (void) const {
		return base;
	}
	AVLt_tree<Database_link_node*> ** get_find_name_struct (void) const {
		return find_name_struct;
	}
	AVLt_tree<Database_link_node*> ** get_find_phone_struct (void) const {
		return find_phone_struct;
	}
	int get_hash_k (void) const {
		return hash_k;
	}
	int get_hash_p (void) const {
		return hash_p;
	}
	
	int insert (Record& new_record) {
#define FAST_BUT_RESQUE
		AVLt_tree_node<Database_link_node*> * tmp_n, * tmp_p;
		List_2_node<Record> * ptr;
		List_2_node<Database_link_node> * link_node_tmp;
		enum { SUCCESS = 0, MEMORY_ERROR, ELEM_ALREADY_EXIST };
		int index_n = func_hash_name (new_record.get_name (), hash_k);
		int index_p = func_hash_phone (new_record.get_phone (), hash_p);
#ifdef FAST_BUT_RESQUE
		ptr = new List_2_node<Record> ((Record&&) new_record);
		base->inject (ptr);
#else
		ptr = base->insert (new_record);
#endif
		if (ptr == nullptr)
			return ELEM_ALREADY_EXIST;
		tmp_n = new AVLt_tree_node<Database_link_node*>;
		if (tmp_n == nullptr)
			return MEMORY_ERROR;
		tmp_p = new AVLt_tree_node<Database_link_node*>;
		if (tmp_p == nullptr) {
			delete tmp_n;
			return MEMORY_ERROR;
		}
		
		link_node_tmp = new List_2_node<Database_link_node>;
		if (link_node_tmp == nullptr) {
			delete tmp_n; delete tmp_p;
			return MEMORY_ERROR;
		}
		// Filling link_node_tmp.
		link_node_tmp->owner = ptr;
		link_node_tmp->owner_container = this->base;
		link_node_tmp->name_avl_node = tmp_n;
		link_node_tmp->name_avl_container = this->find_name_struct[index_n];
		link_node_tmp->phone_avl_node = tmp_p;
		link_node_tmp->phone_avl_container = this->find_phone_struct[index_p];
		link_node_tmp->myself = link_node_tmp;
		link_node_tmp->myself_container = cross_refs;
		
		tmp_n->set (link_node_tmp);
		tmp_p->set (link_node_tmp);
#ifdef FAST_BUT_RESQUE
		cross_refs->inject (link_node_tmp);
#else
		cross_refs->insert (link_node_tmp);
#endif
		find_name_struct[index_n]->insert (tmp_n);
		find_phone_struct[index_p]->insert (tmp_p);
		return SUCCESS;
	}
	
	void print_links (void) const { // Отладочная.
		for (List_2_node<Database_link_node> * cur = cross_refs->get_head(); cur != nullptr; cur = cur->get_next ()) {
			//if (cur->owner == nullptr)
				//printf ("empty node\n");
			//else
				//cur->owner->print (nullptr, stdout);
			cur->check ();
		}
	}
	void print_trees (void) const { // Отладочная.
		for (int i = 0; i < hash_k; i++)
		{
			printf ("-----------------------\n");
			find_name_struct[i]->print (18, print_avl_data_func);
		}
	}
	static void print_avl_data_func (Database_link_node * el) {
		el->owner->print (nullptr, stdout);
	}
	friend class Database_command;
};

#endif // DATABASE.
