#pragma once
#include "binary_tree.h"

enum Trees
{
	AVL,
	RB,
	NOT_TREE
};

struct Employee
{
	int id_;
	string name_;
	double salary_;

	bool operator==(const Employee& other)
	{
		if (id_ == other.id_
			&& name_ == other.name_
			&& salary_ == other.salary_)
		{
			return true;
		}
		else return false;
	}
};

template<typename T> class relation
{
private:
	forward_list<T> list_;
	map<string, binary_tree<T*>*> list_index_;

	void DeleteItem(const T& value)
	{
		typename forward_list<T>::iterator forward_iter = list_.begin();
		typename forward_list<T>::iterator forward_iter_before = list_.before_begin();
		while (forward_iter != list_.end()) {
			if (*forward_iter == value) {
				typename map<string, binary_tree<T*>*>::iterator iter = list_index_.begin();
				while (iter != list_index_.end()) {
					iter->second->deleteItem(&(*forward_iter));
					iter++;
				}

				list_.erase_after(forward_iter_before);
				break;
			}
			forward_iter_before = forward_iter;
			forward_iter++;
		}
	}

	void DeleteIndex(string tree_guide, binary_tree<T*>* iter)
	{
		Trees type = search_trees(tree_guide);
		if (type == AVL) {
			delete(static_cast<AVL_tree<T*>*>(iter));
		}
		else if (type == RB) {
			delete(static_cast<RB_tree<T*>*>(iter));
		}
	}

	void PushItem(const T& value)
	{
		typename forward_list<T>::iterator forward_iter = list_.begin();
		while (forward_iter != list_.end()) { // проверка на повторки
			if (*forward_iter == value) { break; }
			forward_iter++;
		}
		if (forward_iter == list_.end()) { list_.push_front(value); } // если повтроки не нашлось
		else return;

		if (!list_index_.empty())
		{
			typename map<string, binary_tree<T*>*>::iterator iter = list_index_.begin();
			while (iter != list_index_.end()) {
				iter->second->addItem(&list_.front());
				iter++;
			}
		}
	}

	bool SearchItem(T& value, const string& tree_guide)
	{
		typename map<string, binary_tree<T*>*>::iterator iter = list_index_.find(tree_guide);

		if (iter != list_index_.end()) { return iter->second->searchItem(&value); }
		else { return false; }
	}

	string get_index(Trees type, binary_tree<T*>* tree) // получить уникальный идентификатор
	{
		string id;

		if (type == AVL) { id += "AVL_"; }
		else if (type == RB) { id += "RB_"; }

		stringstream ss;
		ss << tree;
		id += ss.str();

		return id;
	}
	void fill_tree(binary_tree<T*>* tree) // заполнить дерево когда добавили новый индекс
	{
		if (!list_.empty())
		{
			typename forward_list<T>::iterator iter = list_.begin();
			while (iter != list_.end()) {
				tree->addItem(&(*iter));
				iter++;
			}
		}
	}

	Trees search_trees(string tree_guide)
	{
		// получаем тип дерева 
		string type; type.reserve(4);
		
		for (int i = 0; i != 4; i++) {
			if (tree_guide[i] == '_' || tree_guide[i] == '\0') { break; }
			type.push_back(tree_guide[i]);
		}

		if (type == "AVL") return AVL;
		else if (type == "RB") return RB;
		else return NOT_TREE;
	}

public:

	void delete_item(T& value)
	{
		DeleteItem(value);
	}

	void delete_item(T&& value)
	{
		DeleteItem(value);
	}

	void delete_index(string& tree_guide)
	{
		typename map<string, binary_tree<T*>*>::iterator iter = list_index_.find(tree_guide);
		
		if (iter != list_index_.end()) {
			DeleteIndex(tree_guide, iter->second);
			list_index_.erase(iter);
		}

		tree_guide = "deleted";
	}

	void push_item(const T& value)
	{
		PushItem(value);
	}

	void push_item(T&& value)
	{
		PushItem(value);
	}

	bool search_item(T&& value, const string& tree_guide)
	{
		return SearchItem(value, tree_guide);
	}

	bool search_item(T& value, const string& tree_guide)
	{
		return SearchItem(value, tree_guide);
	}

	string add_index(Trees&& type, Strategy<T*>* compare)
	{
		if (type == AVL) {
			AVL_tree<T*>* temp = new AVL_tree<T*>(compare);
			fill_tree(temp);
			string id = get_index(type, temp);
			list_index_[id] = temp;
			return id;
		}
		else if (type == RB) {
			RB_tree<T*>* temp = new RB_tree<T*>(compare);
			fill_tree(temp);
			string id = get_index(type, temp);
			list_index_[id] = temp;
			return id;
		}
		else return "not tree";
	}

	~relation()
	{
		typename map<string, binary_tree<T*>*>::iterator iter = list_index_.begin();

		while (iter != list_index_.end()) {
			DeleteIndex(iter->first, iter->second);
			iter++;
		}

		list_index_.clear();
	}
};

// Compares
template<typename T> class Compare_id : public Strategy<T>
{
public:
	int compare(const T& first, const T& second) const override {
		if (first->id_ < second->id_)
			return -1;
		else if (first->id_ > second->id_)
			return 1;
		else
			return 0;
	}
};

template<typename T> class Compare_name : public Strategy<T>
{
public:
	int compare(const T& first, const T& second) const override {
		if (first->name_ < second->name_)
			return -1;
		else if (first->name_ > second->name_)
			return 1;
		else
			return 0;
	}
};

template<typename T> class Compare_salary : public Strategy<T>
{
public:
	int compare(const T& first, const T& second) const override {
		if (first->salary_ < second->salary_)
			return -1;
		else if (first->salary_ > second->salary_)
			return 1;
		else
			return 0;
	}
};


