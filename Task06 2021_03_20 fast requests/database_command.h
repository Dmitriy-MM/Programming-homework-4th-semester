#ifndef DATABASE_COMMAND_H
#define DATABASE_COMMAND_H
#include "command.h"

class Database_command : public Command
{
private:
	List_2_node<Record_ptr> * apply_to_avl_subtree (AVL_tree_node<Record_ptr> * sub_head, int (*cmp) (Record_ptr&, Record_ptr&, void *), void * cmp_arg);
	List_2_node<Record_ptr> * choosing (AVL_tree_node<Record_ptr> * sub_head, int (*cmp) (Record_ptr&, Record_ptr&, void *), void * cmp_arg);
	int removing (AVL_tree<Record_ptr> * tree, AVL_tree_node<Record_ptr> * sub_head, List_2<Record>& list);
	void rm_short_alias (AVL_tree<Record_ptr> * tree, AVL_tree_node<Record_ptr> * sub_head, List_2<Record>& list);
public:
	Database_command () : Command () {}
	~Database_command () = default;
	
	List_2<Record_ptr> * apply (Database& database, int& count);
	void removing_with_avl_subtree (AVL_tree<Record_ptr> * tree, AVL_tree_node<Record_ptr> * sub_head, List_2<Record>& list);
};
#endif // DATABASE_COMMAND_H.
