#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cstring>
#include <sys/wait.h>
#include <unistd.h>

int num = 0;

bool exitStatus = true;
bool nestedExist = false;
bool allComment = true;
bool verboseMode = false;

std::string input, inputBuffer;
std::string openBraces = "${";
std::string path = "uShell";
std::string arrow = ">";

std::vector<std::string> strArr;
std::map<std::string, std::string> map;

void comment()
{
    std::istringstream line{ input };
    
    if(verboseMode == true)
        std::cout << input << std::endl;
    
    while (line.good())
    {
        line >> inputBuffer;

        if (inputBuffer == "#")
            break;

        strArr.push_back(inputBuffer);
    }
}

std::string findNestedBraces(std::string s)
{
    size_t stringLength, startIndex = 0;
    std::string newString;

    startIndex = s.find("${");
    stringLength = s.length();

    if (startIndex == std::string::npos)       //not found
    {
        return s;
    }
    else                                       //found
    {
        startIndex += 2;
        nestedExist = true;
        newString = s.substr(startIndex, stringLength - 1);

        return newString;
    }
}

void echo()
{
    strArr.erase(strArr.begin());

    std::string inputString, outputString;
    size_t startIndex = 0, endIndex = 0, currentIndex = 0, strLength = 0;

    for (auto& arr : strArr)
    {
        inputString += arr;
        inputString += " ";
    }

    startIndex = inputString.find("${");
    endIndex = inputString.find("}");

    if (startIndex == std::string::npos || endIndex == std::string::npos)   //not found
    {
        std::cout << inputString << std::endl;
        return;
    }

    strLength = inputString.length();

    while (currentIndex < strLength - 1)
    {
        startIndex = inputString.find("${", currentIndex);
        endIndex = inputString.find("}", startIndex);

        if (startIndex == std::string::npos || endIndex == std::string::npos)   //not found
        {
            outputString.pop_back();
            outputString += inputString.substr(currentIndex, strLength - 1);

            size_t found = inputString.find("$${");

            if (found != std::string::npos)                 //found
            {
                std::cout << "$" << outputString << std::endl;
                break;
            }
            if (nestedExist)
            {
                std::cout << openBraces << outputString << std::endl;
                nestedExist = false;
                break;
            }
            else
            {
                std::cout << outputString << std::endl;
                break;
            }
        }

        currentIndex = endIndex + 1;

        startIndex += 2;

        std::string variable;
        variable = inputString.substr(startIndex, endIndex - startIndex);
        variable = findNestedBraces(variable);

        std::map<std::string, std::string>::iterator it;
        it = map.find(variable);

        if (variable[0] == '#')
        {
            std::cout << inputString << std::endl;
            break;
        }
        else if (it != map.end())
        {
            outputString += it->second;
            outputString += " ";

            if (currentIndex == strLength - 1)
            {
                size_t found = inputString.find("$${");

                if (found != std::string::npos)                 //found
                {
                    std::cout << "$" << outputString << std::endl;
                    break;
                }
                else if (nestedExist)
                {
                    std::cout << openBraces << outputString << std::endl;
                    nestedExist = false;
                    break;
                }
                else
                {
                    std::cout << outputString << std::endl;
                    break;
                }
            }
        }
        else
        {
            if (inputString[2] == ' ' || inputString[endIndex - 1] == ' ')
            {
                std::cout << inputString << std::endl;
                break;
            }
            else
            {
                if (currentIndex < strLength - 1)
                {
                    std::string outputString2 = inputString.substr(endIndex + 1, strLength - 1);
                    std::cout << "Error: " << variable << outputString2 << "is not a defined variable" << std::endl;
                    break;
                }
                else
                {
                    std::cout << "Error: " << variable << " is not a defined variable" << std::endl;
                    break;
                }
            }
        }
    }
}

void setvar()
{
    strArr.erase(strArr.begin());
    std::string string1, string2;

    string1 = strArr[0];
    strArr.erase(strArr.begin());

    if (!strArr.empty())
    {
        for (auto& arr : strArr)
        {
            string2 += arr;
            string2 += " ";
        }

        string2.pop_back();
    }

    std::map<std::string, std::string>::iterator it;

    it = map.find(string1);

    if (it != map.end())
    {
        if (string1 == it->first)
        {
            it->second = string2;
        }
    }
    
    map.insert(std::make_pair(string1, string2));
}

void changePrompt()
{
    strArr.erase(strArr.begin());

    std::string newPath;
    for (auto& arr : strArr)
    {
        newPath += arr;
        newPath += " ";
    }

    newPath.pop_back();
    path.clear();
    path.replace(0, newPath.length(), newPath);
}

void invalidCommand()
{
    std::string msg;

    for (auto& arr : strArr)
    {
        msg += arr;
        msg += " ";
    }

    msg.pop_back();
    std::cout << msg << ": command not found" << std::endl;
}

bool externalCommand()
{
    for(int i = 0; i < (int)strArr[0].size() ; ++i)
    {
        if(!(strArr[0].compare(i,1,":")))
            strArr[0].replace(i,1," ");
    }
 
    std::stringstream stream{strArr[0]};
    std::vector<std::string> path;
    std::string s;
    
    while (stream.good())
    {
        stream >> s;
        path.push_back(s);
    }
        
    int pid;  
    pid = fork();
    
    if(pid == 0)
    {
        for(auto& subPath: path)
        {
            std::vector<const char*> str;
            std::size_t found = subPath.find_last_of("/\\");
            
            std::string path = subPath.substr(0,found);
            
            if(found ==std::string::npos)
                found = 0;
            else
                found++;
            
            std::string file = subPath.substr(found);
            str.push_back(file.c_str());
            
            for(size_t i; i < strArr.size(); ++i)
                str.push_back(strArr[i].c_str());
            
            str.push_back(NULL);
            
            if(!path.compare(file))
            {
                execv(path.c_str(), const_cast<char* const*>(str.data()));
                execvp(path.c_str(), const_cast<char* const*>(str.data()));
            }
            else
            {
                execv(subPath.c_str(),const_cast<char* const*>(str.data()));
            }
        }
        exit(1);
    }
    else
    {
        int status;
        wait(&status);
        
        if(WIFEXITED(status))
            if(WEXITSTATUS(status))
                return 1;
    }
    return 0;
}

void exit()
{
    strArr.erase(strArr.begin());
    std::istringstream stream(inputBuffer);
    stream >> num;

    exitStatus = 0;
}

int main(int argc, char*argv[])
{
    while (exitStatus)
    {
        std::cout << path << arrow;
        getline(std::cin, input); 
        
        if (input[0] == '#')
           continue;
   
        if(argc == 2)
        {
            if((!strcmp(argv[1],"-v") || !strcmp(argv[1], "[-v]")))
                verboseMode = true;
            else
                verboseMode = false;
        }
     
       comment();

        if (strArr[0] == "echo")
        {
            echo();
        }
        else if (strArr[0] == "setvar")
        {
            setvar();
        }
        else if (strArr[0] == "changeprompt")
        {
            changePrompt();
        }
        else if (strArr[0] == "exit")
        {
            exit();
        }
        else
        {
            if(externalCommand())
            invalidCommand();
        }
        strArr.clear();
    }
    return num;
}
