#include <unistd.h>

#include <string.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <iomanip>
#include "Commands.h"
#include <time.h>
#include "BidirectionalList.h"
#include <sys/types.h>
#include <fcntl.h>

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


typedef enum {
    BACKGROUND_JOB,
    STOPPED_JOB
}JobStatus;



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
    JobsList* jobs;
public:
    Command(const char* cmd_line){
        argsNum= _parseCommandLine(cmd_line, args);
    };
    virtual ~Command(){
        for (int i = 0; i < argsNum; ++i) { //was argnum, changed to argsnum
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
// TODO: add your implementation
}

SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command * SmallShell::CreateCommand(const char* cmd_line) {
    // For example:
/*
  string cmd_s = string(cmd_line);
  if (cmd_s.find("pwd") == 0) {
    return new GetCurrDirCommand(cmd_line);
  }
  else if ...
  .....
  else {
    return new ExternalCommand(cmd_line);
  }
  */
    return nullptr;
}

void SmallShell::executeCommand(const char *cmd_line) {
    // TODO: Add your implementation here

    char** args;
    bool bkg=_isBackgroundComamnd(cmd_line);

    if (bkg){
        _removeBackgroundSign(cmd_line);
    }

    Command clean_command = CreateCommand(cmd_line);

    int pid= 0;
    if (bkg) {
        //TODO: Update job into joblist
        pid = fork();
    }
    if (pid == 0) //I'm the son (or foreground)
        clean_command.execute();



    // for example:
    // Command* cmd = CreateCommand(cmd_line);
    // cmd->execute();
    // Please note that you must fork smash process for some commands (e.g., external commands....)
}

class JobsList {
public:
    class JobEntry {
        // TODO: Add your data members
    private:
        int jobPID;
        int jobSeqID;
        char* jobcommand;
        time_t jobAddingTime;
        JobStatus status;

    public:
        JobEntry(int PID, int SeqID, char* command):jobPID(PID), jobSeqID(SeqID), JobStatus(BACKGROUND_JOB){
            jobcommand = new char[(strlen(command))+1];
            strcpy(jobcommand, command);
            jobAddingTime=time();
        }
        ~JobEntry(){
            delete[](jobcommand);
        }

        int getJobPID(){
            return jobPID;
        }

        JobStatus getJobStatus(){
            return status;
        }

        ostream& operator<<(ostream& os, const JobEntry& jobentry){
            double secondsElapsed = difftime(time(), jobAddingTime);

            if (jobentry.status == STOPPED_JOB){

                cout << jobentry.jobSeqID << " " << jobentry.jobcommand << " : " << jobentry.jobPID << " " << secondsElapsed << "(stopped)" << endl;
            }
            else{
                cout << jobentry.jobSeqID << " " << jobentry.jobcommand << " : " << jobentry.jobPID << " " << secondsElapsed << endl;
            }
            return os;
        }

    };

private:
    BiDirectionalList<JobEntry> list;
    int JobsNum;


    // TODO: Add your data members
public:
    JobsList(): JobsNum(0){
        list= BiDirectionalList<JobEntry>();
    };
    ~JobsList(){ //anythind needed here?
    }
    void addJob(int pid, Command* cmd, bool isStopped = false){
        if (JobsNum>=100){
            return;
        }

        if (list.push(JobEntry(pid, JobsNum+1, Command.command))){  //not command, but the command itself as str, TODO: fix
                JobsNum++;
        }
        }

    void printJobsList() {

//TODO: figure this shit out
        removeFinishedJobs(); //MAYBE DELETE THIS, NOT NECESSARY
        auto iterator = list.getHead();
        while (iterator != nullptr) {
            JobEntry current = iterator->data;
                cout << current << " \n"; //TODO: FIX THIS...
                iterator = (iterator->next);
            }
    }



    void killAllJobs(){
        auto iterator = list.getHead();
        while (iterator != nullptr) {
            JobEntry current = iterator->data;
            int currpid=current.getJobPID();

            if (kill(currpid, 0)) {
                kill(currpid, 9);
            }//TODO: figure out when this would even be necessary
               iterator = (iterator->next);
                list.removeNode(iterator->previous);
            JobsNum--;
                continue;
            }
        }



    void removeFinishedJobs() {
        auto iterator = list.getHead();
        while (iterator != nullptr) {
            JobEntry current = iterator->data;
            int currpid = current.getJobPID(); //pid should be of type pid_t
            if (waitpid(currpid, WNOHANG)) {//should be executed by father
                kill(currpid, SIGKILL);
                iterator = (iterator->next);
                list.removeNode(iterator->previous);
                JobsNum--;
            }
        }
    }



    JobEntry* getJobById(int jobId){ //used to return JobEntry* ... this ain't C, but would it be considered a mistake?

        auto iterator = list.getHead();
        while (iterator != nullptr) {
            JobEntry current = iterator->data;
            if (current.getJobPID() == jobId) {
                return &current;
            }
            iterator = (iterator->next);
        }
        return nullptr;
    }

    void removeJobById(int jobId){
        auto iterator = list.getHead();
        while (iterator != nullptr) {
            JobEntry current = iterator->data;
            if (current.getJobPID() == jobId) {
                if(kill(jobId,0)){  //TODO: and else? could this even happen?
                    kill(jobId,SIGKILL);
                }
                list.removeNode(iterator);
                    JobsNum--;
                return;
            }
            iterator = (iterator->next);
        }
    }
    JobEntry * getLastJob(int* lastJobId) {
        auto iterator = list.getHead();
        while (iterator->next != nullptr) {
            iterator = iterator->next;
        }
        JobEntry current = iterator->data;
        return &current;
    }

    JobEntry *getLastStoppedJob(int *jobId) {
        auto iterator = list.getTail();
        while ((iterator->data).getJobStatus() != STOPPED_JOB
               && iterator->previous != nullptr) {
            iterator = iterator->previous;
        }
        if (iterator->previous == nullptr
            && iterator->data.getJobStatus() != STOPPED_JOB) {
            return nullptr;
        } else {
            JobEntry current = iterator->data;
            return &current; //TODO: CAN I CHANGE THE RETURN TYPE OF A FUNCTION THEY DESIGNED?
        }
    }
    // TODO: Add extra methods or modify exisitng ones as needed
};


class CommandsHistory {
protected:
    class CommandHistoryEntry {
        // TODO: Add your data members
    };
    // TODO: Add your data members
public:
    CommandsHistory();
    ~CommandsHistory() {}
    void addRecord(const char* cmd_line);
    void printHistory();
};

class HistoryCommand : public BuiltInCommand {
    // TODO: Add your data members
public:
    HistoryCommand(const char* cmd_line, CommandsHistory* history);
    virtual ~HistoryCommand() {}
    void execute() override;
};



class GetCurrDirCommand : public BuiltInCommand {
public:
    GetCurrDirCommand(const char* cmd_line){}
    virtual ~GetCurrDirCommand() {}
    void execute() override{
        cout<< getcwd();
    }
};
//DID they mean... past working directory? wtf is this
ChangeDirCommand::ChangeDirCommand(const char *cmd_line, char **plastPwd) : BuiltInCommand(cmd_line), prevDir(*plastPwd){} //TODO: is this what they meant??
virtual ~ChangeDirCommand() {}
void ChangeDirCommand::setPrevDir(char* d){
    //delete[](prevDir);
    prevDir = new char[(strlen(d))+1];
    strcpy(prevDir, d);
}
void ChangeDirCommand::execute(){

    char pwd[256];
    int length;
    if(argsNum > 2){
        cout << "smash error: cd: too many arguments" << endl;
    }

    else{
        if(args[0]=="-") {
            if (prevDir == nullptr) {
                cout << "smash error: cd: OLDPWD not set" << endl;
            } else {
                char* temp=prevDir;
                setPrevDir(getcwd(pwd,length));
                if(chdir(temp)<0){
                    perror("smash error: chdir failed");
                } //TODO: the structure must always hold the previous working directory
            }
        }
        else{
            setPrevDir(getcwd(pwd,length));
            if(args[0]==".."){
                if(chdir("/home")<0){
                    perror("smash error: chdir failed");
                }
            }
            else{
                if(chdir(args[0])<0){
                    perror("smash error: chdir failed");
                }
            }
        }
    }
}

JobsCommand::JobsCommand(const char* cmd_line, JobsList* jobs) : BuiltInCommand (cmd_line), jobs(jobs){}
virtual ~JobsCommand() {}
void JobsCommand::execute(){
    jobs.removeFinishedJobs();
    jobs.printAllJobs();
}

ShowPidCommand::ShowPidCommand(const char* cmd_line): BuiltInCommand(cmd_line){}
virtual ~ShowPidCommand() {}
void ShowPidCommand::execute(){
       pid_t id=getpid();
    cout << "smash pid is " << long(id) << endl;
}

BackgroundCommand::BackgroundCommand(const char* cmd_line, JobsList* jobs): BuiltInCommand(cmd_line), jobs(jobs){}
virtual ~BackgroundCommand() {}
void BackgroundCommand::execute() {

    JobsList::JobEntry * je;
    int ID=0;
    if (argsNum > 2){
        cout << "smash error: bg: invalid arguments" << endl;
        return;
    }
    if (argsNum<2){
        je=jobs.getLastStoppedJob(&ID);
        if(!je){
            cout << "smash error: there is no stopped jobs to resume" << endl;
            return;
    }
    else {
        je = jobs.getJobByID(ID);
            if(!je){
                cout << "smash error: bg: job-id " << ID << " does not exist" << endl;
                return;
            }
        if(je->getJobStatus()==BACKGROUND_JOB){
            cout << "smash error: bg: job-id " << ID << " is already running in the background" << endl;
            return;
        }
    }
        *je.JobSetStatus(BACKGROUND_JOB);
        kill(ID, SIGCONT);
    }

}


CopyCommand::CopyCommand(const char* cmd_line): BuiltInCommand(cmd_line){}
virtual ~CopyCommand() {}
void CopyCommand::execute(){
    int fd=0;
    if (argsNum<3){
        cout<< "smash error: cp: invalid arguments" << endl; //TODO: WHAT IF THIS HAPPENS? PERROR?
    }
    char* buffer=new char[100];
    char* readfile=args[1];
    char* writefile=args[2];
    int fdread=open(O_RDONLY, readfile);
    int fdwrite=open(O_WRONLY, writefile);
    if(fdread==-1 || fdwrite==-1){
        perror("smash error: open failed");
        delete[](buffer);
        return;
    }
    ssize_t s=read(fdread, buffer,90);
         while (s!=0){
             if(s==-1){
                 perror("smash error: read failed");
                 delete[](buffer);
                 return;
             }
             if(write(fdwrite,buffer,90)==-1){
                 perror("smash error: write failed");
                 delete[](buffer);
                 return;
        }
             s=read(fdread, buffer,90);
    }
    if(close(fdread)==-1 ||close(fdwrite)==-1){
        perror("smash error: close failed");
    }
     delete[]buffer;
}