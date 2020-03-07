#include <iostream>
#include <string>
#include <fstream>
#include <list>

int main(int argc, char* args[])
{
	if(argc < 6)
	{
		std::cout << "not enough args!" << std::endl;
		return -1;
	}
	
	std::string firstLine = "#Name;Title;Emulator;CloneOf;Year;Manufacturer;Category;Players;Rotation;Control;Status;DisplayCount;DisplayType;AltRomname;AltTitle;Extra;Buttons";
	
	std::string listPath = args[1];
	int numberOfEntries = std::stoi(args[2]);
	std::string name = args[3];
	std::string title = args[4];
	std::string emulator = args[5];
	//std::cout << numberOfEntries << std::endl;
	//std::cout << name << std::endl;
	//std::cout << title << std::endl;
	//std::cout << emulator << std::endl;
	std::string newEntry = name + ";" + title + ";" + emulator + ";" + ";;;;;;;;;;;;;;";
	
	std::list<std::string*> inFileLines;
	std::ifstream inStream(listPath);
	
	std::string* lineptr = new std::string;
	
	if(inStream.is_open())
	{
		int lines = 0;
		while(std::getline(inStream, *lineptr))
		{
			lines++;
			if(lines == 1)
				continue;
			if(lines > numberOfEntries)
				break;
			if(lineptr->compare(newEntry) == 0)
			{
				lines--;
				delete lineptr;
			}
			else
			{
				inFileLines.push_back(lineptr);
			}
			
			lineptr = new std::string;
		}
		inStream.close();
		delete lineptr;
		
		std::ofstream outStream(listPath);
		
		if(outStream.is_open())
		{
			outStream << firstLine << std::endl;
			outStream << newEntry << std::endl;
			int i = 0;
    		for (std::list<std::string*>::iterator it = inFileLines.begin(); it != inFileLines.end(); ++it)
    		{
    			//std::cerr << "writing: " << **it << std::endl;
    			outStream << **it << std::endl;
    			delete *it;
    			
    			i++;
    		}
			outStream.close();
			std::cout << "[LastPlayedCreator] generated rom list" << std::endl;
		}
		else
		{
			std::cerr << "could not open out file: " << listPath << std::endl;
		}
	}
	else
	{
		std::cerr << "could not open file: " << listPath << std::endl;
	}
	
}
