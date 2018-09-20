#include "Node.h"

inline void Node::AddChild(Node * Node)
{
	Node->Parent = this;
	Children.push_back(Node);

}

inline int Node::GetChildCount()
{
	return Children.size();
}
