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
		Node(AABB area, Tree* tree, const Node* parent) : nodeArea(area), tree(tree), parent(parent) {};
		~Node() {};

		void Draw();
		void Split4();

		void Clear();
		void CollectIntersections(std::vector<const GameObject*>& group, const AABB & area);
		bool Insert(const GameObject* newItem);

		bool IsLeaf() { return nodeObjects.empty(); }
		bool IsBranch() { return (!nodeObjects.empty() && parent != nullptr); }
		bool IsRoot() { return (parent == nullptr); }

		AABB nodeArea;
		Tree* tree = nullptr;
		std::vector<const GameObject*> nodeObjects;
		const Node* parent;
		std::vector<Node*> children;
	};

public:
	Tree(AABB aabb);
	~Tree();

	void Create(const AABB& limits); // Clears and recreates the tree
	void Clear();

	void Insert(GameObject* newItem);
	void Remove(GameObject* itemToRemove);

	void CollectIntersections(std::vector<const GameObject*>& collector, const AABB& area);

	int currentSubdivisions = 0;

	Node* root;
	std::vector<GameObject*> treeObjects;
	int bucket = 1;

};

#endif // __TREE_H__