#include <string.h>
#include <stdio.h>
#include "record.h"
#include "Word_regexp.h"

#define LEN 1234

using namespace std;

int Record::init (const char *n, int p, int g)
{
	phone = p;
	group = g;
	if (n)
	{
		name = std::make_unique<char []> (strlen (n) + 1);
		if (!name) return -1;
		strcpy (name.get(), n);
	}
	else
		name = nullptr;
	return 0;
}

void Record::print (FILE *fp) const
{
	//fprintf (fp, "name = \"%s\" phone = %d group = %d\n", name.get(), phone, group);
	fprintf (fp, "%s %d %d\n", name.get(), phone, group);
}

int Record::read (FILE *fp)
{
	char buf[LEN];
	name = nullptr;
	if (fscanf (fp, "%s%d%d", buf, &phone, &group) != 3)
	{
		if (feof(fp)) return Read_status::eof;
		return Read_status::format;
	}
	if (init (buf, phone, group))
		return Read_status::memory;
	
	return Read_status::success;
}


bool Record::compare_phone (Condition x, const Record& y) const
{
	switch (x)
	{
		case Condition::none: // not specified
			return true; // unspecified opeation is true
		case Condition::eq: // equal
			return y.phone == phone;
		case Condition::ne: // not equal
			return y.phone != phone;
		case Condition::lt: // less than
			return y.phone < phone;
		case Condition::gt: // less than
			return y.phone > phone;
		case Condition::le: // less equal
			return y.phone <= phone;
		case Condition::ge: // great equal
			return y.phone >= phone;
		case Condition::like: // strings only: match pattern
			return false; // cannot be used for phone
	}
	return false;
}

bool Record::compare_group (Condition x, const Record& y) const
{
	switch (x)
	{
		case Condition::none: // not specified
			return true; // unspecified opeation is true
		case Condition::eq: // equal
			return y.group == group;
		case Condition::ne: // not equal
			return y.group != group;
		case Condition::lt: // less than
			return y.group < group;
		case Condition::gt: // less than
			return y.group > group;
		case Condition::le: // less equal
			return y.group <= group;
		case Condition::ge: // great equal
			return y.group >= group;
		case Condition::like: // strings only: match pattern
			return false; // cannot be used for group
	}
	return false;
}

bool Record::compare_name (Condition x, const Record& y) const
{
	int cmpres = 0;
	if (this->get_name() != nullptr)
		cmpres = strcmp (y.get_name(), this->get_name());
	switch (x)
	{
		case Condition::none: // not specified
			return true; // unspecified opeation is true
		case Condition::eq: // equal
			return cmpres == 0;
		case Condition::ne: // not equal
			return cmpres != 0;
		case Condition::lt: // less than
			return cmpres < 0;
		case Condition::gt: // less than
			return cmpres > 0;
		case Condition::le: // less equal
			return cmpres <= 0;
		case Condition::ge: // great equal
			return cmpres >= 0;
		case Condition::like: // strings only: match pattern
			return Word_regexp::check_is_suitable (y.get_name(), this->get_name()); // cannot be used for group
	}
	return false;
}

