#ifndef COMPARATOR
#define COMPARATOR

#include "ordering.h"
#include "command.h"
#include "record.h"

class Cmp_record_by_order: public Record
{
private:
	static const int Max_items = Command::Max_items;
	Ordering order[Max_items] = { Ordering::none };
	Cmp_record_by_order () = default;
public:
	Cmp_record_by_order (Ordering * order)
	{
		for (int i = 0; i < Max_items; i++)
		{
			this->order[i] = order[i];
		}
	}
	~Cmp_record_by_order () = default;
	
	int cmp (Record& r1, Record& r2)
	{
		int i, condition = 0;
		for (i = 0; i < Max_items; i++)
		{
			switch (order[i])
			{
				case Ordering::none:
					break;
				case Ordering::name:
					if (!(r1.get_name() && r2.get_name()))
						return 0;
					condition = strcmp (r1.get_name(), r2.get_name());
					if (condition)
						return condition;
					break;
				case Ordering::group:
					condition = r1.get_group () - r2.get_group ();
					if (condition)
						return condition;
					break;
				case Ordering::phone:
					condition = r1.get_phone () - r2.get_phone ();
					if (condition)
						return condition;
					break;
			}
		}
		return 0;
	}
};

#endif // COMPARATOR.