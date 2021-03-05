# ifndef COMMAND_H
# define COMMAND_H
# include <stdio.h>
# include "record.h"
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
	// Convert string Command to data structure.
	// Example 1: "phone = 1234567" parsed to
	// (Command::phone = 1234567, Command::c_phone = Condition::eq)
	// other fields are unspecified.
	// Example 2: "name like St%" parsed to
	// (Command::name = "St%", Command::c_name = Condition::like)
	// other fields are unspecified.
	bool parse (const char * string);
	// Print parsed structure.
	void print (FILE *fp = stdout) const;
	// Apply Command, return comparision result for Record ’x’.
	bool apply (const Record& x) const;
	
};
# endif // COMMAND_H.
