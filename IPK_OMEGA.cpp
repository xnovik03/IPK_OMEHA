#include <iostream>

int main(int argc, char* argv[])
{
    for (int i{}; i < argc; ++i)
    {
        std::cout << argv[i] << std::endl;
    }
}
bool hasArgument(int argc)
{
	if (argc<=0)
	{
		std::cout << "ERROR"; return false;
	}
	else { return true; }
}
bool validateArguments(int argc, char* argv[])
{
	if (argc<3)
	{
		std::cout << "ERROR"; return false;

	}

}