#include <iostream>
#include "treegen.hpp"

int main()
{
    TreeGenerator* tg = TreeGenerator::fromString("5*2+9*7");
    std::vector<TreeNode*> nodes = tg->buildTree();
    for (TreeNode* tn : nodes)
        tn->print(0);
    for (TreeNode* tn : nodes)
        delete tn;
}
