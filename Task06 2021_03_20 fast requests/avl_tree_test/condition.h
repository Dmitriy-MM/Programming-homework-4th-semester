#ifndef CONDITION_H
#define CONDITION_H
enum class Condition
{
	none, // not specified
	eq, // equal
	ne, // not equal
	lt, // less than
	gt, // less than
	le, // less equal
	ge, // great equal
	like, // strings only: match pattern
	nlike, // strings only: match pattern
};
#endif // CONDITION_H.
