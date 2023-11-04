#include "firstApp.hpp"
#include<cstdlib>
#include<iostream>
#include<stdexcept>
int main()
{
	firstApp app{};
	try 
	{
		app.run();
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}