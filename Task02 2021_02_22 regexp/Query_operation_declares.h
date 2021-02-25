#ifndef QUERY_OPERATION
#define QUERY_OPERATION
enum class Query_operation {
	operation_none,
	operation_invalid,
	operation_eq, // ==
	operation_ne, // !=
	operation_me, // >=
	operation_mr, // >
	operation_le, // <=
	operation_ls, // <
};
#endif