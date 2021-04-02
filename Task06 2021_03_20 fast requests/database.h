#ifndef DATABASE
#define DATABASE
#include <stdio.h>
#include "avl.h"
#include "list_2.h"
#include "record.h"

#define CHANGES 1

class Database {
private:
	// Properties.
	List_2<Record> * base = nullptr;
	AVL_tree<Record_ptr> ** find_name_struct = nullptr;
	int hash_k = 0;
	
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
	int clear_all_ptrs (int error_code = 0) {
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
		
		return error_code;
	}
public:
	// Properties.
	void * arg_func_avl_cmp = nullptr;
	// Constructors.
	Database (int k): Database () {
		this->hash_k = k;
	}
	~Database () {
		clear_all_ptrs ();
	}
	
	// Methods.
	static int func_avl_cmp (Record_ptr& p1, Record_ptr& p2, void * cmp_arg = nullptr) {
		(void) cmp_arg;
		if (!(p1.ptr != nullptr && p2.ptr != nullptr))
			return 0;
		if (!(p1.ptr->get_name () != nullptr && p2.ptr->get_name () != nullptr))
			return 0;
		return strcmp (p1.ptr->get_name (), p2.ptr->get_name ());
	}
	
	int read (FILE *fp) {
		AVL_tree_node<Record_ptr> * tmp = nullptr;
		int ret_code, index;
		enum {
			SUCCESS = 0,
			MEMORY_ERROR,
		};
		
		this->base = new List_2<Record>;
		if (this->base == nullptr)
			return clear_all_ptrs (MEMORY_ERROR);
		ret_code = base->read (fp);
		if (ret_code)
			return clear_all_ptrs (ret_code);
		
		this->find_name_struct = new AVL_tree<Record_ptr>* [hash_k];
		if (find_name_struct == nullptr)
			return clear_all_ptrs (MEMORY_ERROR);
		for (int i = 0; i < hash_k; i++)
			this->find_name_struct[i] = nullptr;
		for (int i = 0; i < hash_k; i++)
		{
			this->find_name_struct[i] = new AVL_tree<Record_ptr> (func_avl_cmp, arg_func_avl_cmp);
			if (this->find_name_struct[i] == nullptr)
				return clear_all_ptrs (MEMORY_ERROR);
		}
		for (List_2_node<Record> * cur = base->get_head (); cur != nullptr; cur = cur->get_next ())
		{
			index = func_hash_name (cur->get_name (), hash_k);
			tmp = new AVL_tree_node<Record_ptr>;
			if (tmp == nullptr)
				return clear_all_ptrs (MEMORY_ERROR);
			tmp->ptr = (Record *) cur;
			this->find_name_struct[index]->insert (tmp);
			tmp = nullptr;
		}
		return ret_code;
	}

	List_2<Record> * get_base (void) const {
		return base;
	}
	AVL_tree<Record_ptr> ** get_find_name_struct (void) const {
		return find_name_struct;
	}
	int get_hash_k (void) const {
		return hash_k;
	}
	int insert (Record& new_record) {
		AVL_tree_node<Record_ptr> * tmp;
		List_2_node<Record> * ptr;
		enum { SUCCESS = 0, MEMORY_ERROR, ELEM_ALREADY_EXIST };
		int index = func_hash_name (new_record.get_name (), hash_k);
		ptr = base->insert (new_record);
		if (ptr == nullptr)
			return ELEM_ALREADY_EXIST;
		tmp = new AVL_tree_node<Record_ptr>;
		if (tmp == nullptr)
			return MEMORY_ERROR;
		tmp->ptr = ptr;
		find_name_struct[index]->insert (tmp);
		return SUCCESS;
	}
	void print_trees (void) const {
		for (int i = 0; i < hash_k; i++)
		{
			printf ("-----------------------\n");
			find_name_struct[i]->print (6);
		}
	}
	friend class Database_command;
};

#endif // DATABASE.
