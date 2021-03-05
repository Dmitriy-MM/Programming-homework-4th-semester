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
	
	char * get_name () const { return name.get(); }
	int get_phone () const { return phone; }
	int get_group () const { return group; }
	int init (const char * n, int p, int q);
	int read (FILE *fp = stdout);
	// Allow as return value fro function.
	Record (Record &&x) = default; // Move constructor.
	// Assignement move operator.
	Record& operator= (Record&& x) = default;
	// Prohoibit pass by value.
	// (it is default when move constructor is declared).
	Record (const Record &x) = delete;
	// Prohoibit assignement operator
	// (it is default when move constructor is declared).
	Record& operator= (const Record&) = delete;
	// Check condition ’x’ for field ’name’ for ’this’ and ’y’
//bool compare_name (condition x, const Record& y) const;
	// Check condition ’x’ for field ’phone’ for ’this’ and ’y’
//bool compare_phone (Condition x, const Record& y) const;
	// Check condition ’x’ for field ’group’ for ’this’ and ’y’
//bool compare_group (condition x, const Record& y) const;
	void print (FILE * fp = stdout) const;
	//Read_status read (FILE *fp = stdin);
	bool compare_group (Condition x, const Record& y) const;
	bool compare_name (Condition x, const Record& y) const;
	bool compare_phone (Condition x, const Record& y) const;
};

#endif // RECORD_H.
