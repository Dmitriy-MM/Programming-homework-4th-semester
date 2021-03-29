#ifndef AVL_TREE
#define AVL_TREE

// Адаптировано с сайта https://habr.com/ru/post/150732/.
/*
 * Шаблонная реализация AVL дерева-списков, с поддержкой методов read (требует T.read), print (требует T.print),
 * основые методы: find (требует наличие оператора сравнения на равенство [T.@operator==]), insert, remove,
 * базовые методы:
 * get/set root,
 * get/set compare_func,
 * get/set compare_arg,
 * также compare_func и compare_arg можно указывать явно для основных методов (крайне не рекомендуется).
 * 
 * Node: поддержаны только базовые методы - get/set left/right/next.
 */
template <class T> class AVL_tree;
template <class T>
class AVL_tree_node : public T {
private:
	// Properties.
	AVL_tree_node * left = nullptr;
	AVL_tree_node * right = nullptr;
	AVL_tree_node * next = nullptr;
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
	AVL_tree_node * rotate_right (void) {
		AVL_tree_node * q = this->left;
		this->left = q->right;
		q->right = this;
		this->to_fix_height ();
		q->to_fix_height ();
		return q;
	}
	AVL_tree_node * rotate_left (void) {
		AVL_tree_node * q = this->right;
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
	static unsigned char get_height (AVL_tree_node * p) {
		if (p != nullptr)
			return p->height;
		return 0;
	}
		
public:
	// Properties.
	// Constructors.
	AVL_tree_node () = default;
	AVL_tree_node (AVL_tree_node&& el): T ((T&&) el) {}
	AVL_tree_node (const AVL_tree_node& el): T (el) {}
	~AVL_tree_node () {
		left = nullptr;
		right = nullptr;
		next = nullptr;
	}
	
	// Methods.
	AVL_tree_node * get_left (void) const {
		return left;
	}
	AVL_tree_node * get_right (void) const {
		return right;
	}
	AVL_tree_node * get_next (void ) const {
		return next;
	}
	
	void set_left (AVL_tree_node * el) {
		left = el;
	}
	void set_right (AVL_tree_node * el) {
		right = el;
	}
	void set_next (AVL_tree_node * el) {
		next = el;
	}
	
	friend AVL_tree<T>;
};

template <class T>
class AVL_tree {
private:
	// Properties.
	AVL_tree_node<T> * root = nullptr;
	int (*compare_func) (T&, T&, void *) = nullptr;
	void *compare_arg = nullptr;
	
	// Constructors.
	AVL_tree () = default; // Explicit prohibition of default constructor.
	
	// Methods.
	void service_choose_cmp_func (int (*&cmp) (T&, T&, void*), void *& cmp_arg) {
		if (cmp == nullptr)
		{
			cmp = compare_func;
			cmp_arg = compare_arg;
		}
	}
	void clear_subtree (AVL_tree_node<T> * el) {
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
	static int get_balance_factor (AVL_tree_node<T> * p) {
		return (int) AVL_tree_node<T>::get_height (p->right) - AVL_tree_node<T>::get_height (p->left);
	}
	static AVL_tree_node<T> * to_balance (AVL_tree_node<T> * p) {
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
	static AVL_tree_node<T> * find_min (AVL_tree_node<T> * p) {
		return (p->left != nullptr)? find_min (p->left): p;
	}
	static AVL_tree_node<T> * remove_min (AVL_tree_node<T> * p) {
		if (p->left == nullptr)
			return p->right;
		p->left = remove_min (p->left);
		return to_balance (p);
	}
	
	static AVL_tree_node<T> * insert_in_subtree (AVL_tree_node<T> * el, AVL_tree_node<T> * sub_root, int (*cmp) (T&, T&, void *) = nullptr, void * compare_arg = nullptr) {
		int cmp_res;
		cmp_res = cmp (*el, *sub_root, compare_arg);
		if (cmp_res < 0)
		{
			AVL_tree_node<T> * left = sub_root->left;
			if (left != nullptr)
				sub_root->left = insert_in_subtree (el, left, cmp, compare_arg);
			else
				sub_root->left = el;
		}
		else if (cmp_res > 0)
		{
			AVL_tree_node<T> * right = sub_root->right;
			if (right != nullptr)
				sub_root->right = insert_in_subtree (el, right, cmp, compare_arg);
			else
				sub_root->right = el;
		}
		else
		{
			AVL_tree_node<T> * cur, * prev;
			for (prev = cur = sub_root; cur != nullptr; cur = cur->next)
				prev = cur;
			prev->next = el;
		}
		return to_balance (sub_root);
	}
	static AVL_tree_node<T> * remove_from_subtree (AVL_tree_node<T> * sub_root, AVL_tree_node<T> * el, int (*cmp) (T&, T&, void *), void * compare_arg) {
		/*
		 * Надо вырезать минимальный элемент из правого поддерева, вставить его на место el,
		 * и сделать балансировку.
		 */
		int cmp_res;
		cmp_res = cmp (*el, *sub_root, compare_arg);
		if (cmp_res < 0)
			sub_root->left = remove_from_subtree (sub_root->left, el, cmp, compare_arg);
		else if (cmp_res > 0)
			sub_root->right = remove_from_subtree (sub_root->right, el, cmp, compare_arg);
		else
		{
			AVL_tree_node<T> * left = el->left, *right = el->right, *next = el->next;
			if (sub_root != el) // Удалять элемент чужого дерева нельзя (удаление может делать только метод того дерева, где элемент лежит).
			{
				AVL_tree_node<T> * cur;
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
				AVL_tree_node<T> * min = find_min (right);
				min->right = remove_min (right);
				min->left = left;
				return to_balance (min);
			}
		}
		
		return to_balance (sub_root);
	}

	static AVL_tree_node<T> * find_in_subtree (T& key, AVL_tree_node<T> * sub_root, int (*cmp) (T&, T&, void *), void * compare_arg) {
		int cmp_res;
		if (sub_root == nullptr)
			return nullptr;
		cmp_res = cmp (key, *sub_root, compare_arg);
		if (cmp_res < 0)
			return find_in_subtree (key, sub_root->left, cmp, compare_arg);
		else if (cmp_res > 0)
			return find_in_subtree (key, sub_root->right, cmp, compare_arg);
		else 
		{
			for (AVL_tree_node<T> * cur = sub_root; cur != nullptr; cur = cur->next)
				if (*cur == key)
					return cur;
			return nullptr;
		}
	}
	void print_branch (int max_level, int level, AVL_tree_node<T> * nd, int position = 0) const {
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
		nd->print ();
		//printf ("\n");
		if (nd->next)
			print_branch (max_level, level, nd->next, Next);
		if (nd->left)
			print_branch (max_level, level + 1, nd->left, Left);
		if (nd->right)
			print_branch (max_level, level + 1, nd->right, Right);
	}
public:
	// Properties.
	// Constructors.
	AVL_tree (int (*cmp) (T&, T&, void *), void * cmp_arg = nullptr): AVL_tree () {
		compare_func = cmp;
		compare_arg = cmp_arg;
	}
	~AVL_tree () { clear_subtree (root); root = nullptr; }
	
	// Methods.
	AVL_tree_node<T> * get_root (void) const {
		return root;
	}
	void set_root (AVL_tree_node<T> * new_root) {
		root = new_root;
	}
	int (*get_compare_func (void)) (T&, T&, void *) const {
		return compare_func;
	}
	void set_compare_func (int (*cmp) (T&, T&, void*)) {
		compare_func = cmp;
	}
	void * get_compare_arg (void) const {
		return compare_arg;
	}
	void set_compare_arg (void * cmp_arg) {
		compare_arg = cmp_arg;
	}
	void print (int max_level) const {
		if (root != nullptr)
			print_branch (max_level, 0, root);
		else
			printf ("Empty tree\n");
	}
	AVL_tree_node<T> * find (T& key, int (*cmp) (T&, T&, void *) = nullptr, void * compare_arg = nullptr) {
		service_choose_cmp_func (cmp, compare_arg);
		return find_in_subtree (key, root, cmp, compare_arg);
	}
	
	void insert (AVL_tree_node<T> * el, int (*cmp) (T&, T&, void *) = nullptr, void * compare_arg = nullptr) {
		if (el == nullptr)
			return;
		el->clean_pointers ();
		if (root == nullptr) 
		{
			root = el;
			return;
		}
		service_choose_cmp_func (cmp, compare_arg);
		root = insert_in_subtree (el, root, cmp, compare_arg);
	}
	
	void remove (AVL_tree_node<T> * el, int (*cmp) (T&, T&, void *) = nullptr, void * compare_arg = nullptr) {
		if (el == nullptr)
			return;
		service_choose_cmp_func (cmp, compare_arg);
		root = remove_from_subtree (root, el, cmp, compare_arg);
	}
	
	int read (const char * filename, int (*cmp) (T&, T&, void *) = nullptr, void * compare_arg = nullptr) {
		FILE *fp = nullptr;
		AVL_tree_node<T> tmp;
		AVL_tree_node<T> * curr = nullptr;
		enum ret_codes {
			SUCCESS = 0,
			ERROR_NOT_REACH_EOF,
			ERROR_OPEN_FILE,
			ERROR_READ_CURR
		};
		
		service_choose_cmp_func (cmp, compare_arg);
		if (!(fp = fopen (filename, "r")))
			return ERROR_OPEN_FILE;
		while (tmp.read (fp) == SUCCESS)
		{
			curr = new AVL_tree_node<T> ((AVL_tree_node<T>&&)(tmp));
			if (curr == nullptr)
			{
				fclose (fp);
				return ERROR_READ_CURR;
			}
			if (root == nullptr)
				root = curr;
			else
				insert (curr, cmp, compare_arg);
		}
		
		if (!feof (fp))
		{
			fclose (fp);
			return ERROR_NOT_REACH_EOF;
		}
		fclose (fp);
		return SUCCESS;
	}
};
#endif // AVL_TREE.
