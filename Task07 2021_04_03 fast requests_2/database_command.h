#ifndef DATABASE_COMMAND_H
#define DATABASE_COMMAND_H
#include "command.h"
#include "database.h"
#include "list_2_typename.h"

class Database_command : public Command
{
private:
	//List_2_node<Record_ptr> * apply_to_avl_subtree (AVL_tree_node<Record_ptr> * sub_head, int (*cmp) (Record_ptr&, Record_ptr&, void *), void * cmp_arg);
	//List_2_node<Record_ptr> * choosing (AVL_tree_node<Record_ptr> * sub_head, int (*cmp) (Record_ptr&, Record_ptr&, void *), void * cmp_arg);
	List_2t_node<Database_link_node*> * choosing (Condition condition, AVLt_tree_node<Database_link_node*> * sub_head, int (*cmp) (Database_link_node*, Database_link_node*, void *), void * cmp_arg);
	List_2t_node<Database_link_node*> * apply_to_avl_subtree (Condition condition, AVLt_tree_node<Database_link_node*> * sub_head, int (*cmp) (Database_link_node*, Database_link_node*, void *), void * cmp_arg);

	//int removing (AVL_tree<Record_ptr> * tree, AVL_tree_node<Record_ptr> * sub_head, List_2<Record>& list);
	//void rm_short_alias (AVL_tree<Record_ptr> * tree, AVL_tree_node<Record_ptr> * sub_head, List_2<Record>& list);
public:
	Database_command () : Command () {}
	~Database_command () = default;
	
	List_2t<Database_link_node*> * apply (Database& database, int& count, int do_print = 1);
	//void removing_with_avl_subtree (AVL_tree<Record_ptr> * tree, AVL_tree_node<Record_ptr> * sub_head, List_2<Record>& list);
};
#endif // DATABASE_COMMAND_H.
