#ifndef COMMAND_H
#define COMMAND_H
#include <stdio.h>
#include "record.h"
#include "operation.h"

class Command : public Record
{
private:
	Condition c_name = Condition::none;
	Condition c_phone = Condition::none;
	Condition c_group = Condition::none;
	Operation op = Operation::none;
	std::unique_ptr<Ordering []> order = nullptr;
	void clean ()
	{
		c_name = Condition::none;
		c_phone = Condition::none;
		c_group = Condition::none;
		op = Operation::none;
		order = nullptr;
		init (nullptr, -1, -1);
	}
	bool service_read_conditions (const char *&);
	bool service_read_ordering (const char *&);
public:
	Command () = default;
	~Command () = default;
	
	Ordering * get_order () { return order.get(); }
	
	bool parse (const char * string);
	void print (FILE *fp = stdout) const;
	bool apply (const Record& x) const;
};
#endif // COMMAND_H.
