#ifndef COMMAND_H
#define COMMAND_H
#include <stdio.h>
#include "record.h"

class Command : public Record
{
private:
	Condition c_name = Condition::none;
	Condition c_phone = Condition::none;
	Condition c_group = Condition::none;
	void clean ()
	{
		c_name = Condition::none;
		c_phone = Condition::none;
		c_group = Condition::none;
		init (nullptr, -1, -1);
	}
public:
	Command () = default;
	~Command () = default;
	bool parse (const char * string);
	void print (FILE *fp = stdout) const;
	bool apply (const Record& x) const;
};
#endif // COMMAND_H.
