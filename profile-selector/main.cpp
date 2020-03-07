//arg1 : location
//arg3 : profile name to switch to
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <utility>
#include <filesystem>
#include <cstring>

using namespace std;
 const string KEY_WORD_BACKUPS = "backups=",
        KEY_WORD_START_COPY_TASKS = "copy",
        KEY_WORD_START_COPY_BACK_TASKS = "copy-back",
        CFG_FILE_NAME = "selected",
        TAKS_FILE_NAME = "tasks",
        ROOT_DIR_NAME = "root/";

string* getCurrentProfile(string& cfgPath);
void copyFromTo(const string& s, const string& d);

int main(int argc, char* args[])
{
	bool init_mode = false;
	int backups = 0;

    if(argc < 3 || argc > 4)
    {
        cout << "need exactly 2 args, path and profile name" << endl;
        return -1;
    }
    if(argc == 4 && strcmp(args[3], "init") == 0)
    {
     	cout << "initializing profile " << args[2] << endl;
    	init_mode = true;
    }
    else{
    	int backups = argc == 4 ? atoi(args[3]) : 0;
    }

    list<string*> copyTasks;
    list<string*> copyBackTasks;

    string targetProfile = args[2];
    string cfgPath = args[1] + CFG_FILE_NAME;
    string dataLocation = args[1] + ROOT_DIR_NAME;
    string taskCfgPath = args[1] + TAKS_FILE_NAME;
    
    if(backups > 4) backups = 4;

    string* currentProfile = getCurrentProfile(cfgPath);
    if(currentProfile == nullptr)
    {
    	cout << "broken selected file" << endl;
    	return -1;
    }


    string* lineptr = new string;

    ifstream inStream(taskCfgPath);
    if(inStream.is_open())
    {
        int mode = -1; // 0 is copy, 1 is copy-back
        while(getline(inStream, *lineptr))
        {
            if(lineptr->compare(KEY_WORD_START_COPY_TASKS) == 0)
                mode = 0;
            else if(lineptr->compare(KEY_WORD_START_COPY_BACK_TASKS) == 0)
                mode = 1;
            else if(mode == 0){
            
           		string* sp = new string(*lineptr);
                copyTasks.push_back(sp);
                }
            else if(mode == 1){
            	string* sp = new string(*lineptr);
                copyBackTasks.push_back(sp);
                }
            else
            {
                cout << "task cfg has to start with type of task" << endl;
                return -1;
            }
        }
    }
    else
    {
        cout << "could not open: " << cfgPath << endl;
        return -1;
    }
    
    

	cout << endl << "start saving date" << endl << endl;
    if(currentProfile->length() > 0 && !init_mode)
    {
    
        for(auto it = copyBackTasks.begin(); it != copyBackTasks.end(); ++it)
        {
        	string targetPath = dataLocation + *currentProfile + **it;
        	for(int i = backups; i > 1; i--)
        	{
        		copyFromTo(targetPath + ".back" + to_string(i-1), targetPath + ".back" + to_string(i));
        	}
        	if(backups > 0)
        		copyFromTo(targetPath, targetPath + ".back1");
        	
            copyFromTo(**it, targetPath);
        }
    }
    
    
    cout << endl << "start copy date" << endl << endl;
    
    for(auto it = copyBackTasks.begin(); it != copyBackTasks.end(); ++it)
    {
        string sourcePath = dataLocation + targetProfile + **it;
        if(init_mode)
       		copyFromTo(**it, sourcePath);
       		else
       		copyFromTo(sourcePath, **it);
       		
       	delete *it;
    }
    
    cout << endl << "start copy 2" << endl << endl;
    
     for(auto it = copyTasks.begin(); it != copyTasks.end(); ++it)
    {
        string sourcePath = dataLocation + targetProfile + **it;
        if(init_mode)
       		copyFromTo(**it, sourcePath);
       		else
       		copyFromTo(sourcePath, **it);
        
        delete *it;
    }


    delete lineptr;
    
    ofstream out(cfgPath);
    out << targetProfile;
    out.close();
    
    return 0;
}

void copyFromTo(const string& s, const string& d)
{
    std::ifstream  src(s, std::ios::binary);
    std::ofstream  dst(d,   std::ios::binary);
    dst << src.rdbuf();

    cout << "copied from: " << s << " to " << d << endl;
}


string* getCurrentProfile(string &cfgPath)
{
    string* lineptr = new string;
    ifstream inStream(cfgPath);

	cout << "opening: " << cfgPath << endl;

    if(inStream.is_open())
    {
        
        if(getline(inStream, *lineptr))
        {
            cout << "current is: " << *lineptr << endl;
            
            return lineptr;
        }
		delete lineptr;
		return nullptr;
    }
    else
    {
        cout << "could not open: " << cfgPath << endl;
         delete lineptr;
        return nullptr;
    }
    delete lineptr;
    return nullptr;
}
