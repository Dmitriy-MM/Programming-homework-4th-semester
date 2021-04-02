#include <stdio.h>
#include <time.h>

#include "database.h"
#include "database_command.h"
#include "record.h"
#include "list_2.h"
#include "command.h"

static const int LEN = 1234;

enum MAIN_RET_CODES {
	SUCCESS = 0,
	READ_BASE_ERROR,
	BAD_INPUT_ERROR,
	WRONG_CONFIG_ERROR
};

static int read_config (int& k)
{
	FILE *fp;
	const int LEN = 1234;
	const char * config_name = "config.txt";
	char buf[LEN] = {'\0'};
	const char * pos = nullptr;
	fp = fopen (config_name, "r");
	if (fp == nullptr)
	{
		return -1;
	}
	while (fgets (buf, LEN, fp))
	{
		if (*buf == '#')
			continue;
		pos = buf + strspn (buf, " \t\n");
		if (sscanf (pos, "%d", &k) == 1)
		{
			fclose (fp);
			return 0;
		}
	}
	fclose (fp);
	return -2;
}

static int process (Database& base, FILE *fp = stdin, FILE *fout = stdout)
{
	Database_command command;
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
	int k;
	FILE * fp;
	
	if (!(argc == 2))
	{
		printf ("Usage: %s a.txt\n", argv[0]);
		return BAD_INPUT_ERROR;
	}
	if (read_config (k))
	{
		printf ("Wrong config.txt\n");
		return WRONG_CONFIG_ERROR;
	}
	else
	{
		Database base (k);
		fp = fopen (argv[1], "r");
		if (fp == nullptr)
		{
			printf ("Can't open file %s\n", argv[1]);
		}
		res = base.read (fp);
		fclose (fp);
		if (res)
		{
			printf ("Can't read base\n");
			return READ_BASE_ERROR;
		}
		
		time = clock ();
		res = process(base);
		time = clock () - time;
		//base.print_trees ();
		printf ("%s : Result = %d Elapsed = %.2lf\n", argv[0], res, (double) time / CLOCKS_PER_SEC);
		return SUCCESS;
	}
}

