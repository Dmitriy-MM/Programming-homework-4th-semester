#include <stdio.h>
#include <time.h>

#include "record.h"
#include "list2/list_2.h"
#include "command.h"

static const int LEN = 1234;

enum MAIN_RET_CODES {
	SUCCESS = 0,
	READ_BASE_ERROR,
	BAD_INPUT_ERROR
};

static int read_base (const char * filename, List_2<Record> & base)
{
	FILE *fp = nullptr;
	int ret = 0;
	
	fp = fopen (filename, "r");
	ret = base.read (fp);
	if (ret != List_2<Record>::RET_CODES::SUCCESS)
	{
		fclose (fp);
		return ret;	
	}
	fclose (fp);
	return 0;
}

static int process (List_2<Record> & base, FILE *fp = stdin, FILE *fout = stdout)
{
	Command command;
	char buf[LEN] = {'\0'};
	bool ret;
	int res = 0;
	List_2_node<Record> * cur = nullptr;
	
	while (fgets (buf, LEN, fp))
	{
		ret = command.parse (buf);
		if (ret)
			for (cur = base.get_head(); cur != nullptr; cur = cur->get_next())
			{
				if (command.apply (*cur))
				{
					//fprintf (fout, "match ");
					cur->print (fout);
					res++;
				}
			}
	}
	if (!feof (fp))
	{
		fprintf (fout, "Can't reach EOF (process())\n");
		return -1;
	}
	return res;
}


template <class T>
int List_2<T>::m = 0;
template <class T>
int List_2<T>::r = 3;
int main (int argc, char * argv[])
{
	int res;
	clock_t time;
	List_2<Record> base;
	
	if (!(argc == 2))
	{
		printf ("Usage: %s a.txt\n", argv[0]);
		return BAD_INPUT_ERROR;
	}
	res = read_base (argv[1], base);
	if (res)
	{
		printf ("Can't read base\n");
		return READ_BASE_ERROR;
	}
	
	//base.print (stdout);
	time = clock ();
	res = process (base);
	time = clock () - time;
	
	printf ("%s : Result = %d Elapsed = %.2lf\n", argv[0], res, (double) time / CLOCKS_PER_SEC);
	
	return SUCCESS;
}
