#include "interpreter.hpp"
#include <fstream>

StandardFunctions StandardFunctions::instance;

XhaustValue func_Println(std::vector<XhaustValue> args)
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

    return XhaustValue().fromString(out);

}

XhaustValue func_Readln(std::vector<XhaustValue> args)
{
    if (args.size() > 0)
    {
        std::cout << args[0].toString();
    }
    std::string res;
    std::getline(std::cin, res);
    return XhaustValue::fromString(res);
}

XhaustValue func_ToNumber(std::vector<XhaustValue> args)
{
    if (args.size() > 0)
    {
        if (args[0].getType() == XhaustValueTypes::string)
        {
            std::string val = args[0].toString();
            return XhaustValue::fromNumber(std::stod(val));
        }
        else
        {
            return XhaustValue::fromNumber(args[0].getNumberValue());
        }
    }
    return XhaustValue::fromNull();
}

XhaustValue func_ToString(std::vector<XhaustValue> args)
{
    if (args.size() > 0)
    {
        return XhaustValue::fromString(args[0].toString());
    }
    return XhaustValue::fromNull();
}

XhaustValue func_Type(std::vector<XhaustValue> args)
{
    if (args.size() > 0)
    {
        XhaustValueTypes type = args[0].getType();
        if (type == XhaustValueTypes::string)
        {
            return XhaustValue::fromString("string");
        }
        else if (type == XhaustValueTypes::boolean)
        {
            return XhaustValue::fromString("boolean");
        }
        else if (type == XhaustValueTypes::number)
        {
            return XhaustValue::fromString("number");
        }
        else if (type == XhaustValueTypes::nulltype)
        {
            return XhaustValue::fromString("null");
        }
        else if (type == XhaustValueTypes::object)
        {
            return XhaustValue::fromString("object");
        }
    }
    return XhaustValue::fromNull();
}

XhaustValue func_DoString(std::vector<XhaustValue> args)
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
            return XhaustValue::fromNull();
        }
    }
    return XhaustValue::fromNull();
}

XhaustValue func_LoadLines(std::vector<XhaustValue> args)
{
    if (args.size() < 1)
        return XhaustValue::fromNull();
    std::fstream file(args[0].toString());
    std::string line;
    std::vector<XhaustValue> *result = new std::vector<XhaustValue>();
    while (file && std::getline(file, line))
    {
        result->push_back(XhaustValue::fromString(line));
    }
    return XhaustValue::fromObject(result);
}

XhaustValue func_ListCreate(std::vector<XhaustValue> args)
{
    return XhaustValue::fromObject(reinterpret_cast<void*>(new std::vector<XhaustValue>()));
}

XhaustValue func_ListFree(std::vector<XhaustValue> args)
{
    if (args.size() > 0 && args[0].getType() == XhaustValueTypes::object)
    {
        std::vector<XhaustValue> *list = reinterpret_cast<std::vector<XhaustValue>*>(args[0].getObjectValue());
        delete list;
        return XhaustValue::fromBoolean(true);
    }
    return XhaustValue::fromBoolean(false);
}

XhaustValue func_ListAppend(std::vector<XhaustValue> args)
{
    if (args.size() > 0 && args[0].getType() == XhaustValueTypes::object)
    {
        std::vector<XhaustValue> *list = reinterpret_cast<std::vector<XhaustValue>*>(args[0].getObjectValue());
        for (int i = 1; i < args.size(); i++)
        {
            list->push_back(args[i]);
        }
    }
    return XhaustValue::fromNull();
}

XhaustValue func_ListGet(std::vector<XhaustValue> args)
{
    if (args.size() > 1 && args[0].getType() == XhaustValueTypes::object)
    {
        std::vector<XhaustValue> *list = reinterpret_cast<std::vector<XhaustValue>*>(args[0].getObjectValue());
        return list->at((int)args[1].getNumberValue());
    }
    return XhaustValue::fromNull();
}

XhaustValue func_ListSize(std::vector<XhaustValue> args)
{
    if (args.size() > 0 && args[0].getType() == XhaustValueTypes::object)
    {
        std::vector<XhaustValue> *list = reinterpret_cast<std::vector<XhaustValue>*>(args[0].getObjectValue());
        return XhaustValue::fromNumber(list->size());
    }
    return XhaustValue::fromNull();
}

XhaustValue func_ListRemove(std::vector<XhaustValue> args)
{
    if (args.size() > 1 && args[0].getType() == XhaustValueTypes::object)
    {
        std::vector<XhaustValue> *list = reinterpret_cast<std::vector<XhaustValue>*>(args[0].getObjectValue());
        int index = (int)args[1].getNumberValue();
        XhaustValue result = list->at(index);
        list->erase(list->begin()+(index-1));
    }
    return XhaustValue::fromNull();
}

XhaustValue func_ListFind(std::vector<XhaustValue> args)
{
    if (args.size() > 1 && args[0].getType() == XhaustValueTypes::object)
    {
        std::vector<XhaustValue> *list = reinterpret_cast<std::vector<XhaustValue>*>(args[0].getObjectValue());
        for (int i = 0; i < list->size(); i++)
        {
            if (list->at(i) == args[1])
            {
                return XhaustValue::fromBoolean(true);
            }
        }
        return XhaustValue::fromBoolean(false);
    }
    return XhaustValue::fromNull();
}

StandardFunctions::StandardFunctions()
{
    callbacks["Println"] = func_Println;
    callbacks["Readln"] = func_Readln;
    callbacks["ToNumber"] = func_ToNumber;
    callbacks["ToString"] = func_ToString;
    callbacks["Type"] = func_Type;
    callbacks["DoString"] = func_DoString;
    callbacks["LoadLines"] = func_LoadLines;
    callbacks["ListCreate"] = func_ListCreate;
    callbacks["ListFree"] = func_ListFree;
    callbacks["ListAppend"] = func_ListAppend;
    callbacks["ListRemove"] = func_ListRemove;
    callbacks["ListGet"] = func_ListGet;
    callbacks["ListSize"] = func_ListSize;
    callbacks["ListFind"] = func_ListFind;
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
