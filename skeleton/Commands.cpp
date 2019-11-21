#include <unistd.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"
#include <stdio.h>
#include <iostream>

using namespace std;

#if 0
#define FUNC_ENTRY()  \
  cout << __PRETTY_FUNCTION__ << " --> " << endl;

#define FUNC_EXIT()  \
  cout << __PRETTY_FUNCTION__ << " <-- " << endl;
#else
#define FUNC_ENTRY()
#define FUNC_EXIT()
#endif


const std::string WHITESPACE = " \n\r\t\f\v";
string _ltrim(const std::string& s)
{
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string& s)
{
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string& s)
{
  return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char* cmd_line, char** args) {
  FUNC_ENTRY()
  int i = 0;
  std::istringstream iss(_trim(string(cmd_line)).c_str());
  for(std::string s; iss >> s; ) {
    args[i] = (char*)malloc(s.length()+1);
    memset(args[i], 0, s.length()+1);
    strcpy(args[i], s.c_str());
    args[++i] = NULL;
  }
  return i;

  FUNC_EXIT()
}

class Command {
protected:
    char ** args;
    int argsNum;
public:
    Command(const char* cmd_line){
        argsNum= _parseCommandLine(cmd_line, args);
    };
    virtual ~Command(){
        for (int i = 0; i < argNum; ++i) {
            free(args[i]);
        }
        free(args);
    }

    virtual void execute() = 0;
    //virtual void prepare();
    //virtual void cleanup();
    // TODO: Add your extra methods if needed
};


/*int _parseCommandLine(const char* cmd_line, char** args) {
  FUNC_ENTRY()
  stringstream check1(cmd_line);
  string intermediate;
  int i = 0;
  while(getline(check1, intermediate, ' '))
  {
    args[i] = (char*)malloc(intermediate.length()+1);
    memset(args[i], 0, intermediate.length()+1);
    strcpy(args[i], intermediate.c_str());
    args[++i] = NULL;
  }
  return i;

  FUNC_EXIT()
}
*/

bool _isBackgroundComamnd(const char* cmd_line) {
  const string whitespace = " \t\n";
  const string str(cmd_line);
  return str[str.find_last_not_of(whitespace)] == '&';
}

void _removeBackgroundSign(char* cmd_line) {
  const string whitespace = " \t\n";
  const string str(cmd_line);
  // find last character other than spaces
  size_t idx = str.find_last_not_of(whitespace);
  // if all characters are spaces then return
  if (idx == string::npos) {
    return;
  }
  // if the command line does not end with & then return
  if (cmd_line[idx] != '&') {
    return;
  }
  // replace the & (background sign) with space and then remove all tailing spaces.
  cmd_line[idx] = ' ';
  // truncate the command line string up to the last non-space character
  cmd_line[str.find_last_not_of(whitespace, idx-1) + 1] = 0;
}

// TODO: Add your implementation for classes in Commands.h 

SmallShell::SmallShell() {
}

SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line) {
	// For example:

  string cmd_s = string(cmd_line);
  if (cmd_s.find("pwd") == 0) {
    return new GetCurrDirCommand(cmd_line);
  }
  else if (cmd_s.find("cd"))
      return new ChangeDirCommand(cmd_line);
  else if(cmd_s.find("history"))
      return new HistoryCommand(cmd_line, *history);
  else if(cmd_s.find("jobs"))
      return new JobsCommand(cmd_line, *jobs);
  else if(cmd_s.find("kill"))
      return new KillCommand(cmd_line, *jobs);
  else if(cmd_s.find("showpid"))
      return new ShowPidCommand(cmd_line);
  else if(cmd_s.find("fg"))
      return new ForegroundCommand(cmd_line, *jobs);
  else if(cmd_s.find("bg"))
      return new BackgroundCommand(cmd_line, *jobs);
  else if(cmd_s.find("quit"))
      return new QuitCommand(cmd_line, *jobs);
  else if(cmd_s.find("cp"))
      return new CopyCommand(cmd_line);
  return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line) {
  // TODO: Add your implementation here
  // for example:
  // Command* cmd = CreateCommand(cmd_line);
  // cmd->execute();
  // Please note that you must fork smash process for some commands (e.g., external commands....)
}

class CommandsHistory {
protected:
    class CommandHistoryEntry {
    private:
        int seqNum;
        char* command;
    public:
        CommandHistoryEntry(int seq, char*comm):seqNum(seq){
            command = new char[strlen(comm)+1];
            strcpy(command, comm);
        }

        ~CommandHistoryEntry(){
            delete[](command);
        }

        void repeatCommand(){
            seqNum++;
        }

        int compareCommand(char*comm){
            return strcmp((comm, command));
        }

        ostream& operator<<(ostream& os, const CommandHistoryEntry& dt){
            cout << right << setw(5) << seqNum << " " << command << endl;
            return os;
        }
    };

private:
    CommandHistoryEntry history[50];
    int top=0;
    int capcitcy=0;
    int seq=1;

    // TODO: Add your data members
public:
    CommandsHistory(){}
    ~CommandsHistory() {}
    void addRecord(const char* cmd_line){
        if(history[top].compareCommand(cmd_line)==0)
            history[top].repeatCommand();
        else
            history[top]= CommandHistoryEntry(seq, cmd_line);
        if (top++==50)
            top=0;

        seq++;
        if (capcitcy!=49)
            capcitcy++;

    }

    void printHistory(){
        for (int i= top; i<=capcitcy){
            cout<< history[i];
        }

        for (int j = 0; j < top; ++j) {
            out<< history[j];
        }
    }
};

class GetCurrDirCommand : public BuiltInCommand {
public:
    GetCurrDirCommand(const char* cmd_line){}
    virtual ~GetCurrDirCommand() {}
    void execute() override{
        cout<< getcwd();
    }
};

class HistoryCommand : public BuiltInCommand {
    CommandsHistory * history;
public:
    HistoryCommand(const char* cmd_line, CommandsHistory* hist):history(hist){}
    virtual ~HistoryCommand() {}
    void execute() override{
        *history->printHistory();
    }
};


class KillCommand : public BuiltInCommand {
    JobsList* jobs
public:
    KillCommand(const char* cmd_line, JobsList* j):jobs(j){}
    virtual ~KillCommand() {}
    void execute() override{
        if()
    }
};
