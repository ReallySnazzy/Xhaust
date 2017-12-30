#include "interpreter.hpp"
#include <fstream>

StandardFunctions StandardFunctions::instance;

XhaustValue Println(std::vector<XhaustValue> args)
{
    int count = args.size();
    std::string out = "";
    for (int i = 0; i < count; i++)
    {
        if (i != 0)
            out += ", ";
        out += args[i].toString();
    }
    out += "\n";
    std::cout << out;

    return XhaustValue::String(out);
}

XhaustValue Readln(std::vector<XhaustValue> args)
{
    if (args.size() > 0)
    {
        std::cout << args[0].toString();
    }
    std::string res;
    std::getline(std::cin, res);
    return XhaustValue::String(res);
}

XhaustValue ToNumber(std::vector<XhaustValue> args)
{
    if (args.size() > 0)
    {
        if (args[0].getType() == XhaustValueTypes::string)
        {
            std::string val = args[0].toString();
            return XhaustValue::Number(std::stod(val));
        }
        else
        {
            return XhaustValue::Number(args[0].getNumberValue());
        }
    }
    return XhaustValue::Null();
}

XhaustValue ToString(std::vector<XhaustValue> args)
{
    if (args.size() > 0)
    {
        return XhaustValue::String(args[0].toString());
    }
    return XhaustValue::Null();
}

XhaustValue Type(std::vector<XhaustValue> args)
{
    if (args.size() > 0)
    {
        XhaustValueTypes type = args[0].getType();
        if (type == XhaustValueTypes::string)
        {
            return XhaustValue::String("string");
        }
        else if (type == XhaustValueTypes::boolean)
        {
            return XhaustValue::String("boolean");
        }
        else if (type == XhaustValueTypes::number)
        {
            return XhaustValue::String("number");
        }
        else if (type == XhaustValueTypes::nulltype)
        {
            return XhaustValue::String("null");
        }
        else if (type == XhaustValueTypes::object)
        {
            return XhaustValue::String("object");
        }
    }
    return XhaustValue::Null();
}

//@bookmark{samybencherif}
XhaustValue Eval(std::vector<XhaustValue> args)
{
    if (args.size() > 0)
    {
        Interpreter *i = Interpreter::fromSource(args[0].toString());
        try
        {
            XhaustValue result = i->start();
            delete i;
            return result;
        }
        catch (std::exception *ex)
        {
            return XhaustValue::Null();
        }
    }
    return XhaustValue::Null();
}

XhaustValue LoadLines(std::vector<XhaustValue> args)
{
    if (args.size() < 1)
        return XhaustValue::Null();
    std::fstream file(args[0].toString());
    std::string line;
    std::vector<XhaustValue> *result = new std::vector<XhaustValue>();
    while (file && std::getline(file, line))
    {
        result->push_back(XhaustValue::String(line));
    }
    return XhaustValue::Object(result);
}

XhaustValue ListCreate(std::vector<XhaustValue> args)
{
    return XhaustValue::Object(reinterpret_cast<void *>(new std::vector<XhaustValue>()));
}

XhaustValue ListFree(std::vector<XhaustValue> args)
{
    if (args.size() > 0 && args[0].getType() == XhaustValueTypes::object)
    {
        std::vector<XhaustValue> *list = reinterpret_cast<std::vector<XhaustValue> *>(args[0].getObjectValue());
        delete list;
        return XhaustValue::Boolean(true);
    }
    return XhaustValue::Boolean(false);
}

XhaustValue ListAppend(std::vector<XhaustValue> args)
{
    if (args.size() > 0 && args[0].getType() == XhaustValueTypes::object)
    {
        std::vector<XhaustValue> *list = reinterpret_cast<std::vector<XhaustValue> *>(args[0].getObjectValue());
        for (int i = 1; i < args.size(); i++)
        {
            list->push_back(args[i]);
        }
    }
    return XhaustValue::Null();
}

XhaustValue ListGet(std::vector<XhaustValue> args)
{
    if (args.size() > 1 && args[0].getType() == XhaustValueTypes::object)
    {
        std::vector<XhaustValue> *list = reinterpret_cast<std::vector<XhaustValue> *>(args[0].getObjectValue());
        return list->at((int)args[1].getNumberValue());
    }
    return XhaustValue::Null();
}

XhaustValue ListSize(std::vector<XhaustValue> args)
{
    if (args.size() > 0 && args[0].getType() == XhaustValueTypes::object)
    {
        std::vector<XhaustValue> *list = reinterpret_cast<std::vector<XhaustValue> *>(args[0].getObjectValue());
        return XhaustValue::Number(list->size());
    }
    return XhaustValue::Null();
}

XhaustValue ListRemove(std::vector<XhaustValue> args)
{
    if (args.size() > 1 && args[0].getType() == XhaustValueTypes::object)
    {
        std::vector<XhaustValue> *list = reinterpret_cast<std::vector<XhaustValue> *>(args[0].getObjectValue());
        int index = (int)args[1].getNumberValue();
        XhaustValue result = list->at(index);
        list->erase(list->begin() + (index - 1));
    }
    return XhaustValue::Null();
}

XhaustValue ListFind(std::vector<XhaustValue> args)
{
    if (args.size() > 1 && args[0].getType() == XhaustValueTypes::object)
    {
        std::vector<XhaustValue> *list = reinterpret_cast<std::vector<XhaustValue> *>(args[0].getObjectValue());
        for (int i = 0; i < list->size(); i++)
        {
            if (list->at(i) == args[1])
            {
                return XhaustValue::Boolean(true);
            }
        }
        return XhaustValue::Boolean(false);
    }
    return XhaustValue::Null();
}

XhaustValue StrLen(std::vector<XhaustValue> args)
{
    if (args.size() < 1)
        return XhaustValue::Null();
    return XhaustValue::Number(args[0].toString().length());
}

XhaustValue StrGet(std::vector<XhaustValue> args)
{
    if (args.size() < 2)
        return XhaustValue::Null();
    return XhaustValue::String(std::string() + args[0].toString()[(int)(args[1].getNumberValue() + 0.5)]);
}

XhaustValue StrSub(std::vector<XhaustValue> args)
{
    if (args.size() < 3)
        return XhaustValue::Null();
    int start = (int)(args[1].getNumberValue() + 0.5);
    int finish = (int)(args[2].getNumberValue() + 0.5);
    int len = (finish - start);
    return XhaustValue::String(args[0].toString().substr(start, len));
}

StandardFunctions::StandardFunctions()
{
    //@bookmark{samybencherif}[Check back to see what the deal with this callbacks dict is]
    callbacks["println"] = Println;
    callbacks["readln"] = Readln;
    callbacks["toNumber"] = ToNumber;
    callbacks["toString"] = ToString;
    callbacks["type"] = Type;
    callbacks["eval"] = Eval;
    callbacks["loadLines"] = LoadLines;
    callbacks["listCreate"] = ListCreate;
    callbacks["listFree"] = ListFree;
    callbacks["listAppend"] = ListAppend;
    callbacks["listRemove"] = ListRemove;
    callbacks["listGet"] = ListGet;
    callbacks["listSize"] = ListSize;
    callbacks["listFind"] = ListFind;
    callbacks["strLen"] = StrLen;
    callbacks["strGet"] = StrGet;
    callbacks["strSub"] = StrSub;
}

StandardFunctions &StandardFunctions::getInstance()
{
    return instance;
}

bool StandardFunctions::hasFunction(std::string name)
{
    return callbacks.count(name) > 0;
}

XhaustValue StandardFunctions::functionCall(std::string name, std::vector<XhaustValue> args)
{
    return callbacks[name](args);
}
