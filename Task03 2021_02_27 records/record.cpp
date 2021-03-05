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
		case Condition::none: 
			return true;
		case Condition::eq: 
			return y.phone == phone;
		case Condition::ne: 
			return y.phone != phone;
		case Condition::lt:
			return y.phone < phone;
		case Condition::gt: 
			return y.phone > phone;
		case Condition::le: 
			return y.phone <= phone;
		case Condition::ge: 
			return y.phone >= phone;
		case Condition::like:
			return false;
	}
	return false;
}

bool Record::compare_group (Condition x, const Record& y) const
{
	switch (x)
	{
		case Condition::none: 
			return true; 
		case Condition::eq: 
			return y.group == group;
		case Condition::ne: 
			return y.group != group;
		case Condition::lt: 
			return y.group < group;
		case Condition::gt: 
			return y.group > group;
		case Condition::le: 
			return y.group <= group;
		case Condition::ge:
			return y.group >= group;
		case Condition::like: 
			return false; 
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
		case Condition::none: 
			return true; 
		case Condition::eq: 
			return cmpres == 0;
		case Condition::ne: 
			return cmpres != 0;
		case Condition::lt: 
			return cmpres < 0;
		case Condition::gt: 
			return cmpres > 0;
		case Condition::le: 
			return cmpres <= 0;
		case Condition::ge: 
			return cmpres >= 0;
		case Condition::like: 
			return Word_regexp::check_is_suitable (y.get_name(), this->get_name()); 
	}
	return false;
}