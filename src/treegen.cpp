#include "treegen.hpp"
#include <stdio.h>

//TODO further clean up this file.
/*
I don't want any functions that do more than they say they do.
eg parseFactor should not also parseGroup.

also like every file in this project, error handling can be improved.

Otherwise the thing is looking like it's running nice
*/

TreeGenerator *TreeGenerator::fromString(std::string source)
{
	return new TreeGenerator(tokenize(source));
}

TreeGenerator::TreeGenerator(std::vector<Token> tokens)
{
	this->tokens = tokens;
}

BlockNode *TreeGenerator::parseBlock()
{
	if (tokens.size() > marker && tokens[marker].type == TK_REGULATOR && tokens[marker].value == "{")
	{
		marker++;
	}
	else
	{
		throw new SyntaxException(-1, "{ expected.");
	}

	std::vector<TreeNode *> body;
	while (tokens.size() > marker && (tokens[marker].type != TK_REGULATOR || tokens[marker].value != "}"))
	{
		body.push_back(parseStatement());
	}
	if (tokens.size() > marker)
		++marker;
	else
		throw new SyntaxException(-1, "} expected.");
	return new BlockNode(body);
}

ExhaustNode *TreeGenerator::parseExhaust()
{
	if (tokens[marker].type == TK_KEYWORD && tokens[marker].value == "exhaust")
		++marker;
	else
		throw new SyntaxException(-1, "exhaust expected"); //never happens
	TreeNode *condition = parseExpression();
	bool usesVariable = (condition->type == TN_VALUE && !reinterpret_cast<ValueNode *>(condition)->isConstant);

	TreeNode *block = parseBlock();
	return new ExhaustNode(condition, block, usesVariable);
}

IfNode *TreeGenerator::parseIf()
{
	if (tokens[marker].type == TK_KEYWORD && tokens[marker].value == "if")
		++marker;
	else
		throw new SyntaxException(-1, "if expected");
	if (tokens.size() > marker && tokens[marker].type == TK_REGULATOR && tokens[marker].value == "(")
	{
		++marker;
	}
	else
		throw new SyntaxException(-1, "( expected after if");
	TreeNode *condition = parseExpression();
	if (tokens.size() > marker && tokens[marker].type == TK_REGULATOR && tokens[marker].value == ")")
	{

		++marker;
	}
	else
		throw new SyntaxException(-1, ") expected after if condition");

	TreeNode *block = parseBlock();
	return new IfNode(condition, block);
}

FunctionCallNode *TreeGenerator::parseFunctionCall()
{
	std::string identifier = tokens[marker++].value;
	if (tokens[marker++].value != "(")
	{
		throw new SyntaxException(-1, "Expected ( at the beginning of function arguments");
	}
	std::vector<TreeNode *> arguments;
	if (tokens[marker].value != ")")
	{
		do
		{
			arguments.push_back(parseExpression());
		} while (tokens[marker++].value == ",");
		--marker;
	}
	if (tokens[marker++].value != ")")
	{
		throw new SyntaxException(-1, "Expected ) at the end of function arguments");
	}
	return new FunctionCallNode(identifier, arguments);
}

GroupNode *TreeGenerator::parseGroup()
{
	++marker;
	GroupNode *res = new GroupNode(parseExpression());
	if (tokens[marker++].value != ")")
	{
		throw new SyntaxException(-1, ") expected to close expression");
	}
	return res;
}

TreeNode *TreeGenerator::parseFactor()
{
	Token token = tokens[marker];
	if (token.type != TK_IDENTIFIER && token.type != TK_NUMBER && token.type != TK_STRING)
	{
		if (token.type == TK_REGULATOR && token.value == "(")
			return parseGroup();
		else
			throw new SyntaxException(-1, "Expected value or identifier, got " + token.value);
	}
	marker++;
	return new ValueNode(token.value, token.type == TK_NUMBER || token.type == TK_STRING);
}

OperatorNode *TreeGenerator::parseBinaryOperation()
{
	if (runningLHS == nullptr)
		throw new SyntaxException(-1, "Binary operator missing left hand side.");

	TreeNode *lhs = runningLHS;
	std::string op = tokens[marker++].value;
	int oldMarker = marker;
	TreeNode *rhs = parseExpression();

	if (oldMarker == marker)
		throw new SyntaxException(-1, "Binary operator missing right hand side.");

	return new OperatorNode(op, lhs, rhs);
}

TreeNode *TreeGenerator::parseExpression()
{

	if (tokens.size() > marker)
	{
		switch (tokens[marker].type)
		{
		case TK_BINARYOPERATOR:
			runningLHS = parseBinaryOperation();
			break;
		case TK_IDENTIFIER:
			if (tokens.size() > marker + 1 && tokens[marker + 1].value == "(")
				runningLHS = parseFunctionCall();
			else
				runningLHS = parseFactor(); //variable
			break;
		case TK_NUMBER:
		case TK_STRING:
		case TK_REGULATOR: //should be broken up into parseGroup / parseLiteral / etc
			runningLHS = parseFactor();
			break;
		}
		if (tokens.size() > marker && tokens[marker].type == TK_BINARYOPERATOR) //hmmm need to check the reasoning + edge cases
			runningLHS = parseExpression();
		//else if (runningLHS != nullptr)
		return runningLHS;
	}

	throw new SyntaxException(-1, "Unexpected token: \"" + tokens[marker].value + "\"");
}

TreeNode *TreeGenerator::parseStatement()
{
	if (tokens.size() > marker && tokens[marker].type == TK_KEYWORD)
	{

		//nullify running lhs
		runningLHS = nullptr;

		if (tokens[marker].value == "exhaust")
		{
			return parseExhaust();
		}
		else if (tokens[marker].value == "if")
		{
			return parseIf();
		}
		else //todo make sure function call makes it to parseExp
		{
			throw new SyntaxException(-1, "Unimplemented keyword: " + tokens[marker].value);
		}
	}
	else
	{
		//set running lhs
		runningLHS = parseExpression();

		return runningLHS;
	}
}

std::vector<TreeNode *> TreeGenerator::buildTree()
{
	std::vector<TreeNode *> nodes;
	while (marker < tokens.size())
	{
		int oldMarker = marker;

		nodes.push_back(parseStatement());

		if (oldMarker == marker)
			throw new SyntaxException(-1, "Unparsable fragment."); //wtf
	}

	return nodes;
}