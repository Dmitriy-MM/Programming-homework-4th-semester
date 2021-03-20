#include <stdio.h>
#include <time.h>

#include "record.h"
#include "list_2.h"
#include "command.h"
#include "comparator.h"

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
				// int i = 99;
	
	fp = fopen (filename, "r");
	ret = base.read (fp);
	if (ret != List_2<Record>::RET_CODES::SUCCESS)
	{
		fclose (fp);
		return ret;	
	}
	fclose (fp);
	// {
		// Ordering order[] = {Ordering::name, Ordering:: phone, Ordering:: group};
		// Cmp_record_by_order cmp (order);
		// base.sort ((((Record&) cmp).cmp));
		// base.sort (((Record&) cmp).cmp, cmp);
	// }
	
	List_2<Record>::set_r (100);
	// base.print ();
	// printf ("\n--------------\nReverse_print:\n");

	// for (List_2_node<Record> * tail = base.get_tail (); tail != nullptr; tail = tail->get_prev(), i--)
	// {printf ("%2d) ", i);tail->print ();}
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
	
	// Record * rec = new Record;
	// rec->init ("govno", 1992825, 208);
	// List_2_node<Record> * cur = nullptr;
	// base.insert (*rec);
					// Record rec[5];
					// for (i = 0; i < 5; i++)
					// {
						// rec[i].init ("govno", 1992825, 208);
						// base.insert (rec[i]);
					// }
					// rec[1].init ("govno2", 1992825, 208);
					// base.insert (rec[1]);
					// base.print();
						// (void) ret; (void) cur;
	// rec.print();
	// while (fgets (buf, LEN, fp))
	// {
		// ret = command.parse (buf);
		// if (ret)
			// for (cur = base.get_head(); cur != nullptr; cur = cur->get_next())
			// {
				// if (command.apply (*cur))
				// {
					// cur->print (command.get_order (), fout);
					// res++;
				// }
			// }
	// }
	for (i = 0; (i < LEN) && (c = fgetc(fp)) && (c != EOF); i++)
	{
		if(c == ';')
		{
			//printf ("parsing \"%s\"\n", buf);
			buf[i] = '\0';
			ret = command.parse (buf);
			
			//base.print();
			if (command.get_type () == Command_type::quit)
				return res;
			if (ret)
				command.apply (base, res);
			
			//if (ret && (command.get_type () == Command_type::select))
				//for (cur = base.get_head(); cur != nullptr; cur = cur->get_next())
				//{
					//if (command.apply (*cur))
					//{
						////cur->print (command.get_order (), fout);
						//res++;
					//}
				//}
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

