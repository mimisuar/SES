#include "System.hpp"

int main()
{
	SES::System* sys = new SES::System;

	sys->LoadRom("test");

	sys->Run();

	delete sys;

	return EXIT_SUCCESS;
}