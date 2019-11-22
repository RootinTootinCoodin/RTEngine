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
		Node(AABB area, const Tree* tree);
		~Node() {};

		void Draw();
		void Split();

		void Clear();

		AABB nodeArea;
		const Tree* tree = nullptr;
		std::vector<const GameObject*> containedGameobj;
		Node* parent;
		std::vector<Node*> children;

	};

public:
	Tree(AABB aabb);
	~Tree();

	void Create(const AABB& limits);
	void Clear();
	void Insert(GameObject* newItem);
	void Remove(GameObject* itemToRemove);
	void Intersect(std::vector<const GameObject*>& group, const AABB& area);

	Node* root;
	std::vector<const GameObject*> containedGameobj;

};

#endif // __TREE_H__