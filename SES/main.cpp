#include "System.hpp"

int main(int argc, char** argv)
{
	/*
	if (argc < 2)
	{
		std::cout << "SES <fname>" << std::endl;
		return EXIT_FAILURE;
	}
	*/

	SES::System* sys = new SES::System;

	if (!sys->LoadRom("graphics.ses"))
	{
		delete sys;
		return EXIT_FAILURE;
	}

	sys->Run();

	delete sys;

	return EXIT_SUCCESS;
}