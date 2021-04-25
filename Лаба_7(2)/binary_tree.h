#pragma once
#include "includes.h"
template<typename> class Compare;


template<typename T> class Strategy
{
public:
	virtual int compare(const T& left, const T& right) const = 0;
};

template<typename T> class binary_tree
{
private:
	Strategy<T>* strategy_;

protected:
	Strategy<T>* get_strategy() { return strategy_; }

public:
	binary_tree(Strategy<T>* strategy) : strategy_(strategy) {}

	~binary_tree() { if (strategy_) delete strategy_; }

	virtual void addItem(T&& value) = 0;
	virtual void addItem(const T& value) = 0;

	virtual void deleteItem(T&& value) = 0;
	virtual void deleteItem(const T& value) = 0;

	virtual bool searchItem(T&& value) = 0;
	virtual bool searchItem(const T& value) = 0;
};

template<typename T> class AVL_tree : public binary_tree<T>
{
private:
	Node_AVL<T>* root_AVL; // ������

	int Height(Node_AVL<T>* root)
	{
		if (root) { return root->height_; }
		else return 0;
	}

	int DifferenceHeight(Node_AVL<T>* root)
	{
		return Height(root->right) - Height(root->left);
	}

	void OverHeight(Node_AVL<T>* root)
	{
		int hleft, hright;
		hleft = Height(root->left);
		hright = Height(root->right);
		root->height_ = (hleft > hright ? hleft : hright) + 1;
	}

	void LeftRotation(Node_AVL<T>** root)
	{
		Node_AVL<T>* ptr = (*root)->right;
		(*root)->right = ptr->left;
		ptr->left = (*root);
		OverHeight(*root);
		OverHeight(ptr);

		(*root) = ptr;
	}

	void RightRotation(Node_AVL<T>** root)
	{
		Node_AVL<T>* ptr = (*root)->left;
		(*root)->left = ptr->right;
		ptr->right = (*root);
		OverHeight(*root);
		OverHeight(ptr);

		(*root) = ptr;
	}

	void Balance(Node_AVL<T>** root)
	{
		OverHeight(*root);

		if (DifferenceHeight(*root) == 2)
		{
			if (DifferenceHeight((*root)->right) < 0) { RightRotation(&((*root)->right)); }
			LeftRotation(root);
		}
		else if (DifferenceHeight(*root) == -2)
		{
			if (DifferenceHeight((*root)->left) > 0) { LeftRotation(&((*root)->left)); }
			RightRotation(root);
		}
	}

	Node_AVL<T>* SearchMin(Node_AVL<T>* root)
	{
		if (root->left) { return SearchMin(root->left); }
		else { return root; }
	}

	void DeleteMin(Node_AVL<T>** root)
	{
		if (!(*root)->left) { (*root) = (*root)->right; return; }
		DeleteMin(&(*root)->left);
		Balance(root);
	}

	void insert(Node_AVL<T>** root, const T& value)
	{
		int strategy = this->get_strategy()->compare(value, (*root)->data_);

		if (strategy == -1) {
			if (!(*root)->left) { (*root)->left = new Node_AVL<T>(value); OverHeight((*root)); return; } // ����� ������ ��� �� �������� � Balance() 
			else { insert(&(*root)->left, value); }
		}
		else if (strategy == 1) {
			if (!(*root)->right) { (*root)->right = new Node_AVL<T>(value); OverHeight((*root)); return; }
			else { insert(&(*root)->right, value); }
		}
		else { return; }

		Balance(root);
	}

	void Delete(Node_AVL<T>** root, const T& value)
	{
		int strategy = this->get_strategy()->compare(value, (*root)->data_);

		if (strategy == -1) {
			if (!(*root)->left) { return; }
			else { Delete(&(*root)->left, value); }
		}
		else if (strategy == 1) {
			if (!(*root)->right) { return; }
			else { Delete(&(*root)->right, value); }
		}
		else {
			Node_AVL<T>* left = (*root)->left;
			Node_AVL<T>* right = (*root)->right;
			delete (*root);

			if (!right) { (*root) = left; return; }

			*root = SearchMin(right);
			DeleteMin(&right);
			(*root)->right = right;
			(*root)->left = left;
		}

		Balance(root);
	}

	bool Search(Node_AVL<T>* root, const T& value)
	{
		int strategy = this->get_strategy()->compare(value, root->data_);

		if (strategy == -1) {
			if (root->left) { return Search(root->left, value); }
			else { return false; }
		}
		else if (strategy == 1) {
			if (root->right) { return Search(root->right, value); }
			else { return false; }
		}
		else { return true; }
	}

	void delete_memory_tree(Node_AVL<T>* ptr)
	{
		if (ptr) {
			delete_memory_tree(ptr->left);
			delete_memory_tree(ptr->right);
			delete ptr;
		}
	}

public:
	AVL_tree(Strategy<T>* strategy) : binary_tree<T>(strategy) { this->root_AVL = nullptr; }
	AVL_tree(Strategy<T>* strategy, const T& value) : binary_tree<T>(strategy) { this->root_AVL = new Node_AVL<T>(value); }
	AVL_tree(Strategy<T>* strategy, T&& value) : binary_tree<T>(strategy) { this->root_AVL = new Node_AVL<T>(value); }
	~AVL_tree()
	{
		if (this->root_AVL) delete_memory_tree(this->root_AVL);
	}

	void addItem(T&& value) override
	{
		if (!root_AVL) { root_AVL = new Node_AVL<T>(value); return; }
		insert(&root_AVL, value);
	}

	void addItem(const T& value) override
	{
		if (!root_AVL) { root_AVL = new Node_AVL<T>(value); return; }
		insert(&root_AVL, value);
	}

	void deleteItem(T&& value) override
	{
		if (root_AVL) { Delete(&root_AVL, value); }
	}

	void deleteItem(const T& value) override
	{
		if (root_AVL) { Delete(&root_AVL, value); }
	}

	bool searchItem(T&& value) override
	{
		if (!root_AVL) { return false; }

		return Search(root_AVL, value);
	}

	bool searchItem(const T& value) override
	{
		if (!root_AVL) { return false; }

		return Search(root_AVL, value);
	}
};





template<typename T> class RB_tree : public binary_tree<T>
{
private:
	Node_RB<T>* root_RB; // ������ 

	void RBInsert(Node_RB<T>** root, Node_RB<T>* ptr)
	{
		int strategy = this->get_strategy()->compare(ptr->data_, (*root)->data_);

		if (strategy == -1) {
			if ((*root)->left == NIL<T>) { (*root)->left = ptr; (*root)->left->parent = (*root); return; }
			else { RBInsert(&(*root)->left, ptr); }
		}
		else if (strategy == 1) {
			if ((*root)->right == NIL<T>) { (*root)->right = ptr; (*root)->right->parent = (*root); return; }
			else { RBInsert(&(*root)->right, ptr); }
		}
		else { return; }
	}

	void rotateLeft(Node_RB<T>*& root, Node_RB<T>*& ptr)
	{
		Node_RB<T>* ptr_right = ptr->right;

		ptr->right = ptr_right->left;
		if (ptr->right != NIL<T>) { ptr->right->parent = ptr; }

		ptr_right->parent = ptr->parent;

		if (!ptr->parent) { root = ptr_right; }
		else if (ptr == ptr->parent->left) { ptr->parent->left = ptr_right; }
		else { ptr->parent->right = ptr_right; }

		ptr_right->left = ptr;
		ptr->parent = ptr_right;
	}

	void rotateRight(Node_RB<T>*& root, Node_RB<T>*& ptr)
	{
		Node_RB<T>* ptr_left = ptr->left;

		ptr->left = ptr_left->right;
		if (ptr->left != NIL<T>) { ptr->left->parent = ptr; }

		ptr_left->parent = ptr->parent;

		if (!ptr->parent) { root = ptr_left; }
		else if (ptr == ptr->parent->left) { ptr->parent->left = ptr_left; }
		else { ptr->parent->right = ptr_left; }

		ptr_left->right = ptr;
		ptr->parent = ptr_left;
	}

	void FixDelete(Node_RB<T>** root, Node_RB<T>* ptr)
	{

		while (ptr != *root && !ptr->color)
		{
			if (ptr == ptr->parent->left) {
				Node_RB<T>* brother = ptr->parent->right;
				if (brother->color) {
					brother->color = false;
					ptr->parent->color = true;
					rotateLeft(*root, ptr->parent);
					brother = ptr->parent->right;
				}

				if (!brother->left->color && !brother->right->color) {
					brother->color = true;
					ptr = ptr->parent;
				}
				else {
					if (!brother->right->color) {
						brother->left->color = false;
						brother->color = true;
						rotateRight(*root, brother);
						brother = ptr->parent->right;
					}
					brother->color = ptr->parent->color;
					ptr->parent->color = false;
					brother->right->color = false;
					rotateLeft(*root, ptr->parent);
					ptr = *root;
				}
			}
			else {
				Node_RB<T>* brother = ptr->parent->left;
				if (brother->color) {
					brother->color = false;
					ptr->parent->color = true;
					rotateRight(*root, ptr->parent);
					brother = ptr->parent->left;
				}

				if (!brother->left->color && !brother->right->color) {
					brother->color = true;
					ptr = ptr->parent;
				}
				else {
					if (!brother->left->color) {
						brother->right->color = false;
						brother->color = true;
						rotateLeft(*root, brother);
						brother = ptr->parent->left;
					}
					brother->color = ptr->parent->color;
					ptr->parent->color = false;
					brother->left->color = false;
					rotateRight(*root, ptr->parent);
					ptr = *root;
				}
			}
		}

		ptr->color = false;
	}

	void RBDelete(Node_RB<T>* root, const T& value)
	{
		if (!root || root == NIL<T>) { return; } // end

		int strategy = this->get_strategy()->compare(value, root->data_);

		if (strategy == -1) {
			RBDelete(root->left, value);
		}
		else if (strategy == 1) {
			RBDelete(root->right, value);
		}
		else {
			Node_RB<T>* ptr;
			Node_RB<T>* temp;

			if (root->left == NIL<T> || root->right == NIL<T>) { ptr = root; } // 1 ������� ��� 0
			else { // 2 �������
				ptr = root->right;
				while (ptr->left != NIL<T>) { ptr = ptr->left; }
			}

			if (ptr->left != NIL<T>) { temp = ptr->left; } // ���� ��� ����� ������
			else { temp = ptr->right; } // ���� ��� ������ ������� ��� �� ������ �� ������������ ������

			temp->parent = ptr->parent;

			if (ptr->parent) {
				if (ptr == ptr->parent->left) ptr->parent->left = temp;
				else ptr->parent->right = temp;
			}
			else { root_RB = temp; }

			if (ptr != root) {
				root->data_ = ptr->data_;
			} // ����� �������� �� ������������ ������ � ����� �������� ��������

			if (!ptr->color) { FixDelete(&root_RB, temp); }

			delete ptr;
			return;
		}
	}

	void FixInsert(Node_RB<T>** root, Node_RB<T>* ptr)
	{
		Node_RB<T>* parent_ptr = nullptr;
		Node_RB<T>* grand_parent_ptr = nullptr;

		while (ptr != (*root) && ptr->color && ptr->parent->color)
		{
			parent_ptr = ptr->parent;
			grand_parent_ptr = ptr->parent->parent;

			//A
			if (parent_ptr == grand_parent_ptr->left)
			{
				Node_RB<T>* uncle_ptr = grand_parent_ptr->right;

				//1 uncle �������
				if (uncle_ptr != NIL<T> && uncle_ptr->color)
				{
					grand_parent_ptr->color = true;
					parent_ptr->color = false;
					uncle_ptr->color = false;
					ptr = grand_parent_ptr;
				}
				else
				{
					//2 ptr ������ ������� ������ ��������
					if (ptr == parent_ptr->right)
					{
						rotateLeft(*root, parent_ptr);
						ptr = parent_ptr;
						parent_ptr = ptr->parent;

					}
					//3 �������� ������
					rotateRight(*root, grand_parent_ptr);
					swap(parent_ptr->color, grand_parent_ptr->color);
					ptr = parent_ptr;
				}
			}
			else //B �������� - ������ �������
			{
				Node_RB<T>* uncle_ptr = grand_parent_ptr->left;

				//1 uncle �������
				if (uncle_ptr != NIL<T> && uncle_ptr->color)
				{
					grand_parent_ptr->color = true;
					parent_ptr->color = false;
					uncle_ptr->color = false;
					ptr = grand_parent_ptr;
				}
				else
				{
					//2 ptr ����� ������� ������ ��������
					if (ptr == parent_ptr->left)
					{
						rotateRight(*root, parent_ptr);
						ptr = parent_ptr;
						parent_ptr = ptr->parent;

					}
					//3 �������� �����
					rotateLeft(*root, grand_parent_ptr);
					swap(parent_ptr->color, grand_parent_ptr->color);
					ptr = parent_ptr;
				}
			}
		}

		(*root)->color = false; // ������ ������ ������
	}

	bool RBSearch(Node_RB<T>*& root, const T& value)
	{
		int strategy = this->get_strategy()->compare(value, root->data_);

		if (root == NIL<T>) { return false; }

		if (strategy == -1) { return RBSearch(root->left, value); }
		else if (strategy == 1) { return RBSearch(root->right, value); }
		else { return true; }
	}

	void delete_memory_tree(Node_RB<T>* ptr)
	{
		if (ptr != NIL<T>) {
			delete_memory_tree(ptr->left);
			delete_memory_tree(ptr->right);
			delete ptr;
		}
	}

public:
	RB_tree(Strategy<T>* strategy) : binary_tree<T>(strategy) { this->root_RB = NIL<T>; }
	RB_tree(Strategy<T>* strategy, const T& value) : binary_tree<T>(strategy) {
		this->root_RB = new Node_RB<T>;
		this->root_RB->data_ = value;
		this->root_RB->left = this->root_RB->right = NIL<T>;
		this->root_RB->parent = nullptr;
		this->root_RB->color = false; // ������ ��� ������
	}
	RB_tree(Strategy<T>* strategy, T&& value) : binary_tree<T>(strategy) {
		this->root_RB = new Node_RB<T>;
		this->root_RB->data_ = value;
		this->root_RB->left = this->root_RB->right = NIL<T>;
		this->root_RB->parent = nullptr;
		this->root_RB->color = false;
	}
	~RB_tree()
	{
		if (this->root_RB != NIL<T>) delete_memory_tree(this->root_RB);
	}

	void addItem(T&& value) override
	{
		Node_RB<T>* ptr = new Node_RB<T>;
		ptr->data_ = value;
		ptr->left = ptr->right = NIL<T>;
		ptr->parent = nullptr;
		ptr->color = true;

		if (root_RB == NIL<T>) { root_RB = ptr; root_RB->color = false; return; }
		RBInsert(&root_RB, ptr);
		FixInsert(&root_RB, ptr);
	}

	void addItem(const T& value) override
	{
		Node_RB<T>* ptr = new Node_RB<T>;
		ptr->data_ = value;
		ptr->left = ptr->right = NIL<T>;
		ptr->parent = nullptr;
		ptr->color = true;

		if (root_RB = NIL<T>) { root_RB = ptr; root_RB->color = false; return; }
		RBInsert(&root_RB, ptr);
		FixInsert(&root_RB, ptr);
	}

	void deleteItem(T&& value) override
	{
		if (root_RB != NIL<T>) { RBDelete(root_RB, value); }
	}

	void deleteItem(const T& value) override
	{
		if (root_RB != NIL<T>) { RBDelete(root_RB, value); }
	}

	bool searchItem(T&& value) override
	{
		if (root_RB != NIL<T>) { return RBSearch(root_RB, value); }
		return false;
	}

	bool searchItem(const T& value) override
	{
		if (root_RB != NIL<T>) { return RBSearch(root_RB, value); }
		return false;
	}
};


