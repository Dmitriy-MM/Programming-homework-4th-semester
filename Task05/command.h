#ifndef COMMAND_H
#define COMMAND_H
#include <stdio.h>
#include "record.h"
#include "operation.h"
#include "command_type.h"
#include "list_2.h"

class Command : public Record
{
public:
	static const int Max_items = 3;
private:
	Command_type type = Command_type::none;
	Condition c_name = Condition::none;
	Condition c_phone = Condition::none;
	Condition c_group = Condition::none;
	Operation op = Operation::none;
	// std::unique_ptr<Ordering []> order = nullptr;
	Ordering order[Max_items] = { Ordering::none };
	Ordering order_by[Max_items] = { Ordering::none };
	Ordering default_order[Max_items] = { Ordering::name, Ordering::phone, Ordering::group };
	void clean ()
	{
		type = Command_type::none;
		c_name = Condition::none;
		c_phone = Condition::none;
		c_group = Condition::none;
		op = Operation::none;
		for (int i = 0; i < Max_items; i++)
			order[i] = default_order[i],
			order_by[i] = Ordering::none;
		init (nullptr, -1, -1);
	}
	bool service_read_conditions (const char *&);
	bool service_read_ordering (const char *&);
	bool service_read_sort_ordering (const char *& str);
	bool service_read_command_type (const char *& str);
	bool service_clear_conditions ()
	{
		c_name = Condition::none;
		c_phone = Condition::none;
		c_group = Condition::none;
		op = Operation::none;
		return false;
	}
public:
	Command () = default;
	~Command () = default;
	
	Ordering * get_order () { return order; }
	Command_type get_type () { return type; }
	
	bool parse (const char * string);
	void print (FILE *fp = stdout) const;
	bool apply (const Record& x) const;
	List_2<Record_ptr> * apply (List_2<Record>& list, int &count);
	//List_2<Record_ptr> * apply (List_2<Record_ptr> *& list);
};
#endif // COMMAND_H.
