#ifndef QUERY_PROCESSING_ORDER
#define QUERY_PROCESSING_ORDER
enum class Query_processing_order {
	order_none,
	order_invalid,
	order_one_by_one, // Соответствие по номеру слова в строке.
	order_once_enough, // Хотя бы одно.
	order_all, // Совпадение для всех слов должно найтись.
};
#endif // QUERY_PROCESSING_ORDER.
