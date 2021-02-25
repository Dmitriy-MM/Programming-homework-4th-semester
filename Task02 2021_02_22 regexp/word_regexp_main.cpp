#include <cstring>
#include <cstdio>

// Реализация с элементами конечного автомата.


enum class Symbol_states {
	start,
	check_next,
	literal_symbol,
	underline,
	backslash,
	percent,
	range,
	reversed_range,
	end,
};


static int all_conditions_for_range_state (const char * str)
{
	// Проверка того, что начало строки имеет вид "[a-b]" или "[^a-b]" (здесь пока не все).
	if (!(strlen (str) >= 5))
		return 0;
	if ((strlen (str) - strcspn (str, "]")) <= 0)
		return 0;
	return 1;
}


class Word_chekcker {
private:
	enum Range_mismatch {
		NONE,
		MISMATCH,
	} range_mismatch = Range_mismatch::NONE;
	const char * str_pos = nullptr;
	const char * regexp_pos = nullptr;
	Symbol_states state = Symbol_states::start;
	// Range-properties.
	char range_left_border = 0;
	char range_right_border = -1;
	int is_reversed_range = 0;
	int shift_of_bad_range_input = 0;
	// Skip-properties.
	const char * regexp_checkpoint = nullptr;
	const char * str_checkpoint = nullptr;
	int skip_shift = -1;
	
	void skip_bad_range_region ()
	{
		// Есть два варианта пропуска неудачного региона - полностью пропустить то, для чего была попытка прочтения,
		// либо просто пропустить один символ посчитав его литеральным, и продолжить обработку с этого момента.
		regexp_pos -= shift_of_bad_range_input;
		// Выбираем второй вариант обработки (один символ пропускаем, считая литеральным, остальное обрабатываем заново).
		shift_of_bad_range_input = 1;
	}
	int range_processing ()
	{
		char smb;
		
		shift_of_bad_range_input = 0;
		range_mismatch = Range_mismatch::NONE;
		
		regexp_pos++, shift_of_bad_range_input++;
		if (*regexp_pos == '^')
		{
			is_reversed_range = 1;
			regexp_pos++, shift_of_bad_range_input++;
		}
		if (*regexp_pos == '\\')
			regexp_pos++, shift_of_bad_range_input++;
		range_left_border = *regexp_pos;
		
		regexp_pos++, shift_of_bad_range_input++;
		if (*regexp_pos != '-')
		{
			printf ("SOMETHING WENT WRONG range_processing()->can't find '-'\n");
			skip_bad_range_region ();
			return 1;
		}
		
		regexp_pos++, shift_of_bad_range_input++;
		if (*regexp_pos == '\\')
			regexp_pos++, shift_of_bad_range_input++;
		range_right_border = *regexp_pos;
		
		regexp_pos++, shift_of_bad_range_input++;
		if (*regexp_pos != ']')
		{
			printf ("SOMETHING WENT WRONG range_processing()->can't find '-'\n");
			skip_bad_range_region ();
			return 1;
		}
		
		shift_of_bad_range_input = 0;
		smb = *str_pos;
		regexp_pos++;
		if (is_reversed_range)
		{
			if ((smb < range_left_border) || (smb > range_right_border))
				return 1;
			return 0;
		}
		else
		{
			//printf ("%c <= %c <= %c\n", range_left_border, smb, range_right_border);
			if ((smb >= range_left_border) && (smb <= range_right_border))
			{
				//printf ("da %c <= %c <= %c\n", range_left_border, smb, range_right_border);
				return 1;
			}
			return 0;
		}
	}
	
	Word_chekcker () = default; // Explicit prohibition of all constructors.
	Word_chekcker (const char *s1, const char *s2) : Word_chekcker () {
		str_pos = s1, regexp_pos = s2;
		state = Symbol_states::start;
	}
	~Word_chekcker() {}
public:
	int next () {
		switch (state) {
			case Symbol_states::start:
			case Symbol_states::check_next:
				if (shift_of_bad_range_input > 0)
				{
					shift_of_bad_range_input--;
					state = Symbol_states::literal_symbol;
					break;
				}
				
				if (*regexp_pos == '\\') { // *regexp_pos поменять на smb.
					state = Symbol_states::backslash;
				} else if (*regexp_pos == '%') {
					state = Symbol_states::percent;
					break;
				} else if (*regexp_pos == '_') {
					state = Symbol_states::underline;
				} else if (*regexp_pos == '[') {
					if (all_conditions_for_range_state (regexp_pos))
						state = Symbol_states::range;
					else
						state = Symbol_states::literal_symbol;
				} else if (*regexp_pos == '\0') {
					state = Symbol_states::end;
				} else state = Symbol_states::literal_symbol;
				
				if (*str_pos == '\0')
					state = Symbol_states::end;
				break;
				
			case Symbol_states::literal_symbol:
				if (*str_pos != *regexp_pos)
					state = Symbol_states::end;
				else
				{
					str_pos++, regexp_pos++;
					state = Symbol_states::check_next;
				}
				break;
			
			case Symbol_states::backslash:
				regexp_pos++;
				state = Symbol_states::literal_symbol;
				break;
			
			case Symbol_states::underline:
				if (*str_pos == '\0')
				{
					state = Symbol_states::end;
				}
				else
				{
					regexp_pos++, str_pos++;
					state = Symbol_states::check_next;
				}
				break;
				
			case Symbol_states::percent:
				if (*regexp_pos == '%')
				{
					if (regexp_checkpoint == nullptr)
					{
						
						regexp_pos += strspn (regexp_pos, "%"); // Пропуск повторяющихся процентов.
						regexp_checkpoint = regexp_pos;
						printf ("%s\n", regexp_checkpoint);
						str_checkpoint = str_pos;
						state = Symbol_states::check_next;
					}
					else
					{
						// Рекурсия (как жаль...) вместо создания списка чекпоинтов.
						return check_is_suitable (str_pos, regexp_pos);
					}
				}
				break;
	
			case Symbol_states::range:
				if (!range_processing ())
				{
					range_mismatch = Range_mismatch::MISMATCH;
					state = Symbol_states::end; // Там тоже будет 0 и выход с результатом несоответствия.
				}
				else
				{
					str_pos++;
					state = Symbol_states::check_next;
				}
				break;
			case Symbol_states::end:
				if ((*regexp_pos == '\0') && (*str_pos == '\0'))
					return 1;
				if (regexp_checkpoint != nullptr)
				{
					skip_shift++;
					regexp_pos = regexp_checkpoint;
					str_pos = str_checkpoint + skip_shift;
					state = Symbol_states::check_next;
					if (*str_pos != '\0')
						break;
				}
			
				if (range_mismatch == Range_mismatch::MISMATCH)
					return 0;
				if (*str_pos != *regexp_pos)
					return 0;
				break;
			default:
				printf ("SOMETHING WENT WRONG next()->switch\n");
				break;
		}
		return -10;
	}
	static int check_is_suitable (const char *s1, const char *s2)
	{
		Word_chekcker checker (s1, s2);
		int result = 0;
		int i;
		
		for (i = 0; ((result = checker.next()) != 1) && (result != 0) && (i < (1 << 10)); i++)
		{}

		// printf ("i had counted to %d\n", i);
		return result;
	}
	
};


int main (int argc, char * argv[])
{
	int is_suitable;
	
	if (argc != 3)
	{
		printf ("Usage: %s str regexp\n", argv[0]);
		return -1;
	}
	
	is_suitable = Word_chekcker::check_is_suitable (argv[1], argv[2]);
	if (is_suitable == 1)
		printf ("Suitable\n");
	else if (is_suitable == 0)
		printf ("Not suitable\n");
	else
		printf ("Something went wrong, main ()\n");
	
	return 0;
}
