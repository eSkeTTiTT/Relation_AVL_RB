#include "relation.h"

int main()
{
	relation<Employee> a;

	Employee daun1 = { 228, "daun1", 1337.228 };
	Employee daun2 = { 322, "daun2", 1337.322 };
	Employee daun3 = { 1337, "daun3", 1337.1337 };

	Employee daun1_for_delete = { 228, "daun1", 1337.228 };
	Employee daun2_for_delete = { 322, "daun2", 1337.322 };
	Employee daun3_for_delete = { 1337, "daun3", 1337.1337 };
	
	a.push_item(daun1);
	a.push_item(daun2);
	a.push_item(daun3);

	auto tree_guide1 = a.add_index(AVL, new Compare_id<Employee*>);
	auto tree_guide2 = a.add_index(RB, new Compare_name<Employee*>);

	if (a.search_item(daun1_for_delete, tree_guide2)) {
		cout << "lox\n";
	}

	a.delete_item(daun1_for_delete);
	a.delete_item(daun2_for_delete);
	a.delete_item(daun3_for_delete);

	a.delete_index(tree_guide1);
	a.delete_index(tree_guide2);

	return 0;
}