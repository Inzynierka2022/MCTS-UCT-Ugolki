#include "TreeNode.h"
#include <cmath>

TreeNode::TreeNode()
{
	this->parent = nullptr;
	this->firstChild = nullptr;
	this->nextSibling = nullptr;
	this->lastChild = nullptr;
	this->move = std::pair<int, int>(0, 0);
	this->wins = 0;
	this->simulations = 0;
	this->depth = 0;
}

TreeNode::TreeNode(std::pair<int, int> move, std::shared_ptr<TreeNode> parent)
{
	this->move = move;
	this->parent = parent;
	this->nextSibling = nullptr;
	this->firstChild = nullptr;
	this->lastChild = nullptr;
	this->wins = 0;
	this->simulations = 0;
	this->depth = parent->depth + 1;
}

void TreeNode::appendChild(std::shared_ptr<TreeNode> child)
{
	if (firstChild == nullptr)
		firstChild = child;
	else
		this->lastChild->nextSibling = child;
	this->lastChild = child;
}

std::vector<std::shared_ptr<TreeNode>> TreeNode::GetChildren()
{
	std::vector<std::shared_ptr<TreeNode>> children;
	if (firstChild == nullptr) return children;
	std::shared_ptr<TreeNode> temp = firstChild;
	do
	{
		children.push_back(temp);
		temp = temp->nextSibling;
	} while (temp != nullptr);
	return children;
}

void TreeNode::update(int result, int number_of_simulations)
{
	this->simulations+= number_of_simulations;
	this->wins += result;
	if(this->parent != nullptr)
		this->parent->update(result, number_of_simulations);
}


double TreeNode::calculateUCT()
{
	if (simulations == 0) return std::numeric_limits<double>::max();
	return ((double)wins / simulations) + (sqrt(2) * sqrt(log(parent->simulations) / simulations));
}

double TreeNode::calculateUCTOpponent()
{
	if (simulations == 0) return std::numeric_limits<double>::max();
	return (((double)simulations - wins) / simulations) + (sqrt(2) * sqrt(log(parent->simulations) / simulations));
}

bool TreeNode::hasChildren()
{
	if (firstChild != nullptr) return true;
	return false;
}

std::shared_ptr<TreeNode> TreeNode::getParent()
{
	return this->parent;
}

int TreeNode::getSimulations()
{
	return simulations;
}

void TreeNode::showInfo()
{
	std::printf("%d-%d", move.first, move.second);
	std::printf(" (%d/%d)\n", wins, simulations);
}