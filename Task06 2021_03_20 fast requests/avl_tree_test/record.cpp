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


void Record::print (const Ordering order[], FILE *fp) const
{
	const int max_items = 3;
	const Ordering default_ordering[max_items] = {Ordering::name, Ordering::phone, Ordering::group};
	const Ordering * p = (order ? order : default_ordering);
	for (int i = 0; i < max_items; i++)
		switch (p[i])
		{
			case Ordering::name:
				printf (" %s", name.get()); break;
			case Ordering::phone:
				printf (" %d", phone); break;
			case Ordering::group:
				printf (" %d", group); break;
			case Ordering::none:
				continue;
		}
	fprintf (fp, "\n");
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
		case Condition::nlike:
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
		case Condition::nlike:
			return false;
	}
	return false;
}

bool Record::compare_name (Condition x, const Record& y) const
{
	int cmpres = 0;
	if (!(this->get_name() && y.get_name()))
		return true;
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
		case Condition::nlike:
			return !Word_regexp::check_is_suitable (y.get_name(), this->get_name());
	}
	return false;
}
