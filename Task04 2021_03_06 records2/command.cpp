#include <stdio.h>
#include <cstring>
#include "condition.h"
#include "command.h"

static const int LEN = 1234;

static void to_lower_case (char * str, int len = LEN)
{
	for (int i = 0; (i < len) && str[i]; i++)
	{
		if ((str[i] <= 'Z') && (str[i] >= 'A'))
			str[i] += 'a' - 'A';
	}
}	

static void skip_spaces (const char *& str)
{
	str += strspn (str, " \t\n");
}

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
}

bool Command::service_read_ordering (const char *& str)
{
	const int Ordering_length = 3;
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
		return true;
	order = std::make_unique<Ordering []> (Ordering_length);
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

bool Command::service_read_conditions (const char *& str)
{
	const char * str_pos = str;
	char * name = nullptr;
	int phone = -1, group = -1;
	char field_name[LEN], operator_name[LEN], expression_name[LEN], operation_name[LEN];
	
	if (hightlight_words (str_pos, field_name, operator_name, expression_name) != 3)
		return false;
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
		to_lower_case (operation_name);
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
				return false;
		}
	}
	if (!strcmp (field_name, "phone"))
	{
		this->c_phone = get_condition (operator_name);
		if (sscanf (expression_name, "%d", &phone) != 1)
			return false;
		set_phone (phone);
	} 
	else if (!strcmp (field_name, "group"))
	{
		this->c_group = get_condition (operator_name);
		if (sscanf (expression_name, "%d", &group) != 1)
			return false;
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
