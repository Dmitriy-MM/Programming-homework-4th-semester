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

#endif // RECORD_H.
