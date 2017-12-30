#include <iostream>

int main()
{
	std::string s1("../somepath/somemorepath/somefile.ext");
	std::string s2("..\\somepath\\somemorepath\\somefile.ext");
	std::cout << s1.substr(0, s1.find_last_of("\\/")) << std::endl;
	std::cout << s2.substr(0, s2.find_last_of("\\/")) << std::endl;
}