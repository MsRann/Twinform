#include "Terminal.h"
#include "CommandStream.h"

#include <iostream>
#include <vector>
#include <thread>
#include <string>
#include <iterator>
#include <sstream>

namespace
{
	bool mKill = false;

	void ExecuteHelp() 
	{
		std::cout << "Commands:" << std::endl;
		std::cout << "  help            // Shows all terminal commands available" << std::endl;
		std::cout << "  save <filename> // Save current map to location indicated by filename" << std::endl;
		std::cout << "  load <filename> // Load the map at location indicated by filename" << std::endl;
		std::cout << "  clear           // Clear the map of characters and tiles" << std::endl;
	}

	void ExecuteSave(const std::vector<std::string>& params)
	{
		if (params.size() != 2)
			std::cout << "Usage: save <filename>" << std::endl;

		CommandStream::Add(new SaveCommand(params[1]));
	}

	void ExecuteLoad(const std::vector<std::string>& params)
	{
		if (params.size() != 2)
			std::cout << "Usage: load <filename>" << std::endl;

		CommandStream::Add(new LoadCommand(params[1]));
	}

	void ExecuteClear()
	{
		CommandStream::Add(new ClearCommand());
	}

	void ExecuteDefault()
	{
		std::cout << "Command not recognized, type help for options" << std::endl;
	}

	void Run()
	{
		while (!mKill) {
			std::cout << std::endl;
			std::cout << "> ";
			std::string value;
			std::getline(std::cin, value);
			// Split string on space
			std::vector<std::string> tokens{ 
				std::istream_iterator<std::string>{std::istringstream(value)}, 
				std::istream_iterator<std::string>{std::istringstream(" ")} };
			
			if (tokens[0] == "help") ExecuteHelp();
			else if (tokens[0] == "save") ExecuteSave(tokens);
			else if (tokens[0] == "load") ExecuteLoad(tokens);
			else if (tokens[0] == "clear") ExecuteClear();
			else ExecuteDefault();
		}
	}

	std::thread mThread(&Run);
}

void Terminal::Kill()
{
	mKill = true;
	mThread.join();
}