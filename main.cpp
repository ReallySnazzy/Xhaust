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
    try
    {
        outputTree("5*2+9*7");
        outputTree("do testCall()");
        outputTree("(do testCall(14+2,99,bottles,beer))*9");
        outputTree("lol=99");
        outputTree("lol = 5 if (lol == 5) {lol=10 xd=20}");
    }
    catch (SyntaxException *ex)
    {
        std::cout << ex->what() << std::endl;
    }
}
