#include <stdio.h>
#include "database.h"
#include "database_command.h"
 
 
static int comparator (Record_ptr& p1, Record_ptr& p2, void * order_by)
{
	int i, condition = 0;
	Record& r1 = *p1;
	Record& r2 = *p2;
	Ordering * order = (Ordering *) order_by;
	
	for (i = 0; i < Command::Max_items; i++)
	{
		switch (order[i])
		{
			case Ordering::none:
				break;
			case Ordering::name:
				if (!(r1.get_name() && r2.get_name()))
					return 0;
				condition = strcmp (r1.get_name(), r2.get_name());
				if (condition)
					return condition;
				break;
			case Ordering::group:
				condition = r1.get_group () - r2.get_group ();
				if (condition)
					return condition;
				break;
			case Ordering::phone:
				condition = r1.get_phone () - r2.get_phone ();
				if (condition)
					return condition;
				break;
		}
	}
	return 0;
}


List_2_node<Record_ptr> * Database_command::choosing (AVL_tree_node<Record_ptr> * sub_head, int (*cmp) (Record_ptr&, Record_ptr&, void *), void * cmp_arg)
{
	List_2<Record_ptr> tmp_list;
	List_2_node<Record_ptr> * tmp = nullptr, *tmp2 = nullptr;
	for (AVL_tree_node<Record_ptr> * cur = sub_head; cur != nullptr; cur = cur->get_next ())
	{
		if (((Command *)this)->apply (*cur->ptr))
		{
			tmp2 = tmp;
			tmp = new List_2_node<Record_ptr>;
			if (tmp == nullptr)
			{
				printf ("Malloc error (apply_to_avl_subtree()\n");
				return nullptr;
			}
			tmp->ptr = cur->ptr;
			tmp->set_next (tmp2);
		}
	}
	tmp_list.join (tmp);
	tmp_list.join (apply_to_avl_subtree (sub_head->get_left (), cmp, cmp_arg));
	tmp_list.join (apply_to_avl_subtree (sub_head->get_right (), cmp, cmp_arg));
	tmp_list.restore_prev_links (); // Можно не вызывать, но тогда возвращенный список не будет двусвязным.
	tmp = tmp_list.get_head ();
	tmp_list.set_head (nullptr);
	return tmp;	
}



List_2_node<Record_ptr> * Database_command::apply_to_avl_subtree (AVL_tree_node<Record_ptr> * sub_head, int (*cmp) (Record_ptr&, Record_ptr&, void *), void * cmp_arg)
{
	int cmp_res;
	Record_ptr this_ptr;
	
	if (sub_head == nullptr)
		return nullptr;
	this_ptr.ptr = this;
	cmp_res = cmp (this_ptr, *sub_head, cmp_arg);
	if (op == Operation::lor)
		return choosing (sub_head, cmp, cmp_arg);
	switch (c_name)
	{
		case Condition::ne:
		case Condition::like:
		case Condition::nlike:
		case Condition::none:
			return choosing (sub_head, cmp, cmp_arg);
		case Condition::eq:
			if (cmp_res < 0) { // this_ptr < *sub_head.
				return apply_to_avl_subtree (sub_head->get_left (), cmp, cmp_arg);
			} else if (cmp_res > 0) {
				return apply_to_avl_subtree (sub_head->get_right (), cmp, cmp_arg);
			} else {
				List_2_node<Record_ptr> * tmp = nullptr, * tmp2 = nullptr;
				for (AVL_tree_node<Record_ptr> * cur = sub_head; cur != nullptr; cur = cur->get_next ())
				{
					if (((Command *)this)->apply (*cur->ptr))
					{
						tmp2 = tmp;
						tmp = new List_2_node<Record_ptr>;
						if (tmp == nullptr)
						{
							printf ("Malloc error (apply_to_avl_subtree()\n");
							return nullptr;
						}
						tmp->ptr = cur->ptr;
						tmp->set_next (tmp2);
					}
				}
				return tmp;
			}
		case Condition::gt:
			if (cmp_res < 0) { // this_ptr < *sub_head.
				return choosing (sub_head, cmp, cmp_arg);
			} else {
				return apply_to_avl_subtree (sub_head->get_right (), cmp, cmp_arg);				
			}
		case Condition::ge:
			if (cmp_res <= 0) { // this_ptr <= *sub_head.
				return choosing (sub_head, cmp, cmp_arg);
			} else {
				return apply_to_avl_subtree (sub_head->get_right (), cmp, cmp_arg);				
			}
		case Condition::lt:
			if (cmp_res <= 0) { // this_ptr <= *sub_head.
				return apply_to_avl_subtree (sub_head->get_left (), cmp, cmp_arg);	
			} else {
				return choosing (sub_head, cmp, cmp_arg);		
			}
		case Condition::le:
			if (cmp_res < 0) { // this_ptr < *sub_head.
				return apply_to_avl_subtree (sub_head->get_left (), cmp, cmp_arg);	
			} else {
				return choosing (sub_head, cmp, cmp_arg);
			}
	}
	return nullptr;
}

void Database_command::rm_short_alias (AVL_tree<Record_ptr> * tree, AVL_tree_node<Record_ptr> * sub_head, List_2<Record>& list) 
{
	if (!removing (tree, sub_head, list))
	{
		removing_with_avl_subtree (tree, sub_head->get_left (), list);
		removing_with_avl_subtree (tree, sub_head->get_right (), list);
	}
	else
	{
		removing_with_avl_subtree (tree, tree->get_root (), list);
		removing_with_avl_subtree (tree, tree->get_root (), list);
	}
}

int Database_command::removing (AVL_tree<Record_ptr> * tree, AVL_tree_node<Record_ptr> * sub_head, List_2<Record>& list)
{
	int res = 0;
	for (AVL_tree_node<Record_ptr> * cur = sub_head, *next = nullptr; cur != nullptr; cur = next)
	{
		next = cur->get_next ();
		if (((Command *)this)->apply (*cur->ptr))
		{
			res++;
			List_2_node<Record> * list_node_ptr = nullptr;
			list_node_ptr = (List_2_node<Record> *) cur->ptr;
			tree->remove (cur);
			list.remove (list_node_ptr);
		}
	}
	return res;
}


void Database_command::removing_with_avl_subtree (AVL_tree<Record_ptr> * tree, AVL_tree_node<Record_ptr> * sub_head, List_2<Record>& list)
{
	int cmp_res;
	Record_ptr this_ptr;
	int (*cmp) (Record_ptr&, Record_ptr&, void *) = tree->get_compare_func ();
	void * cmp_arg = tree->get_compare_arg ();
	
	if (sub_head == nullptr)
		return ;
	this_ptr.ptr = this;
	cmp_res = cmp (this_ptr, *sub_head, cmp_arg);
	if (op == Operation::lor)
	{
		rm_short_alias (tree, sub_head, list);
		return;
	}
	switch (c_name)
	{
		case Condition::ne:
		case Condition::like:
		case Condition::nlike:
		case Condition::none:
			rm_short_alias (tree, sub_head, list);
			break;

		case Condition::eq:
			if (cmp_res < 0) { // this_ptr < *sub_head.
				removing_with_avl_subtree (tree, sub_head->get_left (), list);//(sub_head->get_left (), cmp, cmp_arg);
			} else if (cmp_res > 0) {
				removing_with_avl_subtree (tree, sub_head->get_right (), list);//(sub_head->get_right (), cmp, cmp_arg);
			} else {
				removing (tree, sub_head, list);
			}
			break;
		case Condition::gt:
			if (cmp_res < 0) { // this_ptr < *sub_head.
				//removing_with_avl_subtree (tree, sub_head->get_left (), list);
				//removing_with_avl_subtree (tree, sub_head->get_right (), list);
				//removing (tree, sub_head, list);
				rm_short_alias (tree, sub_head, list);
			} else {
				removing_with_avl_subtree (tree, sub_head->get_right (), list);
			}
			break;
		case Condition::ge:
			if (cmp_res <= 0) { // this_ptr <= *sub_head.
				rm_short_alias (tree, sub_head, list);
			} else {
				removing_with_avl_subtree (tree, sub_head->get_right (), list);
			}
			break;
		case Condition::lt:
			if (cmp_res <= 0) { // this_ptr <= *sub_head.
				removing_with_avl_subtree (tree, sub_head->get_left (), list);
			} else {
				rm_short_alias (tree, sub_head, list);
			}
			break;
		case Condition::le:
			if (cmp_res < 0) { // this_ptr < *sub_head.
				removing_with_avl_subtree (tree, sub_head->get_left (), list);
			} else {
				rm_short_alias (tree, sub_head, list);
			}
			break;
	}
	return;
}


List_2<Record_ptr> * Database_command::apply (Database& database, int& count)
{
	int (*avl_cmp) (Record_ptr&, Record_ptr&, void *) = Database::func_avl_cmp;
	int (*cmp) (Record_ptr&, Record_ptr&, void *) = comparator;
	void * cmp_arg = database.arg_func_avl_cmp;
	List_2<Record_ptr> * selected_list = nullptr;
	List_2<Record>& list = *database.base;
	int i;
	(void) avl_cmp; (void) cmp_arg;
	switch (type)
	{
		case Command_type::none:
			break;
		case Command_type::select:
			{
				int k = database.get_hash_k ();
				
				AVL_tree<Record_ptr> ** avl_trees = database.get_find_name_struct ();
				selected_list = new List_2<Record_ptr>;
				// Цикл по массиву деревьев.
				for (i = 0; i < k; i++)
				{
					// Поиск по i-му дереву.
					selected_list->join (apply_to_avl_subtree (avl_trees[i]->get_root (), avl_trees[i]->get_compare_func (), avl_trees[i]->get_compare_arg ()));
				}
				for (i = 0; i < Max_items; i++)
				{
					if (order_by[i] != Ordering::none)
						break;
				}
				if (i < Max_items) 
					selected_list->sort (cmp, (void *) order_by);
				for (List_2_node<Record_ptr> * cur = selected_list->get_head(); cur != nullptr; cur = cur->get_next())
					cur->ptr->print (this->get_order ()), count++;
				delete selected_list;
				return nullptr;
			}
			break;
		case Command_type::insert:
			database.insert (*this);
			break;
		case Command_type::remove:
			{
				int k = database.get_hash_k ();
				AVL_tree<Record_ptr> ** avl_trees = database.get_find_name_struct ();
				// Цикл по массиву деревьев.
				for (i = 0; i < k; i++)
				{
					// Поиск по i-му дереву.
					removing_with_avl_subtree(avl_trees[i], avl_trees[i]->get_root (), list);
				}
				return nullptr;
			}
		case Command_type::quit:
			break;
	}
	return nullptr;
}
