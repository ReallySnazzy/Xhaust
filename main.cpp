#include <iostream>
#include "treegen.hpp"

void outputTree(std::string source)
{
    TreeGenerator* tg = TreeGenerator::fromString(source);
    std::vector<TreeNode*> nodes = tg->buildTree();
    std::cout << source << std::endl;
    for (TreeNode* tn : nodes)
        tn->print(1);
    for (TreeNode* tn : nodes)
        delete tn;
    delete tg;
    std::cout << std::endl;
}

int main()
{
    outputTree("5*2+9*7");
    outputTree("do testCall()");
    outputTree("(do testCall(14+2,99,bottles,beer))*9");
}
