#ifndef AVL_TREE
#define AVL_TREE

// Адаптировано с сайта https://habr.com/ru/post/150732/.
/*
 * Шаблонная реализация AVL дерева-списков, с поддержкой метода [void print (int max_level, void (*print_data_func) (T)) const;],
 * основые методы: find, insert, remove,
 * базовые методы:
 * get/set root,
 * get/set compare_func,
 * get/set compare_arg,
 * также compare_func и compare_arg можно указывать явно для основных методов (крайне не рекомендуется).
 * 
 * Node: поддержаны только базовые методы - get/set left/right.
 */
 
template <typename T> class AVLt_tree;
template <typename T>
class AVLt_tree_node {
private:
	// Properties.
	T data;
	AVLt_tree_node * left = nullptr;
	AVLt_tree_node * right = nullptr;
	AVLt_tree_node * next = nullptr;
	unsigned char height = 1;
	
	// Constructors.
	// Methods.
	void remove (void) {
		delete this;
	}
	void clean_pointers (void) {
		left = nullptr;
		right = nullptr;
		next = nullptr;
	}
	AVLt_tree_node * rotate_right (void) {
		AVLt_tree_node * q = this->left;
		this->left = q->right;
		q->right = this;
		this->to_fix_height ();
		q->to_fix_height ();
		return q;
	}
	AVLt_tree_node * rotate_left (void) {
		AVLt_tree_node * q = this->right;
		this->right = q->left;
		q->left = this;
		this->to_fix_height ();
		q->to_fix_height ();
		return q;
	}
	void to_fix_height (void) {
		unsigned char hl, hr;
		hl = get_height (left), hr = get_height (right);
		height = (hl > hr? hl: hr) + 1;
	}
	static unsigned char get_height (AVLt_tree_node * p) {
		if (p != nullptr)
			return p->height;
		return 0;
	}
		
public:
	// Properties.
	// Constructors.
	AVLt_tree_node () = default;
	AVLt_tree_node (AVLt_tree_node&& el) {
		this->data = el->data;
		this->left = el->left;
		this->right = el->right;
		this->next = el->next;
		el->left = el->right = el->next = nullptr;
	}
	AVLt_tree_node (const AVLt_tree_node& el) : AVLt_tree_node () {
		this->data = el->data;
	}
	AVLt_tree_node (T data): AVLt_tree_node () {
		this->data = data;
	}
	~AVLt_tree_node () {
		left = nullptr;
		right = nullptr;
		next = nullptr;
	}
	
	// Methods.
	T get (void) const { return data; }
	void set (T new_data) { data = new_data; }
	AVLt_tree_node * get_left (void) const {
		return left;
	}
	AVLt_tree_node * get_right (void) const {
		return right;
	}
	AVLt_tree_node * get_next (void) const {
		return next;
	}
	
	void set_left (AVLt_tree_node * el) {
		left = el;
	}
	void set_right (AVLt_tree_node * el) {
		right = el;
	}
	void set_next (AVLt_tree_node * el) {
		next = el;
	}
	
	friend AVLt_tree<T>;
};

template <typename T>
class AVLt_tree {
private:
	// Properties.
	AVLt_tree_node<T> * root = nullptr;
	int (*compare_func) (T, T, void *) = nullptr;
	void *compare_arg = nullptr;
	
	// Constructors.
	AVLt_tree () = default; // Explicit prohibition of default constructor.
	
	// Methods.
	void service_choose_cmp_func (int (*&cmp) (T, T, void*), void *& cmp_arg) {
		if (cmp == nullptr)
		{
			cmp = compare_func;
			cmp_arg = compare_arg;
		}
	}
	void clear_subtree (AVLt_tree_node<T> * el) {
		if (el == nullptr)
			return;
		if (el->left != nullptr)
			clear_subtree (el->left);
		if (el->right != nullptr)
			clear_subtree (el->right);
		if (el->next != nullptr)
			clear_subtree (el->next);
		delete el;
	}
	static int get_balance_factor (AVLt_tree_node<T> * p) {
		return (int) AVLt_tree_node<T>::get_height (p->right) - AVLt_tree_node<T>::get_height (p->left);
	}
	static AVLt_tree_node<T> * to_balance (AVLt_tree_node<T> * p) {
		p->to_fix_height();
		if(get_balance_factor (p) == 2)
		{
			if(get_balance_factor (p->right) < 0 )
				p->right = p->right->rotate_right();
			return p->rotate_left();
		}
		if(get_balance_factor(p) == -2)
		{
			if(get_balance_factor (p->left) > 0)
				p->left = p->left->rotate_left();
			return p->rotate_right();
		}
		return p; // Балансировка не нужна.
	}
	static AVLt_tree_node<T> * find_min (AVLt_tree_node<T> * p) {
		return (p->left != nullptr)? find_min (p->left): p;
	}
	static AVLt_tree_node<T> * remove_min (AVLt_tree_node<T> * p) {
		if (p->left == nullptr)
			return p->right;
		p->left = remove_min (p->left);
		return to_balance (p);
	}
	
	static AVLt_tree_node<T> * insert_in_subtree (AVLt_tree_node<T> *& el, AVLt_tree_node<T> * sub_root, int (*cmp) (T, T, void *) = nullptr, void * compare_arg = nullptr) {
		int cmp_res;
		cmp_res = cmp (el->get(), sub_root->get(), compare_arg);
		if (cmp_res < 0)
		{
			AVLt_tree_node<T> * left = sub_root->left;
			if (left != nullptr)
				sub_root->left = insert_in_subtree (el, left, cmp, compare_arg);
			else
				sub_root->left = el;
		}
		else if (cmp_res > 0)
		{
			AVLt_tree_node<T> * right = sub_root->right;
			if (right != nullptr)
				sub_root->right = insert_in_subtree (el, right, cmp, compare_arg);
			else
				sub_root->right = el;
		}
		else
		{
			//T data = el->get ();
			//for (AVLt_tree_node<T> * cur = sub_root; cur != nullptr; cur = cur->next) 
			//{
				//if (cur->get() == data)
				//{
					//delete el;
					//el = nullptr;
				//}
			//}
			AVLt_tree_node<T> * cur, * prev;
			for (prev = cur = sub_root; cur != nullptr; cur = cur->next)
				prev = cur;
			prev->next = el;
		}
		return to_balance (sub_root);
	}
	static AVLt_tree_node<T> * remove_from_subtree (AVLt_tree_node<T> * sub_root, AVLt_tree_node<T> * el, int (*cmp) (T, T, void *), void * compare_arg) {
		/*
		 * Надо вырезать минимальный элемент из правого поддерева, вставить его на место el,
		 * и сделать балансировку.
		 */
		int cmp_res;
		cmp_res = cmp (el->get(), sub_root->get(), compare_arg);
		if (cmp_res < 0)
			sub_root->left = remove_from_subtree (sub_root->left, el, cmp, compare_arg);
		else if (cmp_res > 0)
			sub_root->right = remove_from_subtree (sub_root->right, el, cmp, compare_arg);
		else
		{
			AVLt_tree_node<T> * left = el->left, *right = el->right, *next = el->next;
			//if (sub_root != el) // Удалять элемент чужого дерева нельзя (удаление может делать только метод того дерева, где элемент лежит).
				//return to_balance (sub_root);
			//delete el;
			if (sub_root != el) // Удалять элемент чужого дерева нельзя (удаление может делать только метод того дерева, где элемент лежит).
			{
				AVLt_tree_node<T> * cur;
				for (cur = sub_root; cur != nullptr; cur = cur->next)
					if (cur->next == el)
					{
						cur->next = el->next;
						delete el;
					} 
				return to_balance (sub_root);
			}
			delete el;
			if (next != nullptr)
			{
				next->left = left;
				next->right = right;
				return to_balance (next);
			}
			if(right == nullptr) 
				return left;
			else 
			{	
				AVLt_tree_node<T> * min = find_min (right);
				min->right = remove_min (right);
				min->left = left;
				return to_balance (min);
			}
		}
		
		return to_balance (sub_root);
	}
	
	static AVLt_tree_node<T> * remove_from_subtree_by_key (AVLt_tree_node<T> * sub_root, T key, int (*cmp) (T, T, void *), void * compare_arg) {
		/*
		 * Надо вырезать минимальный элемент из правого поддерева, вставить его на место el,
		 * и сделать балансировку.
		 */
		int cmp_res;
		cmp_res = cmp (key, sub_root->get(), compare_arg);
		if (cmp_res < 0)
			sub_root->left = remove_from_subtree_by_key (sub_root->left, key, cmp, compare_arg);
		else if (cmp_res > 0)
			sub_root->right = remove_from_subtree_by_key (sub_root->right, key, cmp, compare_arg);
		else
		{
			AVLt_tree_node<T> * left = sub_root->left, *right = sub_root->right, *next = sub_root->next;
			
			delete sub_root;
			if (next != nullptr)
			{
				next->left = left;
				next->right = right;
				return to_balance (next);
			}
			if(right == nullptr) 
				return left;
			else 
			{	
				AVLt_tree_node<T> * min = find_min (right);
				min->right = remove_min (right);
				min->left = left;
				return to_balance (min);
			}
		}
		
		return to_balance (sub_root);
	}

	static AVLt_tree_node<T> * find_in_subtree (T key, AVLt_tree_node<T> * sub_root, int (*cmp) (T, T, void *), void * compare_arg) {
		int cmp_res;
		if (sub_root == nullptr)
			return nullptr;
		cmp_res = cmp (key, sub_root->get(), compare_arg);
		if (cmp_res < 0)
			return find_in_subtree (key, sub_root->left, cmp, compare_arg);
		else if (cmp_res > 0)
			return find_in_subtree (key, sub_root->right, cmp, compare_arg);
		else 
		{
			AVLt_tree_node<T> * cur;
			for (cur = sub_root; cur != nullptr; cur = cur->next)
				if (cur->get() == key)
					return sub_root;
			return nullptr;
		}
	}
	
	void print_branch (int max_level, int level, AVLt_tree_node<T> * nd, void (*print_data_func) (T), int position = 0) const { // (T).
		int spaces = level-1;
		int i;
		enum {
			Head = 0,
			Left,
			Right,
			Next, 
		};
		
		if (max_level <= level) return;
		for (i = 0; i < spaces; i++)
			printf ("   |");
		if (i < level)
			printf ("   ");
		
		if (position == Left)
			printf ("L)");
		else if (position == Right)
			printf ("R)");
		else if (position == Head)
			printf ("H)");
		else if (position == Next)
			printf ("->)");
		print_data_func (nd->get());
		//printf ("\n");
		if (nd->next)
			print_branch (max_level, level, nd->next, print_data_func, Next);
		if (nd->left)
			print_branch (max_level, level + 1, nd->left, print_data_func, Left);
		if (nd->right)
			print_branch (max_level, level + 1, nd->right, print_data_func, Right);
		
	}
public:
	// Properties.
	// Constructors.
	AVLt_tree (int (*cmp) (T, T, void *), void * cmp_arg = nullptr): AVLt_tree () {
		compare_func = cmp;
		compare_arg = cmp_arg;
	}
	~AVLt_tree () { clear_subtree (root); root = nullptr; }
	
	// Methods.
	int check_tree () {
		int res;
		if (root == nullptr) {
			printf ("true avl - empty\n");
			return 0;
		}
		fix_heights (root);
		res = check_avl_heights (root);
		if (res == 0)
			printf ("true avl\n");
		else
			printf ("nah, not avl\n");
		printf ("deep %d\n", root->height);
		return res;
	}
	int fix_heights (AVLt_tree_node<T> * el) {
		int lh = 0, rh = 0, max;
		if (el->right != nullptr)
			rh = fix_heights (el->right);
		if (el->left != nullptr)
			lh = fix_heights (el->left);
		max = (lh>rh)?lh:rh;
		el->height = max + 1;
		return max+1;
	}
	int check_avl_heights (AVLt_tree_node<T> * el){
		// 0 - avl
		// 1 - not_avl
		int lh = 0, rh = 0, dif;
		if (el->left != nullptr)
		{
			lh = el->left->height;
			if (check_avl_heights (el->left) != 0)
				return 1;
		}
		if (el->right != nullptr)
		{
			rh = el->right->height;
			if (check_avl_heights (el->right) != 0)
				return 2;
		}
		dif = (lh > rh)? lh - rh: rh - lh;
		if (dif <= 1)
			return 0;
		else
			return 3;
	}
	
	
	AVLt_tree_node<T> * get_root (void) const {
		return root;
	}
	void set_root (AVLt_tree_node<T> * new_root) {
		root = new_root;
	}
	int (*get_compare_func (void)) (T, T, void *) const {
		return compare_func;
	}
	void set_compare_func (int (*cmp) (T, T, void*)) {
		compare_func = cmp;
	}
	void * get_compare_arg (void) const {
		return compare_arg;
	}
	void set_compare_arg (void * cmp_arg) {
		compare_arg = cmp_arg;
	}
	void print (int max_level, void (*print_data_func) (T)) const {
		if (root != nullptr)
			print_branch (max_level, 0, root, print_data_func);
		else
			printf ("Empty tree\n");
	}
	AVLt_tree_node<T> * find (T key, int (*cmp) (T, T, void *) = nullptr, void * compare_arg = nullptr) {
		service_choose_cmp_func (cmp, compare_arg);
		return find_in_subtree (key, root, cmp, compare_arg);
	}
	
	AVLt_tree_node<T> * insert (AVLt_tree_node<T> *& el, int (*cmp) (T, T, void *) = nullptr, void * compare_arg = nullptr) {
		if (el == nullptr)
			return el;
		el->clean_pointers ();
		if (root == nullptr) 
		{
			root = el;
			return root;
		}
		service_choose_cmp_func (cmp, compare_arg);
		root = insert_in_subtree (el, root, cmp, compare_arg);
		return root;
	}
	AVLt_tree_node<T> * insert (T data, int (*cmp) (T, T, void *) = nullptr, void * compare_arg = nullptr) {
		AVLt_tree_node<T> * el = new AVLt_tree_node<T> (data);
		if (el != nullptr)
			if (insert (el, cmp, compare_arg) == nullptr)
			{
				delete el;
				el = nullptr;
			}
		return el;
	}
	
	void remove (AVLt_tree_node<T> * el, int (*cmp) (T, T, void *) = nullptr, void * compare_arg = nullptr) {
		if (el == nullptr)
			return;
		service_choose_cmp_func (cmp, compare_arg);
		root = remove_from_subtree (root, el, cmp, compare_arg);
	}
	void remove (T key, int (*cmp) (T, T, void *) = nullptr, void * compare_arg = nullptr) {
		service_choose_cmp_func (cmp, compare_arg);
		root = remove_from_subtree_by_key (root, key, cmp, compare_arg);
	}
};
#endif // AVL_TREE.
