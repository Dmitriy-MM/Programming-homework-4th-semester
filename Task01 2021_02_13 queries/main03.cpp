#include <cstdio>
#include <ctime>

#include "Query.h"

#define LEN 1234

enum FUNC_RET_CODES 
{
	F_SUCCESS = 0,
	ERROR_CANT_REACH_EOF,
	ERROR_CANT_OPEN_FIN,
	ERROR_CANT_OPEN_FOUT
};


enum MAIN_RET_CODES
{
	SUCCESS = 0,
	WRONG_INPUT
};

static void rm_line_feed (char * str, int max_length)
{
	int i;
	str[max_length-1] = '\0';
	for (i = 0; str[i] && (i < max_length); i++)
	{
		if (str[i] == '\n')
		{
			str[i] = '\n';
			break;
		}
	}
}


static int make_s_replace_x (
	const char * filename,
	const char * filename2,
	const char * s,
	const char * t,
	const char * x)
{
	FILE * fin, *fout;
	char buf[LEN] = {'\0'};
	int count_right_strs = 0;
	
	Words_rider s_words (s, t), x_words (x, t);
	int cmp_res, n;
	int buf_word_length, s_word_length;
	char buf_word_buf [WORD_LEN], s_word_buf[WORD_LEN];
	
	fin = fopen (filename, "r");
	if (!fin)
	{ return -ERROR_CANT_OPEN_FIN; }
	fout = fopen (filename2, "w");
	if (!fout)
	{
		fclose (fin);
		return -ERROR_CANT_OPEN_FOUT;
	}
	
	while (fgets (buf, LEN, fin))
	{
		Words_rider buf_words (buf, t); // Что-то очень непроизводительное (выделять память каждую итерацию).
		char output_buf[LEN] = {'\0'};
		
		rm_line_feed (buf, LEN);
		strncpy (output_buf, buf, strspn (buf, t));
		while ((buf_word_length = buf_words.get_word_length ()) != 0)
		{			
			s_words.reset_pos ();
			while ((s_word_length = s_words.get_word_length ()) != 0)
			{
				buf_words.extract_word_to (buf_word_buf, WORD_LEN);
				s_words.extract_word_to (s_word_buf, WORD_LEN);
				
				cmp_res = strcmp (buf_word_buf, s_word_buf);
				if (cmp_res == 0)
				{
					n = s_words.get_cur_num ();
					x_words.extract_N_word_to (n, buf_word_buf, WORD_LEN);
					strcat (output_buf, buf_word_buf);
					count_right_strs++;
					break;
				}
				
				s_words.next();
			}
			if (s_word_length == 0)
			{
				strcat (output_buf, buf_word_buf);
			}
			buf_words.extract_tabs_after_word (output_buf + strlen(output_buf), WORD_LEN);
			
			buf_words.next();
		}
		
		fprintf (fout, "%s", output_buf);
	}
	if (!feof (fin))
	{
		fclose (fin), fclose (fout);
		return -ERROR_CANT_REACH_EOF;
	}
	fclose (fin), fclose (fout);
	return count_right_strs;
}

int main (int argc, char ** argv)
{
	int task = 1, res;
	clock_t t_elapsed;
	
	if (!(
		(argc == 6)
	))
	{
		printf ("Usage: %s input.txt output.txt \"input_str\" \"tab_symbols_str\" \"replaced_str\"\n", argv[0]);
		return WRONG_INPUT;
	}
	
	t_elapsed = clock ();
	res = make_s_replace_x (argv[1], argv[2], argv[3], argv[4], argv[5]);
	t_elapsed = clock () - t_elapsed;
	printf ("%s : Task = %d Result = %d Elapsed = %.2lf\n", argv[0], task, res, (double) t_elapsed/CLOCKS_PER_SEC);
	
	return SUCCESS;
}