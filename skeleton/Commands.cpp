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

string _ltrim(const std::string &s) {
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

string _rtrim(const std::string &s) {
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

string _trim(const std::string &s) {
    return _rtrim(_ltrim(s));
}

int _parseCommandLine(const char *cmd_line, char **args) {
    FUNC_ENTRY()
    int i = 0;
    std::istringstream iss(_trim(string(cmd_line)).c_str());
    for (std::string s; iss >> s;) {
        args[i] = (char *) malloc(s.length() + 1);
        memset(args[i], 0, s.length() + 1);
        strcpy(args[i], s.c_str());
        args[++i] = NULL;
    }
    return i;

    FUNC_EXIT()
}


Command::Command(const char *cmd_line) {
    argsNum = _parseCommandLine(cmd_line, args);
};

Command::~Command() {
    for (int i = 0; i < this->argsNum; ++i) {
        free(args[i]);
    }
    free(args);
}

//virtual void prepare();
//virtual void cleanup();
// TODO: Add your extra methods if needed



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

bool _isBackgroundComamnd(const char *cmd_line) {
    const string whitespace = " \t\n";
    const string str(cmd_line);
    return str[str.find_last_not_of(whitespace)] == '&';
}

void _removeBackgroundSign( char *cmd_line) {
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
    cmd_line[str.find_last_not_of(whitespace, idx - 1) + 1] = 0;
}

// TODO: Add your implementation for classes in Commands.h 

SmallShell::SmallShell():jobs(), history() {

}

SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command *SmallShell::CreateCommand(const char *cmd_line) {
    // For example:

    string cmd_s = string(cmd_line);
    if (cmd_s.find("pwd") == 0) {
        return new GetCurrDirCommand(cmd_line);
    } else if (cmd_s.find("cd"))
        return new ChangeDirCommand(cmd_line);
    else if (cmd_s.find("history"))
        return new HistoryCommand(cmd_line, &history);
    else if (cmd_s.find("jobs"))
        return new JobsCommand(cmd_line, &jobs);
    else if (cmd_s.find("kill"))
        return new KillCommand(cmd_line, &jobs);
    else if (cmd_s.find("showpid"))
        return new ShowPidCommand(cmd_line);
    else if (cmd_s.find("fg"))
        return new ForegroundCommand(cmd_line, &jobs);
    else if (cmd_s.find("bg"))
        return new BackgroundCommand(cmd_line, &jobs);
    else if (cmd_s.find("quit"))
        return new QuitCommand(cmd_line, &jobs);
    else if (cmd_s.find("cp"))
        return new CopyCommand(cmd_line);
    return nullptr;
}

CommandHistoryEntry::CommandHistoryEntry(int seq, const char *comm) : seqNum(seq) {
    command = new char[strlen(comm) + 1];
    strcpy(command, comm);
}


// TODO: Add your data members
CommandsHistory::CommandsHistory() {}

CommandsHistory::~CommandsHistory() {}

CommandHistoryEntry::~CommandHistoryEntry() {
    delete[](command);
}

void CommandHistoryEntry::repeatCommand() {
    seqNum++;
}


void CommandsHistory::addRecord(const char *cmd_line) {
    if (history[top]->compareCommand(cmd_line) == 0)
        history[top]->repeatCommand();
    else
        *history[top] = CommandHistoryEntry(seq, cmd_line);
    if (top++ == 50)
        top = 0;

    seq++;
    if (capcitcy != 49)
        capcitcy++;

}

void CommandsHistory::printHistory() {
    for (int i = top; i <= capcitcy; ++i) {
        cout << history[i];
    }

    for (int j = 0; j < top; ++j) {
        cout << history[j];
    }
}

GetCurrDirCommand::GetCurrDirCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}

GetCurrDirCommand::~GetCurrDirCommand() {}

void GetCurrDirCommand::execute() {
    char cwd[256];
    int size;
    getcwd(cwd, size);
    cout << cwd << endl;
}


HistoryCommand::HistoryCommand(const char *cmd_line, CommandsHistory *hist) : BuiltInCommand(cmd_line), history(hist) {}

HistoryCommand::~HistoryCommand() {}

void HistoryCommand::execute() {
    history->printHistory();
}


KillCommand::KillCommand(const char *cmd_line, JobsList *jobs) : BuiltInCommand(cmd_line), jobs(jobs) {}

KillCommand::~KillCommand() {}

void KillCommand::execute() {
    if (argsNum < 3)
        cout << "smash error: kill: invalid arguments" << endl;
    else {
        int id = atoi(args[1]);
        if (jobs->removeJobById(id))
            cout << "signal number " << id << " was sent to pid 30985" << endl;
        else
            cout << "smash error: kill: job-id" << id << " does not exist" << endl;
    }
};


ForegroundCommand::ForegroundCommand(const char *cmd_line, JobsList *jobj) : BuiltInCommand(cmd_line), jobs(jobj) {}

ForegroundCommand::~ForegroundCommand() {

}

void ForegroundCommand::execute() {
    JobsList::JobEntry *entry;
    int id;
    if (argsNum > 2)
        cout << "smash error: fg: invalid arguments" << endl;

    if (argsNum < 2) {
        entry = jobs->getLastJob(&id);
        if (entry == NULL) {
            cout << "smash error: fg: jobs list is empty" << endl;
            return;
        }
    } else {
        id = atoi(args[1]);
        entry = jobs->getJobById(id);
        if (entry == NULL) {
            cout << "smash error: fg: job-id " << id << " does not exist" << endl;
            return;
        }
    }

    int pid = entry->getPID();

    waitpid(pid, NULL, NULL);

}


QuitCommand::QuitCommand(const char *cmd_line, JobsList *j) : BuiltInCommand(cmd_line), jobs(j) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute() {
    if (argsNum == 2) {
        if (strcmp(args[1], "kill") == 0)
            jobs->killAllJobs();
    }
    raise(SIGKILL);
}


void SmallShell::executeCommand(const char *cmd_line) {
    // TODO: Add your implementation here

    char **args;
    bool bkg = _isBackgroundComamnd(cmd_line);
    char new_command[strlen(cmd_line)];
    strcpy(new_command,cmd_line);
    if (bkg) {
        _removeBackgroundSign(new_command);
    }

    Command* clean_command = CreateCommand(new_command);

    int pid = 0;
    if (bkg) {
        //TODO: Update job into joblist
        pid = fork();
    }
    if (pid == 0) //I'm the son (or foreground)
        clean_command->execute();

    // for example:
    // Command* cmd = CreateCommand(cmd_line);
    // cmd->execute();
    // Please note that you must fork smash process for some commands (e.g., external commands....)
}

int CommandHistoryEntry::compareCommand(const char *comm) {
    return strcmp(comm, command);
}

ostream &operator<<(ostream &os, const CommandHistoryEntry &dt) {
    os << right << setw(5) << dt.seqNum << " " << dt.command << endl;
    return os;
}
