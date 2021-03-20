#include <stdio.h>
#include <cstring>
#include "condition.h"
#include "command.h"

#include "list_2.h"

static const int LEN = 1234;

static void skip_spaces (const char *& str)
{
	str += strspn (str, " \t\n");
}

#define Command_CHANGES 1
#if Command_CHANGES
// static int comparator (Record& r1, Record& r2, void * order_by)
// {
	// int i, condition = 0;
	// Ordering * order = (Ordering *) order_by;
	
	// for (i = 0; i < Command::Max_items; i++)
	// {
		// switch (order[i])
		// {
			// case Ordering::none:
				// break;
			// case Ordering::name:
				// if (!(r1.get_name() && r2.get_name()))
					// return 0;
				// condition = strcmp (r1.get_name(), r2.get_name());
				// if (condition)
					// return condition;
				// break;
			// case Ordering::group:
				// condition = r1.get_group () - r2.get_group ();
				// if (condition)
					// return condition;
				// break;
			// case Ordering::phone:
				// condition = r1.get_phone () - r2.get_phone ();
				// if (condition)
					// return condition;
				// break;
		// }
	// }
	// return 0;
// }
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
#endif

static int hightlight_words (const char * str, char * field_name, char * operator_name, char * expression_name)
{
	const int LEN = 50;
	int count_read = 0;
	int i, length;
	const char spec_words[][LEN] = {
		"not like",
		"like",
		"<>",
		">=",
		"<=",
		">",
		"<",
		"=",
		""
	};
	
	skip_spaces (str);
	
	length = strcspn (str, " \t\n=<>");
	strncpy (field_name, str, length);
	field_name[length] = '\0';
	count_read++;
	str += length;
	skip_spaces (str);
	for (i = 0; spec_words[i][0]; i++)
	{
		if (str == strstr (str, spec_words[i]))
		{
			strcpy (operator_name, spec_words[i]);
			count_read++;
			break;
		}
	}
	if (!spec_words[i][0])
		return count_read;
	str += strlen (operator_name);
	
	skip_spaces (str);
	
	length = strcspn (str, " \t\n");
	strncpy (expression_name, str, length);
	expression_name[length] = '\0';
	count_read++;
	
	return count_read;
}

static Condition get_condition (const char * str)
{
	Condition c = Condition::none;
	if (!strcmp (str, "="))
		c = Condition::eq;
	else if (!strcmp (str, "<>")) 
		c = Condition::ne;
	else if (!strcmp (str, "<"))
		c = Condition::lt;
	else if (!strcmp (str, ">"))
		c = Condition::gt;
	else if (!strcmp (str, "<="))
		c = Condition::le;
	else if (!strcmp (str, ">="))
		c = Condition::ge;
	else if (!strcmp (str, "like"))
		c = Condition::like;
	else if (!strcmp (str, "not like"))
		c = Condition::nlike;
	return c;
}


bool Command::parse (const char * str)
{
	const char * str_pos = str;


#if Command_CHANGES
	// Очистка.
	clean ();
	if (!service_read_command_type (str_pos))
		return false;
	switch (type)
	{
		case Command_type::none:
			return false;
		case Command_type::select:
			// Считать условия на выводимые поля.
			if (!service_read_ordering (str_pos))
				return false;
			{	// Считать where.
				const char * where_pos = strstr (str_pos, "where");
				const char * prev_pos;
				if (where_pos != nullptr)
					str_pos = where_pos + strlen ("where");
				// Считать условия поиска.
				prev_pos = str_pos;
				if (!service_read_conditions (str_pos))
					str_pos = prev_pos;
			}
			{	// Считать условия сортировки.
				const char * order_by_pos = strstr (str_pos, "order by");
				const char * prev_pos;
				//if (order_by_pos)
					//service_read_sort_ordering (order_by_pos);
				prev_pos = str_pos;
				str_pos = order_by_pos != nullptr? order_by_pos + strlen ("order by"): str_pos;
				if (!service_read_sort_ordering (str_pos))
				{
					for (int i = 0; i < Max_items; i++)
						order_by[i] = Ordering::none;
					str_pos = prev_pos;
				}
			}
			return true;
		case Command_type::insert:
			str = strstr (str, "(");
			if (str == nullptr)
				return false;
			str += strlen ("(");
			{
				int length, phone, group;
				const int LEN = 1234;
				char buf[LEN] = {'\0'}, buf2[LEN] = {'\0'};
				skip_spaces (str);
				length = strcspn (str, " ,\t\n");
				strncpy (buf, str, length);
				buf[length] = '\0';
				str += length;
				str += strspn (str, " ,\t\n");
				
				length = strcspn (str, " ,\t\n");
				strncpy (buf2, str, length);
				buf2[length] = '\0';
				if (sscanf (buf2, "%d", &phone) != 1)
					return false;
				str += length;
				str += strspn (str, " ,\t\n");
				length = strcspn (str, " ,\t\n");
				strncpy (buf2, str, length);
				buf2[length] = '\0';
				if (sscanf (buf2, "%d", &group) != 1)
					return false;
				str = strstr (str, ")");
				if (str == nullptr)
					return false;
				
				init (buf, phone, group);
				return true;
			}
		case Command_type::remove:
			{	// Считать where.
				const char * where_pos = strstr (str_pos, "where");
				const char * prev_pos;
				if (where_pos != nullptr)
					str_pos = where_pos + strlen ("where");
				// Считать условия поиска.
				prev_pos = str_pos;
				if (!service_read_conditions (str_pos))
					str_pos = prev_pos;
			}
			return true;
		case Command_type::quit:
			return true;
	}
	return true;
#else
	// Очистка.
	clean ();
	// Считать условия на выводимые поля.
	if (!service_read_ordering (str_pos))
		return false;
	// Считать where.
	str_pos = strstr (str_pos, "where");
	if (!str_pos)
		return false;
	else
		str_pos += strlen ("where");
	// Считать условия поиска.
	if (!service_read_conditions (str_pos))
		return false;
	return true;
#endif
}

bool Command::service_read_ordering (const char *& str)
{
	const int Ordering_length = Max_items;
	const char field_name[][LEN] = {
		 "phone",
		 "name",
		 "group"
	};
	Ordering associated_field_name_arr[] = {
		Ordering::phone,
		Ordering::name,
		Ordering::group
	};
	
	skip_spaces (str);
	if (*str == '*')
	{
		for (int i = 0; i < Max_items; i++)
			order[i] = default_order[i];
		return true;
	}
	else
	{
		for (int i = 0; i < Max_items; i++)
			order[i] = Ordering::none;
	}
	for (int i = 0; i < Ordering_length; i++)
		for (int k = 0; k < Ordering_length; k++)
			if (str == strstr (str, field_name[k]))
			{
				order[i] = associated_field_name_arr[k];
				str += strlen (field_name[k]);
				str += strspn (str, ", \t\n");
				break;
			}
	return true;
}

bool Command::service_read_sort_ordering (const char *& str)
{
	const int Ordering_length = Max_items;
	const char field_name[][LEN] = {
		 "phone",
		 "name",
		 "group"
	};
	Ordering associated_field_name_arr[] = {
		Ordering::phone,
		Ordering::name,
		Ordering::group
	};
	
	skip_spaces (str);
	for (int i = 0; i < Ordering_length; i++)
		for (int k = 0; k < Ordering_length; k++)
			if (str == strstr (str, field_name[k]))
			{
				order_by[i] = associated_field_name_arr[k];
				str += strlen (field_name[k]);
				str += strspn (str, ", \t\n");
				break;
			}
			else
				order_by[i] = Ordering::none;
	return true;
}

bool Command::service_read_conditions (const char *& str_pos)
{
	//const char * str_pos = str;
	char * name = nullptr;
	int phone = -1, group = -1;
	char field_name[LEN], operator_name[LEN], expression_name[LEN], operation_name[LEN];
	
	if (hightlight_words (str_pos, field_name, operator_name, expression_name) != 3)
		return service_clear_conditions();
	skip_spaces (str_pos);
	str_pos += strlen (field_name);
	skip_spaces (str_pos);
	str_pos += strlen (operator_name);
	skip_spaces (str_pos);
	str_pos += strlen (expression_name);
	skip_spaces (str_pos);
	if (sscanf (str_pos, "%s", operation_name) == 1)
	{
		int is_read = 0;
		if (!strcmp (operation_name, "and"))
		{
			this->op = Operation::land;
			is_read = 1;
		}
		else if (!strcmp (operation_name, "or"))
		{
			this->op = Operation::lor;
			is_read = 1;
		}
		if (is_read)
		{
			str_pos += strlen (operation_name);
			skip_spaces (str_pos);
			if (!service_read_conditions (str_pos))
				return service_clear_conditions();
		}
	}
	if (!strcmp (field_name, "phone"))
	{
		this->c_phone = get_condition (operator_name);
		if (sscanf (expression_name, "%d", &phone) != 1)
			return service_clear_conditions();
		set_phone (phone);
	} 
	else if (!strcmp (field_name, "group"))
	{
		this->c_group = get_condition (operator_name);
		if (sscanf (expression_name, "%d", &group) != 1)
			return service_clear_conditions();
		set_group (group);
	}
	else if (!strcmp (field_name, "name"))
	{
		this->c_name = get_condition (operator_name);
		name = expression_name;
		set_name (name);
	}
	
	return true;
}

bool Command::service_read_command_type (const char *& str)
{
	const int LEN = 50;
	const char spec_words[][LEN] = {
		"select",
		"insert",
		"delete",
		"quit",
		""
	};
	Command_type associated_command_type_arr [] = {
		Command_type::select,
		Command_type::insert,
		Command_type::remove,
		Command_type::quit,
		Command_type::none,
	};

	skip_spaces (str);
	for (int i = 0; spec_words[i]; i++)
	{
		if (str == strstr (str, spec_words[i]))
		{
			type = associated_command_type_arr[i];
			str += strlen (spec_words[i]);
			return true;
		}
	}
	type = Command_type::none;
	return false;
}

// Print parsed structure.
void Command::print (FILE *fp) const 
{
	this->Record::print ();
	if (c_name != Condition::none)
		fprintf (fp, "c_name ");
	if (c_phone != Condition::none)
		fprintf (fp, "c_phone ");
	if (c_group != Condition::none)
		fprintf (fp, "c_group ");
	fprintf (fp, "\n");
}

// Apply Command, return comparision result for Record ’x’.
bool Command::apply (const Record& x) const
{
	if (op == Operation::lor)
	{
		if ( ((c_phone != Condition::none) && compare_phone (c_phone, x)) || 
			 ((c_group != Condition::none) && compare_group (c_group, x)) ||
			 ((c_name != Condition::none) && compare_name (c_name, x))
		)
			return true;
	}
	else
	{
		if (compare_phone (c_phone, x) && compare_group (c_group, x) && compare_name (c_name, x))
			return true;
	}
	return false;
}

#if Command_CHANGES
// List_2<Record>& Command::apply (List_2<Record>& list)
List_2<Record_ptr> * Command::apply (List_2<Record>& list, int& count)
{
	int (*cmp) (Record_ptr&, Record_ptr&, void *) = comparator; 
	List_2<Record_ptr> * selected_list = nullptr;
	int i;
	//(void) cmp;
	switch (type)
	{
		case Command_type::none:
			break;
		case Command_type::select:
			selected_list = new List_2<Record_ptr>;
			for (List_2_node<Record> * cur = list.get_head(); cur != nullptr; cur = cur->get_next())
				if (this->apply (*cur))
				{
					List_2_node<Record_ptr> * new_node = new List_2_node<Record_ptr>;
					new_node->ptr = cur;
					// cur->print (this->get_order (), fout);
					//selected_list->insert (* new Record_ptr (cur));
					selected_list->insert (new_node);
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
			break;
		case Command_type::insert:
			list.insert (*this);
			break;
		case Command_type::remove:
			for (List_2_node<Record> * cur = list.get_head(); cur != nullptr; )
				if (this->apply (*cur))
				{
					List_2_node<Record> * next = cur->get_next();
					list.remove (cur);
					cur = next;
					
					//count++; // Для совпадения тестов.
				}
				else
					cur = cur->get_next();
			break;
		case Command_type::quit:
			break;
	}
	return nullptr;
}

//List_2<Record_ptr> * Command::apply (List_2<Record_ptr> *& list)
//{
	//List_2<Record_ptr> * selected_list = nullptr;
	//switch (type)
	//{
		//case Command_type::none:
			//return list;
		//case Command_type::select:
			//selected_list = new List_2<Record_ptr>;
			//for (List_2_node<Record_ptr> * cur = list->get_head(); cur != nullptr; cur = cur->get_next())
				//if (this->apply (*(cur->ptr)))
				//{
					//// cur->print (this->get_order (), fout);
					//list->cut (cur);
					//selected_list->insert (cur);
				//}
					
			//selected_list->sort (comparator, (void *) order_by);
			//delete list;
			//list = selected_list;
			//return selected_list;
		//case Command_type::insert: // FORBIDDEN!
			//printf ("Forbidden call (insert)\n");
			//return nullptr;
		//case Command_type::remove:
			//printf ("Forbidden call (delete)\n");
			//return nullptr;
		//case Command_type::quit:
			//return nullptr;
	//}
	//return nullptr;
//}
#endif
