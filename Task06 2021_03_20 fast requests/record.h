#ifndef RECORD_H
#define RECORD_H

#include <memory>
#include <stdio.h>
#include <cstring>
#include "condition.h"
#include "ordering.h"

enum Read_status
{
	success = 0,
	eof,
	format,
	memory,
};

class Record
{
private:
	std::unique_ptr<char []> name = nullptr;
	int phone = 0;
	int group = 0;
public:
	Record () = default;
	~Record () = default;
	Record (Record &&x) = default;
	Record& operator= (Record&& x) = default;
	Record (const Record &x) = delete;
	Record& operator= (const Record&) = delete;
	int operator== (const Record& r)
	{
		if (
			(phone != r.phone) ||
			(group != r.group) ||
			( 
				((get_name() != nullptr) && (r.get_name() != nullptr)) &&
				(strcmp (get_name(), r.get_name()) != 0)
			) ||
			((get_name() != nullptr) && (r.get_name() == nullptr)) ||
			((get_name() == nullptr) && (r.get_name() != nullptr))
		)
			return 0;
		return 1;
	}
	
	char * get_name () const { return name.get(); }
	int get_phone () const { return phone; }
	int get_group () const { return group; }
	
	int init (const char * n, int p, int q);
	int read (FILE *fp = stdin);
	void print (FILE * fp) const;
	void print (const Ordering * order = nullptr, FILE * fp = stdout) const;
	
	void set_phone (int p) { phone = p; }
	void set_group (int g) { group = g; }
	int set_name (const char * n) {
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
	
	bool compare_group (Condition x, const Record& y) const;
	bool compare_name (Condition x, const Record& y) const;
	bool compare_phone (Condition x, const Record& y) const;
};

class Record_ptr {
public:
	Record * ptr = nullptr;
	Record_ptr () = default;
	Record_ptr (Record * ptr) { this->ptr = ptr; }
	Record& operator* ()
	{
		return *ptr;
	}
	int operator== (const Record_ptr& p)
	{
		return (*ptr == *(p.ptr));
	}
	void print (FILE*fp = stdout)
	{ ptr->print (fp); }
};
#endif // RECORD_H.
