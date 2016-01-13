#include "Terminal.h"
#include "CommandStream.h"
#include "OSUtils.h"

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
		std::cout << "  help                  // Shows all terminal commands available" << std::endl;
		std::cout << "  save <filename>       // Save current map to location indicated by filename" << std::endl;
		std::cout << "  load <filename>       // Load the map at location indicated by filename" << std::endl;
		std::cout << "  clear                 // Clear the map of characters and tiles" << std::endl;
		std::cout << "  view maps             // List all map files in system" << std::endl;
		std::cout << "  view settings         // List all settings files in system" << std::endl;
		std::cout << "  delete map <filename> // Delete the map named filename(include extension)" << std::endl;
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

	void ExecuteViewMaps()
	{
		std::vector<std::string> maps;
		OSGetFilesInDir("Maps", maps);
		for (auto map : maps)
		{
			// Don't include current and backwards dirs
			if (map == "." || map == "..")
				continue;
			std::cout << map << std::endl;
		}
	}

	void ExecuteViewSettings()
	{
		std::vector<std::string> maps;
		OSGetFilesInDir("Settings", maps);
		for (auto map : maps)
		{
			// Don't include current and backwards dirs
			if (map == "." || map == "..")
				continue;
			std::cout << map << std::endl;
		}
	}

	void ExecuteDeleteMap(const std::vector<std::string>& params)
	{
		if (params.size() != 3)
			std::cout << "Usage: delete map <filename>" << std::endl;
		OSDeleteFile("Maps", params[2]);
	}

	void ExecuteDefault()
	{
		std::cout << "Command not recognized, type help for options" << std::endl;
	}

	void Run()
	{
		std::cout << "INTERACTIVE CONSOLE" << std::endl;
		std::cout << "-------------------" << std::endl << std::endl;
		while (!mKill) {
			std::cout << std::endl;
			std::cout << "> ";
			std::string value;
			std::getline(std::cin, value);
			// If nothing entered in console show usage details and continue
			if (value.size() == 0)
			{
				ExecuteDefault();
				continue;
			}
			// Split string on space
			std::vector<std::string> tokens{ 
				std::istream_iterator<std::string>{std::istringstream(value)}, 
				std::istream_iterator<std::string>{std::istringstream(" ")} };
			
			if (tokens[0] == "help") ExecuteHelp();
			else if (tokens[0] == "save") ExecuteSave(tokens);
			else if (tokens[0] == "load") ExecuteLoad(tokens);
			else if (tokens[0] == "clear") ExecuteClear();
			else if (tokens[0] == "view")
			{
				if (tokens.size() < 2)
				{
					std::cout << "Usage: view map OR view settings" << std::endl;
					continue;
				}
				if (tokens[1] == "settings") ExecuteViewSettings();
				else if (tokens[1] == "maps") ExecuteViewMaps();
				else std::cout << "Usage: view map OR view settings" << std::endl;
			}
			else if (tokens[0] == "delete")
			{
				if (tokens.size() < 2)
				{
					std::cout << "Usage: delete map <filename>" << std::endl;
					continue;
				}
				if (tokens[1] == "map") ExecuteDeleteMap(tokens);
				else std::cout << "Usage: delete map <filename>" << std::endl;
			}
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