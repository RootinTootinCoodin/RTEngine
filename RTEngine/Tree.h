#ifndef __TREE_H__
#define __TREE_H__

#include "Globals.h"
#include "MathGeoLib/Geometry/AABB.h" //I dont like this, but i get compiler errors otherwise

class GameObject;

class Tree
{
	class Node
	{
	public:
		Node() {};
		Node(AABB area, const Tree* tree, const Node* parent) : nodeArea(area), tree(tree), parent(parent) {};
		~Node() {};

		void Draw();
		void Split();

		void Clear();
		void Intersect(std::vector<const GameObject*>& group, const AABB & area);

		AABB nodeArea;
		const Tree* tree = nullptr;
		std::vector<const GameObject*> containedGameobj;
		const Node* parent;
		std::vector<Node*> children;

	};

public:
	Tree(AABB aabb);
	~Tree();

	void Create(const AABB& limits); // Re-creates a new tree from an existing one
	void Clear();

	void Insert(GameObject* newItem);
	void Remove(GameObject* itemToRemove);

	void Intersect(std::vector<const GameObject*>& group, const AABB& area);

	Node* root;
	std::vector<const GameObject*> containedGameobj;

};

#endif // __TREE_H__