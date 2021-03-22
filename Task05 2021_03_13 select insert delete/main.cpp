#include <stdio.h>
#include <time.h>

#include "record.h"
#include "list_2.h"
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
	int res = 0;
	int i;
	char c;
	bool ret;

	for (i = 0; (i < LEN) && (c = fgetc(fp)) && (c != EOF); i++)
	{
		if(c == ';')
		{
			buf[i] = '\0';
			ret = command.parse (buf);
			
			if (command.get_type () == Command_type::quit)
				return res;
			if (ret)
				command.apply (base, res);
			
			i = -1;
			buf[0] = '\0';
		}
		else if (c == '\n')
			i--;
		else
			buf[i] = c;
	}
	if (i >= LEN)
	{
		printf ("Buffer size exceeded (process())\n");
		return -2;
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
	
	time = clock ();
	res = process (base);
	time = clock () - time;
	
	printf ("%s : Result = %d Elapsed = %.2lf\n", argv[0], res, (double) time / CLOCKS_PER_SEC);
	return SUCCESS;
}

