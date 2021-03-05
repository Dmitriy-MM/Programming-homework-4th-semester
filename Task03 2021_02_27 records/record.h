#ifndef RECORD_H
#define RECORD_H

#include <memory>
#include <stdio.h>
#include "condition.h"

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
	int read (FILE *fp = stdout);
	void print (FILE * fp = stdout) const;
	
	bool compare_group (Condition x, const Record& y) const;
	bool compare_name (Condition x, const Record& y) const;
	bool compare_phone (Condition x, const Record& y) const;
};

#endif // RECORD_H.
