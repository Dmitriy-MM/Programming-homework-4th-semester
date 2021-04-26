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

static int read_config (int& k, int& p, const char * config_name)
{
	FILE *fp;
	char buf[LEN] = {'\0'};
	const char * pos = nullptr;
	int is_k_read = 0;
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
		if (!is_k_read && (sscanf (pos, "%d", &k) == 1))
		{
			is_k_read = 1;
		}
		if (is_k_read && (sscanf (pos, "%d", &p) == 1))
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
#define DEBUG
#ifdef DEBUG
			printf ("%s start\n", buf);
#endif
			if (command.get_type () == Command_type::quit)
				return res;
			if (ret)
				delete command.apply (base, res);
#ifdef DEBUG
			//printf ("%s end\n", buf);
#endif 		
			
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



static void calc_cfg_path (const char * prog_path, char * cfg_path) {
	char * pos = nullptr, * prev_pos = nullptr;
	const char prog_name[] = "a.out";
	const char config_name[] = "config.txt";
	const char execution_sequence[] = "./";
	int shift = 0;
	if (strstr (prog_path, execution_sequence) == prog_path)
		shift = strlen (execution_sequence);
	strcpy (cfg_path, prog_path + shift);
	for (prev_pos = pos = cfg_path; pos != nullptr; pos++)
	{
		pos = strstr (pos, prog_name);
		if (pos == nullptr)
			break;
		prev_pos = pos;
	}
	if (prev_pos == nullptr)
	{
		*cfg_path = '\0';
		return;
	}
	strcpy (prev_pos, config_name);
	//printf ("%s\n", cfg_path);
}

template <class T>
int List_2<T>::m = 0;
template <class T>
int List_2<T>::r = 3;
int main (int argc, char * argv[])
{
	int res;
	const int LEN = 1234;
	char config_path [LEN] = {'\0'};
	clock_t time;
	int k, p;
	FILE * fp;
	
	if (!(argc == 2))
	{
		printf ("Usage: %s a.txt\n", argv[0]);
		return BAD_INPUT_ERROR;
	}
	
	calc_cfg_path (argv[0], config_path);
	
	if (read_config (k, p, config_path))
	{
		printf ("Wrong config.txt\n");
		return WRONG_CONFIG_ERROR;
	}
	else
	{
		Database base (k, p);
		fp = fopen (argv[1], "r");
		if (fp == nullptr)
		{
			printf ("Can't open file %s\n", argv[1]);
			return BAD_INPUT_ERROR;
		}
		res = base.read (fp);
		fclose (fp);
		if (res)
		{
			printf ("Can't read base\n");
			return READ_BASE_ERROR;
		}
		
		printf ("Base have read\n");
		time = clock ();
		res = process(base);
		time = clock () - time;
		//base.print_trees ();
		//base.print_links ();
		printf ("%s : Result = %d Elapsed = %.2lf\n", argv[0], res, (double) time / CLOCKS_PER_SEC);
		//printf ("called %lu\n", Database::get_called());
		return SUCCESS;
	}
}

