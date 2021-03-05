#include <stdio.h>
#include <cstring>
#include "condition.h"
#include "command.h"

static const int LEN = 1234;

static void to_lower_case (char * str, int len)
{
	for (int i = 0; (i < len) && str[i]; i++)
	{
		if (str[i] <= 'Z' && str[i] >= 'A')
			str[i] += 'a' - 'A';
	}
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
	return c;
}

bool Command::parse (const char * str)
{
	char * name = nullptr;
	int phone = -1, group = -1;
	char field_name[LEN], operator_name[LEN], expression_name[LEN];
	
	clean();
	if (sscanf (str, "%s%s%s", field_name, operator_name, expression_name) != 3)
		return false;
	to_lower_case (field_name, LEN);
	to_lower_case (operator_name, LEN);
	to_lower_case (expression_name, LEN);
	if (!strcmp (field_name, "phone"))
	{
		this->c_phone = get_condition (operator_name);
		if (sscanf (expression_name, "%d", &phone) != 1)
		{
			return false;
		}
	} 
	else if ((!strcmp (field_name, "group")) || (!strcmp (field_name, "value")))
	{
		this->c_group = get_condition (operator_name);
		if (sscanf (expression_name, "%d", &group) != 1)
		{
			return false;
		}
	}
	else if (!strcmp (field_name, "name"))
	{
		this->c_name = get_condition (operator_name);
		name = expression_name;
	}
	
	if (init (name, phone, group))
		return false;
	return true;
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
	if (compare_phone (c_phone, x))
		if (compare_group (c_group, x))
			if (compare_name (c_name, x))
				return true;
	return false;
}
