#include "Tree.h"
#include "GameObject.h"

#include "SDL/include/SDL_assert.h"
#include "SDL/include/SDL_opengl.h"

Tree::Tree(AABB aabb)
{
	root = new Node(aabb, this, nullptr);
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
	root->Clear();
}

void Tree::Insert(GameObject * newItem)
{
	if (root->nodeArea.Contains(newItem->GetAABB()))
	{
		containedGameobj.push_back(newItem);
		root->containedGameobj.push_back(newItem);
	}
}

void Tree::Remove(GameObject * itemToRemove)
{
}

void Tree::Intersect(std::vector<const GameObject*>& group, const AABB & area)
{
	root->Intersect(group, area);
}

void Tree::Node::Draw()
{
	// Debug draw in direct mode
	glLineWidth(1.5f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 0.0f);

	glVertex3f(nodeArea.MaxX(), nodeArea.MaxY(), nodeArea.MaxZ());
	glVertex3f(nodeArea.MinX(), nodeArea.MaxY(), nodeArea.MaxZ());

	glVertex3f(nodeArea.MaxX(), nodeArea.MaxY(), nodeArea.MaxZ());
	glVertex3f(nodeArea.MaxX(), nodeArea.MinY(), nodeArea.MaxZ());

	glVertex3f(nodeArea.MaxX(), nodeArea.MaxY(), nodeArea.MaxZ());
	glVertex3f(nodeArea.MaxX(), nodeArea.MaxY(), nodeArea.MinZ());

	glVertex3f(nodeArea.MinX(), nodeArea.MinY(), nodeArea.MinZ());
	glVertex3f(nodeArea.MaxX(), nodeArea.MinY(), nodeArea.MinZ());

	glVertex3f(nodeArea.MinX(), nodeArea.MinY(), nodeArea.MinZ());
	glVertex3f(nodeArea.MinX(), nodeArea.MaxY(), nodeArea.MinZ());

	glVertex3f(nodeArea.MinX(), nodeArea.MinY(), nodeArea.MinZ());
	glVertex3f(nodeArea.MinX(), nodeArea.MinY(), nodeArea.MaxZ());

	glVertex3f(nodeArea.MinX(), nodeArea.MaxY(), nodeArea.MaxZ());
	glVertex3f(nodeArea.MinX(), nodeArea.MinY(), nodeArea.MaxZ());

	glVertex3f(nodeArea.MinX(), nodeArea.MaxY(), nodeArea.MaxZ());
	glVertex3f(nodeArea.MinX(), nodeArea.MaxY(), nodeArea.MinZ());

	glVertex3f(nodeArea.MaxX(), nodeArea.MinY(), nodeArea.MinZ());
	glVertex3f(nodeArea.MaxX(), nodeArea.MaxY(), nodeArea.MinZ());

	glVertex3f(nodeArea.MaxX(), nodeArea.MaxY(), nodeArea.MinZ());
	glVertex3f(nodeArea.MinX(), nodeArea.MaxY(), nodeArea.MinZ());

	glVertex3f(nodeArea.MaxX(), nodeArea.MinY(), nodeArea.MinZ());
	glVertex3f(nodeArea.MaxX(), nodeArea.MinY(), nodeArea.MaxZ());

	glVertex3f(nodeArea.MaxX(), nodeArea.MinY(), nodeArea.MaxZ());
	glVertex3f(nodeArea.MinX(), nodeArea.MinY(), nodeArea.MaxZ());

	glColor3f(1, 1, 1);
	glEnd();

	// Draw children
	if (!children.empty())
		for (int i = 0; i < children.size(); ++i)
			children[i]->Draw();
}

void Tree::Node::Split4()
{
	AABB newNodesAABB[4];
	float3 minP, maxP;

	//Top-right
	maxP = { nodeArea.MaxX(), nodeArea.MaxY(), nodeArea.MaxZ() };
	minP = { (nodeArea.MaxX() + nodeArea.MinX()) / 2 , nodeArea.MinY(), (nodeArea.MaxZ() + nodeArea.MinZ()) / 2 };
	newNodesAABB[0].minPoint = minP;
	newNodesAABB[0].maxPoint = maxP;

	//Top-left
	maxP = { (nodeArea.MaxX() + nodeArea.MinX()) / 2, nodeArea.MaxY(), nodeArea.MaxZ() };
	minP = { nodeArea.MinX(), nodeArea.MinY(), (nodeArea.MaxZ() + nodeArea.MinZ()) / 2 };
	newNodesAABB[1].minPoint = minP;
	newNodesAABB[1].maxPoint = maxP;

	//Bottom-right
	maxP = { nodeArea.MaxX(), nodeArea.MaxY(),(nodeArea.MaxZ() + nodeArea.MinZ()) / 2 };
	minP = { (nodeArea.MaxX() + nodeArea.MinX()) / 2, nodeArea.MinY(), nodeArea.MinZ() };
	newNodesAABB[2].minPoint = minP;
	newNodesAABB[2].maxPoint = maxP;

	//Bottom-left
	maxP = { (nodeArea.MaxX() + nodeArea.MinX()) / 2, nodeArea.MaxY(),(nodeArea.MaxZ() + nodeArea.MinZ()) / 2 };
	minP = { nodeArea.MinX(), nodeArea.MinY(), nodeArea.MinZ() };
	newNodesAABB[3].minPoint = minP;
	newNodesAABB[3].maxPoint = maxP;

	for (int i = 0; i < 4; i++)
	{
		children.push_back(new Node(newNodesAABB[i], tree, this));
	}
}

void Tree::Node::CheckAndSplit()
{
	if (containedGameobj.size() > tree->bucket)
	{
		// First, split the current node in 4
		Split4();

		// Iterate the new node children and add each gameobject from this node to the correspondent child/children
		for (int i = 0; i < containedGameobj.size(); i++)
		{
			for (int j = 0; j < children.size(); j++)
			{
				if (children[j]->nodeArea.Intersects(containedGameobj[i]->GetAABB()))
				{
					children[j]->containedGameobj.push_back(containedGameobj[i]);
					break;
				}
			}
			
		}
		for (int i = 0; i < children.size(); i++)
		{
			children[i]->CheckAndSplit();
		}
		
		containedGameobj.clear();
	}
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

void Tree::Node::Intersect(std::vector<const GameObject*>& group, const AABB & area)
{
	//if (nodeArea.Intersects(area)) {
	//	for (int i = 0; i < containedGameobj.size(); i++)
	//		group.push_back(containedGameobj[i]);
	//}
}
