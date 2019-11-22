#include "Tree.h"

Tree::Tree(AABB aabb)
{
	root = new Node(aabb, this);
}

Tree::~Tree()
{
}

void Tree::Create(const AABB& limits)
{
	Clear();
	root->nodeArea = limits;
}

void Tree::Clear()
{
}

void Tree::Insert(GameObject * newItem)
{
}

void Tree::Remove(GameObject * itemToRemove)
{
}

void Tree::Intersect(std::vector<const GameObject*>& group, const AABB & area)
{
}

Tree::Node::Node()
{
}

Tree::Node::Node(AABB area, const Tree * tree)
{
}

void Tree::Node::Draw()
{
}

void Tree::Node::Split()
{
	AABB newNodesAABB[4];
	float3 minP, maxP;

	//Top-right
	maxP = { nodeArea.MaxX(), nodeArea.MaxY(), nodeArea.MaxZ() };
	minP = { (nodeArea.MaxX() + nodeArea.MinX()) / 2 , nodeArea.MinY(), (nodeArea.MaxZ() + nodeArea.MinZ()) / 2 };
	newNodesAABB[0].minPoint = minP;
	newNodesAABB[0].maxPoint = maxP;

}

void Tree::Node::Clear()
{
	// Clear all children
	if (!children.empty())
	{
		for (int i = 0; i < children.size(); i++)
		{
			children[i]->Clear();
		}
	}

	// Clear gameobject list
	if (!containedGameobj.empty())
		containedGameobj.clear();
}
