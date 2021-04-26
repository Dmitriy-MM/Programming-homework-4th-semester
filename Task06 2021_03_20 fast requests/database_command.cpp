#include <stdio.h>
#include "database.h"
#include "database_command.h"
 
 
static int comparator (Database_link_node* p1, Database_link_node* p2, void * order_by)
{
	int i, condition = 0;
	Record& r1 = *p1->owner;
	Record& r2 = *p2->owner;
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

List_2t<Database_link_node*> * Database_command::apply (Database& database, int& count, int do_print)
{
	//int (*avl_cmp) (Record_ptr&, Record_ptr&, void *) = Database::func_avl_cmp;
	int (*cmp) (Database_link_node*, Database_link_node*, void *) = comparator;
	//void * cmp_arg = database.arg_func_avl_cmp;
	List_2t<Database_link_node*> * selected_list = nullptr;
	List_2<Database_link_node> * list = database.cross_refs;
	int i;
	//(void) avl_cmp; (void) cmp_arg;
	(void) comparator;
	switch (type)
	{
		case Command_type::none:
			break;
		case Command_type::select:
			{
				int k = database.get_hash_k ();
				AVLt_tree<Database_link_node*> ** avl_trees = database.get_find_name_struct ();
				selected_list = new List_2t<Database_link_node*>;
				if (
					(op == Operation::lor) || 
					(c_name == Condition::none) || 
					(c_name == Condition::like) ||
					(c_name == Condition::nlike) || 
					(c_name == Condition::ne) 
				)
				{
					for (List_2_node<Database_link_node> * cur = list->get_head(); cur != nullptr; cur = cur->get_next ()) {
						if (((Command *)this)->apply (*cur->owner)) {
							selected_list->insert ((Database_link_node *) cur);
						}
					}
				}
				else {
					if (c_name == Condition::eq)
					{
						int index = Database::func_hash_name (this->get_name(), k);
						selected_list->join (apply_to_avl_subtree (avl_trees[index]->get_root (), avl_trees[index]->get_compare_func (), avl_trees[index]->get_compare_arg ()));
					}
					else
					{
						for (i = 0; i < k; i++)
						{
							// Поиск по i-му дереву.
							selected_list->join (apply_to_avl_subtree (avl_trees[i]->get_root (), avl_trees[i]->get_compare_func (), avl_trees[i]->get_compare_arg ()));
						}
					}
				}
				count += selected_list->get_length ();
				if (do_print)
				{
					for (i = 0; i < Max_items; i++)
					{
						if (order_by[i] != Ordering::none)
							break;
					}
					if (i < Max_items) 
						selected_list->sort (cmp, (void *) order_by);
					for (List_2t_node<Database_link_node*> * cur = selected_list->get_head(); cur != nullptr; cur = cur->get_next())
						cur->get()->owner->print (this->get_order ());
				}
				return selected_list;
			}
			break;
		case Command_type::insert:
			{
				int count_eq = 0;
				c_name = Condition::eq;
				c_phone = Condition::eq;
				c_group = Condition::eq;
				op = Operation::land;
				type = Command_type::select;
				delete this->apply (database, count_eq, 0);
				type = Command_type::insert;
				if (count_eq == 0)
					database.insert (*this);
			} break;
		case Command_type::remove:
			{
				int count_deleted = 0;
				this->type = Command_type::select;
				selected_list = this->apply (database, count_deleted, 0);
				this->type = Command_type::remove;
				for (List_2t_node<Database_link_node*> * cur = selected_list->get_head (); cur != nullptr; cur = cur->get_next ()) {
					cur->get()->remove();
				}
				delete selected_list;
			} break;
		case Command_type::quit:
			break;
	}
	return nullptr;
}


List_2t_node<Database_link_node*> * Database_command::choosing (AVLt_tree_node<Database_link_node*> * sub_head, int (*cmp) (Database_link_node*, Database_link_node*, void *), void * cmp_arg)
{
	List_2t<Database_link_node*> tmp_list;
	List_2t_node<Database_link_node*> * tmp = nullptr, *tmp2 = nullptr;
	for (AVLt_tree_node<Database_link_node*> * cur = sub_head; cur != nullptr; cur = cur->get_next ())
	{
		if (((Command *)this)->apply (*cur->get()->owner))
		{
			tmp2 = tmp;
			tmp = new List_2t_node<Database_link_node*>;
			if (tmp == nullptr)
			{
				printf ("Malloc error (apply_to_avl_subtree()\n");
				return nullptr;
			}
			tmp->set (cur->get());
			tmp->set_next (tmp2);
		}
	}
	tmp_list.join (tmp);
	tmp_list.join (apply_to_avl_subtree (sub_head->get_left (), cmp, cmp_arg));
	tmp_list.join (apply_to_avl_subtree (sub_head->get_right (), cmp, cmp_arg));
	//tmp_list.restore_prev_links (); // Можно не вызывать, но тогда возвращенный список не будет двусвязным.
	tmp = tmp_list.get_head ();
	tmp_list.set_head (nullptr);
	return tmp;	
}

List_2t_node<Database_link_node*> * Database_command::apply_to_avl_subtree (AVLt_tree_node<Database_link_node*> * sub_head, int (*cmp) (Database_link_node*, Database_link_node*, void *), void * cmp_arg)
{
	int cmp_res;
	Database_link_node this_fake_node;
	
	if (sub_head == nullptr)
		return nullptr;
	
	//this_fake_node.owner = ((List_2_node<Record> *) ((Record *) this));
	this_fake_node.owner = (List_2_node<Record>*) this;
	cmp_res = cmp (&this_fake_node, sub_head->get(), cmp_arg);
	
	switch (c_name)
	{
		case Condition::eq:
			if (cmp_res < 0) { // this_ptr < *sub_head.
				return apply_to_avl_subtree (sub_head->get_left (), cmp, cmp_arg);
			} else if (cmp_res > 0) {
				return apply_to_avl_subtree (sub_head->get_right (), cmp, cmp_arg);
			} else {
				List_2t_node<Database_link_node*> * tmp = nullptr, * tmp2 = nullptr;
				for (AVLt_tree_node<Database_link_node*> * cur = sub_head; cur != nullptr; cur = cur->get_next ())
				{
					if (((Command *)this)->apply (*cur->get()->owner))
					{
						tmp2 = tmp;
						tmp = new List_2t_node<Database_link_node*>;
						if (tmp == nullptr)
						{
							printf ("Malloc error (apply_to_avl_subtree()\n");
							return nullptr;
						}
						tmp->set (cur->get());
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
		case Condition::ne:
		case Condition::like:
		case Condition::nlike:
		case Condition::none:
			break;
	}
	return nullptr;
}
