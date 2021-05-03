#pragma once
#include <iostream>
#include <vector>
class TreeNode
{
private:
	std::shared_ptr<TreeNode> parent;
	std::shared_ptr<TreeNode> firstChild;
	std::shared_ptr<TreeNode> lastChild;
	std::shared_ptr<TreeNode> nextSibling;
	int wins;
protected:
	int simulations;
public:
	int depth;
	std::pair<int, int> move;
	TreeNode();
	TreeNode(std::pair<int, int>, std::shared_ptr<TreeNode> parent);
	void appendChild(std::shared_ptr<TreeNode> child);
	std::vector<std::shared_ptr<TreeNode>> GetChildren();
	void update(bool result);
	double calculateUCT();
	double calculateUCTOpponent();
	bool hasChildren();
	std::shared_ptr<TreeNode> getParent();
	int getSimulations();
	void showInfo();
	void showMove();
};

