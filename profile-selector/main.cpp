//arg1 : location
//arg3 : profile name to switch to
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <utility>
#include <filesystem>
#include <cstring>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;
using namespace std;
 const string KEY_WORD_BACKUPS = "backups=",
        KEY_WORD_START_COPY_TASKS = "copy",
        KEY_WORD_START_COPY_BACK_TASKS = "copy-back",
        KEY_WORD_START_REPLACE_TASKS = "replace-text",
        CFG_FILE_NAME = "selected",
        TAKS_FILE_NAME = "tasks",
        ROOT_DIR_NAME = "root/",
        SEPERATION_CHAR = "|",
        KEY_WORD_REPLACE_WITH_TARGET_NAME = "[profile_name]";

string* getCurrentProfile(string& cfgPath);
void CopySoft(const string& s, const string& d);
void copyFromTo(const string& s, const string& d);
void replaceTextTask(const string& taskLine, const string& targetName);

// arg 1 : location
// arg 2 : target profile
// arg 3 : "init" or backup number
// arg 4 : video file
// arg 5 : minimal duration

int main(int argc, char* args[])
{
	auto startTime = chrono::steady_clock::now();
	bool init_mode = false;
	int backups = 0;
	bool video_mode = false;
	int minMillisVideoPlayback;

    if(argc != 3 && argc != 4 && argc != 6)
    {
        cout << "need exactly 2 args, path and profile name" << endl;
        return -1;
    }
    if(argc == 4 && strcmp(args[3], "init") == 0)
    {
     	cout << "initializing profile " << args[2] << endl;
    	init_mode = true;
    }
    else if(argc == 4){
    	backups = argc == 4 ? atoi(args[3]) : 0;
    }
    video_mode = (argc == 6);
    if(video_mode)
    {
    	string videoPath = "cvlc -f --loop --video-on-top --no-video-title " + string(args[4]) + " &";
    	system(videoPath.c_str());
    	minMillisVideoPlayback = atoi(args[5]);
    }

    list<string*> copyTasks;
    list<string*> copyBackTasks;
    list<string*> replaceTextTasks;

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
        int mode = -1; // 0 is copy, 1 is copy-back, 2 is replace text line
        while(getline(inStream, *lineptr))
        {
        	if(lineptr->compare("") == 0)
        		continue;
            else if(lineptr->compare(KEY_WORD_START_COPY_TASKS) == 0)
                mode = 0;
            else if(lineptr->compare(KEY_WORD_START_COPY_BACK_TASKS) == 0)
                mode = 1;
            else if(lineptr->compare(KEY_WORD_START_REPLACE_TASKS) == 0)
            	mode = 2;
            else if(mode == 0){
            
           		string* sp = new string(*lineptr);
                copyTasks.push_back(sp);
                }
            else if(mode == 1){
            	string* sp = new string(*lineptr);
                copyBackTasks.push_back(sp);
                }
            else if(mode == 2){
            	string* sp = new string(*lineptr);
                replaceTextTasks.push_back(sp);
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
       		CopySoft(**it, sourcePath);
       		else
       		copyFromTo(sourcePath, **it);
       		
       	delete *it;
    }
    
    cout << endl << "start copy 2" << endl << endl;
    
     for(auto it = copyTasks.begin(); it != copyTasks.end(); ++it)
    {
        string sourcePath = dataLocation + targetProfile + **it;
        if(init_mode)
       		CopySoft(**it, sourcePath);
       		else
       		copyFromTo(sourcePath, **it);
        
        delete *it;
    }
	 
	if(!init_mode)
	{
		cout << endl << "start editing files" << endl << endl;
		for(auto it = replaceTextTasks.begin(); it != replaceTextTasks.end(); ++it)
    	{
       		replaceTextTask(**it, targetProfile);
    	}
    }

    delete lineptr;
    
    ofstream out(cfgPath);
    out << targetProfile;
    out.close(); 
    
    if(video_mode)
    {
    	auto end = chrono::steady_clock::now();
    	int deltaMillis = chrono::duration_cast<chrono::milliseconds>(end - startTime).count();
    	if(deltaMillis < minMillisVideoPlayback)
    		this_thread::sleep_for(chrono::milliseconds(minMillisVideoPlayback - deltaMillis));
    		
    	cout << "waited for: " << (minMillisVideoPlayback - deltaMillis) << "\n"; 
     	system("killall vlc");
     }
    
    return 0;
}

void createDir(const fs::path& dir)
{
	try {
        fs::create_directories(dir);
    }
    catch (std::exception& e) { // Not using fs::filesystem_error since std::bad_alloc can throw too.
        std::cout << e.what() << std::endl;
    }
}

void CopySoft(const string& s, const string& d)
{
    fs::path folder(d);
	folder.remove_filename();
	createDir(folder);
    try
    {
        fs::copy(s, d, fs::copy_options::skip_existing);
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
    }

    cout << "copied soft from: " << s << " to " << d << endl;
}

void copyFromTo(const string& s, const string& d)
{
	fs::path folder(d);
	folder.remove_filename();
	createDir(folder);
    try
    {
        fs::copy(s, d, fs::copy_options::overwrite_existing);
    }
    catch (std::exception& e)
    {
        std::cout << e.what();
    }

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

//taken from https://stackoverflow.com/questions/5878775/how-to-find-and-replace-string
void ReplaceStringInPlace(std::string& subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

void replaceTextTask(const string& taskLine, const string& targetName)
{
	unsigned int p1 = taskLine.find(SEPERATION_CHAR);
	unsigned int p2 = taskLine.rfind(SEPERATION_CHAR);
	if(p1 == string::npos || p1 == p2)
	{
		cout << "taskline misses 2 seperation chars(" << SEPERATION_CHAR << ")\n" << taskLine << endl;
		return;
	}	
	string fileLocation = taskLine.substr(0, p1);
	string searchText = taskLine.substr(p1+1, p2-p1-1);
	string replacementText = taskLine.substr(p2+1,taskLine.length() - p2 - 1);
	
	ReplaceStringInPlace(replacementText, KEY_WORD_REPLACE_WITH_TARGET_NAME, targetName);
	
	//replace occurences of [profile_name] with actual name
	
	if(searchText.find(SEPERATION_CHAR) != string::npos)
	{
		cout << "taskline has too many seperation chars(" << SEPERATION_CHAR << ")\n" << taskLine << endl;
		return;
	}
	/*cout << "SEGMENTS: " << endl;
	cout << fileLocation << endl;
	cout << searchText << endl;
	cout << replacementText << endl;*/

	string* lineptr = new string;
	list<string*> inFileLines;
	ifstream inStream(fileLocation);
	int changes = 0;
	
	if(inStream.is_open())
	{
		while(std::getline(inStream, *lineptr))
		{
			if(lineptr->find(searchText) != string::npos){
				inFileLines.push_back(&replacementText);
				changes++;
				delete lineptr;
			}
			else{
				inFileLines.push_back(lineptr);
			}
			
			lineptr = new string;
			
		}	
		inStream.close();
		delete lineptr;
		
		ofstream outStream(fileLocation);
		
		if(outStream.is_open())
		{
			
    		for (list<string*>::iterator it = inFileLines.begin(); it != inFileLines.end(); ++it)
    		{
    			//std::cerr << "writing: " << **it << std::endl;
    			outStream << **it << endl;
    			if(*it != &replacementText)
    				delete *it;
    			
    		}
			outStream.close();
			cout << "written file(" << changes << " changes): " << fileLocation << endl;
		}
		else
		{
			cerr << "could not open out file: " << fileLocation << endl;
		}
	}
	else
	{
		cerr << "could not open file: " << fileLocation << endl;
	}
}
