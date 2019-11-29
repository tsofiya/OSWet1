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
#include <time.h>
#include "BidirectionalList.h"
#include <sys/types.h>
#include <fcntl.h>
#include <pwd.h>

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

Command::Command(const char* cmd_line){
    args= (char**)malloc(COMMAND_MAX_ARGS*sizeof(char*));
    argsNum = _parseCommandLine(cmd_line, args);
}

Command::~Command(){
    for (int i = 0; i < COMMAND_MAX_ARGS; ++i) {
        free(args[i]);
    }
    free(args);
}

BuiltInCommand::BuiltInCommand(const char* cmd_line):Command(cmd_line){}

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

int JobEntry::getJobSeqID(){
    return jobSeqID;
}

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

SmallShell::SmallShell():jobs(), history(), plastPwd(nullptr) {

}

SmallShell::~SmallShell() {
// TODO: add your implementation
}

/**
* Creates and returns a pointer to Command class which matches the given command line (cmd_line)
*/
Command *SmallShell::CreateCommand(const char *cmd_line) {
    // For example:
    history.addRecord(cmd_line);
    string cmd_s = string(cmd_line);
    if (cmd_s.find("pwd") == 0) {
        return new GetCurrDirCommand(cmd_line);
    }
    else if (cmd_s.find("cd") == 0) {
        char buffer[256];
        size_t s;
        // char* p=(getcwd(buffer, s));
        //char **plast =&p;
        return new ChangeDirCommand(cmd_line, &plastPwd);
    }
    else if (cmd_s.find("history") == 0)
        return new HistoryCommand(cmd_line, &history);
    else if (cmd_s.find("jobs") == 0)
        return new JobsCommand(cmd_line, &jobs);
    else if (cmd_s.find("kill") == 0)
        return new KillCommand(cmd_line, &jobs);
    else if (cmd_s.find("showpid") == 0)
        return new ShowPidCommand(cmd_line);
    else if (cmd_s.find("fg") == 0)
        return new ForegroundCommand(cmd_line, &jobs);
    else if (cmd_s.find("bg") == 0)
        return new BackgroundCommand(cmd_line, &jobs);
    else if (cmd_s.find("quit") == 0)
        return new QuitCommand(cmd_line, &jobs);
    else if (cmd_s.find("cp") == 0)
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
    if (capcitcy!=0 && history[top]->compareCommand(cmd_line) == 0) {
        history[top]->repeatCommand();
    }
    else {
        if (top++ == 49)
            top = 0;
        if(capcitcy==49)
            delete(history[top]);
        history[top] = new CommandHistoryEntry(seq, cmd_line);
        if (capcitcy != 49)
            capcitcy++;
    }

    seq++;


}

void CommandsHistory::printHistory() {
    for (int i = top+1; i < capcitcy; ++i) {
        cout << *history[i];
    }

    for (int j = 0; j <= top; ++j) {
        cout << *history[j];
    }
}

GetCurrDirCommand::GetCurrDirCommand(const char *cmd_line) : BuiltInCommand(cmd_line) {}

GetCurrDirCommand::~GetCurrDirCommand() {}

void GetCurrDirCommand::execute() {
    char cwd[256];
    getwd(cwd);
    cout << cwd << endl;
}


HistoryCommand::HistoryCommand(const char *cmd_line, CommandsHistory *hist) : BuiltInCommand(cmd_line), history(hist) {}

HistoryCommand::~HistoryCommand() {}

void HistoryCommand::execute() {
    history->printHistory();
}


KillCommand::KillCommand(const char *cmd_line, JobsList *jobs) : BuiltInCommand(cmd_line), jobs(jobs) {}
KillCommand::~KillCommand(){}

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
ForegroundCommand::~ForegroundCommand(){}

void ForegroundCommand::execute() {
    JobEntry *entry;
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

    int pid = entry->getJobPID();

    waitpid(pid, NULL, NULL);

}


QuitCommand::QuitCommand(const char *cmd_line, JobsList *j) : BuiltInCommand(cmd_line), jobs(j) {}

void QuitCommand::execute() {
    if (argsNum == 2) {
        if (strcmp(args[1], "kill") == 0)
            jobs->killAllJobs();
    }
    raise(SIGKILL);
}


void SmallShell::executeCommand(const char *cmd_line) {
    // TODO: Add your implementation here

    char** args;
    bool bkg=_isBackgroundComamnd(cmd_line);
    char* cmd_line_copy= (char*)malloc(100);
    strcpy(cmd_line_copy,cmd_line);

    if (bkg){
        _removeBackgroundSign(cmd_line_copy);
    }

    Command * clean_command = CreateCommand(cmd_line);

    int pid= 0;
    if (bkg) {
        //TODO: Update job into joblist
        pid = fork();
    }
    if (pid == 0) //I'm the son (or foreground)
        clean_command->execute();

    //delete( clean_command);
    free(cmd_line_copy);


    // for example:
    // Command* cmd = CreateCommand(cmd_line);
    // cmd->execute();
    // Please note that you must fork smash process for some commands (e.g., external commands....)
}


JobsList::JobsList(): JobsNum(0){
    list= std::vector<JobEntry>();
}
JobsList::~JobsList(){ }

void JobsList::addJob(int pid, char* cmd, bool isStopped = false){//used to receive Command* cmd instead of char* cmd, but command is a virtual class...
    if (JobsNum>=100){
        return;
    }
    //JobEntry j = new JobEntry(pid, JobsNum+1,cmd);
    list.push_back(JobEntry(pid, (JobsNum+1),cmd));
    //list.push_back(JobEntry(pid, JobsNum+1, cmd));  //not command, but the command itself as str, TODO: fix
    JobsNum++;
}

void JobsList::printJobsList() {

//TODO: figure this shit out
    // JobsList::removeFinishedJobs(); //MAYBE DELETE THIS, NOT NECESSARY
    /*for(auto iterator = list.vector::begin(); iterator!=list.vector::end(); ++iterator) {
         JobEntry current = *iterator;
        std::cout << *iterator << std::endl; //TODO: FIX THIS...
    */
    for(int i=0; i<list.size();i++){
        std::cout<<"printing..."<<std::endl;
        std::cout<<list.at(i)<<std::endl;

    }
}

void JobsList::killAllJobs(){
    for (auto iterator = list.begin(); iterator!=list.end();++iterator){
        // JobEntry current = iterator->data;
        int currpid=iterator->getJobPID();
        if (kill(currpid, 0)) {
            kill(currpid, 9);
        }//TODO: figure out when this would even be necessary
        // JobsNum--;
    }
    list.clear();
    JobsNum=0;

}

void JobsList::removeFinishedJobs() {
    for (auto iterator = list.begin(); iterator!=list.end();++iterator){
        //JobEntry current = iterator->data;
        pid_t currpid = iterator->getJobPID();
        int* ptr;
        waitpid(currpid, ptr, WNOHANG);
        if(*ptr>0){//should be executed by father
            kill(currpid, SIGKILL);
            list.erase(iterator);
            JobsNum--;
        }
    }
}





JobEntry * JobsList::getJobById(int jobId){ //used to return JobEntry* ... this ain't C, but would it be considered a mistake?
    for(int i=0; i<list.size();i++){
        if (list.at(i).getJobPID()==jobId){
            return &(list.at(i));
        }
    }
    return nullptr;
}

bool JobsList::removeJobById(int jobId){
    for (auto iterator = list.begin(); iterator!=list.end();++iterator){
        if (iterator->getJobPID() == jobId) {
            //    if(kill(jobId,0)){  //TODO: and else? could this even happen?
            //      kill(jobId,SIGKILL);
            // }
            list.erase(iterator);
            JobsNum--;
            return true;
        }
    }
    return false;
}

JobEntry * JobsList::getLastJob(int* lastJobId) {

    if(list.begin()==list.end()){
        return nullptr;
    }
    int i=list.size()-1;
    *lastJobId = (list.at(i).getJobPID());//TODO: update last job ID !!!!
    return &(list.at(i));
}

JobEntry * JobsList::getLastStoppedJob(int *jobId) {

    int MaxSeqID=0;
    JobEntry* current;
    if(list.begin()==list.end()){
        return nullptr;
    }
    if(list.begin()->getJobStatus()==STOPPED_JOB) {
        MaxSeqID = list.begin()->getJobSeqID();
    }
    for (auto iterator = list.begin(); iterator!=list.end();++iterator){
        if(iterator->getJobSeqID() > MaxSeqID && iterator->getJobStatus()==STOPPED_JOB){
            *current=*iterator;
            MaxSeqID=iterator->getJobSeqID();
        }

    }
    if(MaxSeqID==0 || current->getJobStatus()!=STOPPED_JOB){
        MaxSeqID=0;
        jobId=&MaxSeqID;
        return nullptr;
    }
    jobId=&MaxSeqID;
    return current;



    /* auto iterator = list.getTail();
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
     */
}
// TODO: Add extra methods or modify exisitng ones as needed

//DID they mean... past working directory? wtf is this
ChangeDirCommand::ChangeDirCommand(const char *cmd_line, char **plastPwd) : BuiltInCommand(cmd_line), plastPwd(plastPwd){//can i do this?
    //   prevDir=new char[(strlen(*plastPwd))+1];
    //  strcpy(prevDir,*plastPwd);
} //TODO: is this what they meant??
ChangeDirCommand::~ChangeDirCommand() {
    //  delete[]prevDir;
}
/*void ChangeDirCommand::setPrevDir(char* d){
    delete[](prevDir);
   prevDir = new char[(strlen(d))+1];
    strcpy(prevDir, d);

}*/

void ChangeDirCommand::execute(){
    if(argsNum > 2){
        cout << "smash error: cd: too many arguments" << endl;
        return;
    }

    char pwd[256];
    getwd(pwd);
    if (strcmp(args[1], "..")==0){
        char *result = (char*)malloc(strlen(pwd) + strlen("/..") + 1); // +1 for the null-terminator
        // in real code you would check for errors in malloc here
        strcpy(result, pwd);
        strcat(result, "/..");

        if(chdir(result)<0){
            perror("smash error: chdir failed");
            return;
        }
        free(result);

    }
    else if (strcmp(args[1], "~")==0) {
        char *ptr = pwd;
        int count = 0;
        while (*ptr != '\0' && count != 2) {
            if (*ptr == '/')
                count++;
            ptr++;

        }
        *ptr = '\0';
        if (chdir(pwd) < 0) {
            perror("smash error: chdir failed");
            return;
        }
    }

    else if (strcmp(args[1], "-")==0){
        if (*plastPwd == nullptr) {
            cout << "smash error: cd: OLDPWD not set" << endl;
            return;
        }

        if(chdir(*plastPwd)<0){
            perror("smash error: chdir failed");
            return;
        }
    }else {
        if(chdir(args[1])<0){
            perror("smash error: chdir failed");
            return;
        }
    }

    // setPrevDir(pwd); //TODO: change this function.
    free(*plastPwd);
    *plastPwd=(char*)malloc(strlen(pwd)+1);
    strcpy(*plastPwd,pwd);

}


JobsCommand::JobsCommand(const char* cmd_line, JobsList* jobs) : BuiltInCommand (cmd_line), jobs(jobs){}
JobsCommand::~JobsCommand() {}
void JobsCommand::execute(){
    jobs->removeFinishedJobs();
    jobs->printJobsList();
}

ShowPidCommand::ShowPidCommand(const char* cmd_line): BuiltInCommand(cmd_line){}
ShowPidCommand::~ShowPidCommand() {}
void ShowPidCommand::execute(){
    pid_t id=getpid();
    cout << "smash pid is " << long(id) << endl;
}

BackgroundCommand::BackgroundCommand(const char* cmd_line, JobsList* jobs): BuiltInCommand(cmd_line), jobs(jobs){}
BackgroundCommand::~BackgroundCommand() {}
void BackgroundCommand::execute() {

    JobEntry * je;
    int ID=0;
    if (argsNum > 2){
        cout << "smash error: bg: invalid arguments" << endl;
        return;
    }
    if (argsNum<2){
        je=jobs->getLastStoppedJob(&ID);
        if(!je){
            cout << "smash error: there is no stopped jobs to resume" << endl;
            return;
        }
        else {
            je = jobs->getJobById(ID);
            if(!je){
                cout << "smash error: bg: job-id " << ID << " does not exist" << endl;
                return;
            }
            if(je->getJobStatus()==BACKGROUND_JOB){
                cout << "smash error: bg: job-id " << ID << " is already running in the background" << endl;
                return;
            }
        }
        je->JobEntry::JobSetStatus(BACKGROUND_JOB);
        kill(ID, SIGCONT);
    }

}


CopyCommand::CopyCommand(const char* cmd_line): BuiltInCommand(cmd_line){}
CopyCommand::~CopyCommand() {}
void CopyCommand::execute(){
    int fd=0;
    if (argsNum<3){
        cout<< "smash error: cp: invalid arguments" << endl; //TODO: WHAT IF THIS HAPPENS? PERROR?
        return;
    }
    char buf[BUF_SIZE];
    char pwd[256];
    char *readfile=args[1];
    char* writefile= args[2];
    int fdread=open(readfile,O_RDONLY);
    int fdwrite=open(writefile, O_WRONLY|O_CREAT|O_TRUNC, 0666 );
    if(fdread==-1 || fdwrite==-1){
        if (fdread!=-1)
            close(fdread);
        if (fdwrite!=-1)
            close(fdwrite);

        perror("smash error: open failed");
        close(fdread);
        return;
    }

    ssize_t numRead;

    while ((numRead = read(fdread, buf, BUF_SIZE)) > 0)
        if (write(fdwrite, buf, numRead) != numRead) {
            perror("smash error: write failed");
            close(fdread);
            close(fdwrite);
            return;
        }

    if (numRead<0)
        perror("smash error: read failed");

    close(fdread);
    close(fdwrite);
}

int CommandHistoryEntry::compareCommand(const char *comm) {
    return strcmp(comm, command);
}

ostream &operator<<(ostream &os, const CommandHistoryEntry &dt) {
    os << right << setw(5) << dt.seqNum << " " << dt.command << endl;
    return os;
}


JobEntry::JobEntry(int PID, int SeqID, const char* command):jobPID(PID), jobSeqID(SeqID), jobcommand(command),status(BACKGROUND_JOB){
    /*  jobcommand = new char[(strlen(command))+1];
      strcpy(jobcommand, command);
      */
    jobAddingTime=time(NULL);
}

JobEntry::~JobEntry(){
    // delete[](jobcommand);
}

JobStatus JobEntry::getJobStatus(){
    return status;
}

std::ostream& operator<<(std::ostream& os, const JobEntry& jobentry){
    double secondsElapsed = difftime(time(NULL), jobentry.jobAddingTime);

    if (jobentry.status == STOPPED_JOB){

        std::cout << "["<<jobentry.jobSeqID << "] " << jobentry.jobcommand << " : " << jobentry.jobPID << " " << secondsElapsed << "(stopped)" << std::endl;
    }
    else{
        std::cout << "["<<jobentry.jobSeqID << "] " << jobentry.jobcommand << " : " << jobentry.jobPID << " " << secondsElapsed << std::endl;
    }
    return os;
}