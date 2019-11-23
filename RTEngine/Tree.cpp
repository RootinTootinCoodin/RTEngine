#include "Tree.h"
#include "GameObject.h"
#include "Component.h"
#include "ComponentMesh.h"

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
	if (newItem->GetComponent(MESH) != nullptr && newItem->active && newItem->is_static)
	{
		for (int i = 0; i < treeObjects.size(); i++)
			if (treeObjects[i] == newItem)
				return;

		if (root->Insert(newItem))
		{
			treeObjects.push_back(newItem);
		}
	}
	
}

void Tree::Remove(GameObject * itemToRemove)
{
}

void Tree::CollectIntersections(std::vector<const GameObject*>& collector, const AABB & area)
{
	root->CollectIntersections(collector, area);
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
	if (!nodeObjects.empty())
		nodeObjects.clear();
}

void Tree::Node::CollectIntersections(std::vector<const GameObject*>& collector, const AABB & area)
{
	if (area.Intersects(nodeArea))
	{
		for (int i = 0; i < nodeObjects.size(); i++)
			collector.push_back(nodeObjects[i]);

		if (!children.empty())
			for (int i = 0; i < children.size(); i++)
				children[i]->CollectIntersections(collector, area);
	}
}

bool Tree::Node::Insert(const GameObject * newItem)
{
	bool ret = false;

	if (nodeArea.Contains(newItem->GetAABB().CenterPoint()))

		// If the node has children
		if (!children.empty())
		{
			// Try to insert in one of the children
			for (int i = 0; i < children.size(); i++)
				if (children[i]->Insert(newItem))
				{
					ret = true;
					break;
				}

			if (!ret)
			{
				nodeObjects.push_back(newItem);
				ret = true;
			}
		}
		else
		{
			nodeObjects.push_back(newItem);
			ret = true;

			if (nodeObjects.size() > tree->bucket && tree->currentSubdivisions < 50)
			{
				Split4();
				tree->currentSubdivisions++;

				std::vector<const GameObject*> left;

				for (int i = 0; i < nodeObjects.size(); i++)
				{
					for (int j = 0; j < children.size(); j++)
					{
						if (children[j]->Insert(nodeObjects[i]))
						{
							break;
						}
					}
				}
			}
		}

	return ret;
}
