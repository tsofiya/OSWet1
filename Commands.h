
#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <cstring>
#include "BidirectionalList.h"

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define HISTORY_MAX_RECORDS (50)

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

typedef enum {
    BACKGROUND_JOB,
    STOPPED_JOB
}JobStatus;

class Command {
protected:
    char ** args;
    char* line;
    int argsNum;
    bool isBackGround;
    int currentFgPID=-1;
public:
    Command(const char* cmd_line);
    virtual ~Command();
    virtual void execute() = 0;
    //virtual void prepare();
    //virtual void cleanup();
    // TODO: Add your extra methods if needed
    int getCurrFgPID();
    char * getLine();
};


class JobEntry {
    // TODO: Add your data members
private:
    int jobPID;
    int jobSeqID;
    std::string jobcommand;
    time_t jobAddingTime;
    JobStatus status;

public:
    JobEntry(int PID, int SeqID, const char* command, JobStatus s);
    ~JobEntry();
    void JobSetStatus(JobStatus s){
        status=s;
    }
    int getJobPID(){
        return jobPID;
    }

    JobStatus getJobStatus();
    int getJobSeqID();
    std::string getJobCommandLine();

    friend std::ostream& operator<<(std::ostream& os, const JobEntry& jobentry);

};

class JobsList {


private:
    std::vector<JobEntry> list;
    int JobsNum;
    int PIDS[101];


    // TODO: Add your data members
public:
    JobsList();
    ~JobsList();
    int findPID(int pid);
    void addBack(int pid, char* cmd, JobStatus isStopped, int seqID);
    int getMaxJobID();
    void addJob(int pid, char* cmd, bool isStopped); //used to receive Command* cmd instead of char* cmd, but command is a virtual class...
    void printJobsList();
    void killAllJobs();
    JobEntry* getJobBySeqID (int sID);
    JobEntry * getJobById(int jobId);
    bool removeJobById(int jobId);
    bool removeStoppedJobByID(int jobId);
    void removeFinishedJobs();
    JobEntry * getLastJob(int* lastJobId);
    JobEntry *getLastStoppedJob(int *jobId);

    void printPIDS(){
        for (int i=0; i<101;i++){
            std::cout << PIDS[i] << std::endl;
        }
    }
    // TODO: Add extra methods or modify exisitng ones as needed
};


class BuiltInCommand : public Command {
public:
    BuiltInCommand(const char* cmd_line);
    virtual ~BuiltInCommand() {}
};

class ExternalCommand : public Command {
private:
    bool isBashCommand;
    JobsList * jobs;
public:
    ExternalCommand(const char* cmd_line, JobsList * jobs);
    virtual ~ExternalCommand() {}
    void execute() override;
};

class PipeCommand : public Command {
    // TODO: Add your data members
public:
    PipeCommand(const char* cmd_line);
    virtual ~PipeCommand() {}
    void execute() override;
};

class RedirectionCommand : public Command {
    // TODO: Add your data members
public:
    explicit RedirectionCommand(const char* cmd_line);
    virtual ~RedirectionCommand() {}
    void execute() override;
    //void prepare() override;
    //void cleanup() override;
};

class ChangeDirCommand : public BuiltInCommand {

private:
    char** plastPwd;

public:
// TODO: Add your data members public:
    ChangeDirCommand(const char* cmd_line, char** plastPwd);
    virtual ~ChangeDirCommand();
    void execute() override;
    void setPrevDir(char* d);
};

class GetCurrDirCommand : public BuiltInCommand {
public:
    GetCurrDirCommand(const char* cmd_line);
    virtual ~GetCurrDirCommand();
    void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
public:
    ShowPidCommand(const char* cmd_line);
    virtual ~ShowPidCommand() ;
    void execute() override;
};



class JobsList;
class QuitCommand : public BuiltInCommand {
private:
    JobsList *jobs;
// TODO: Add your data members
protected:
public:
    QuitCommand(const char* cmd_line, JobsList* jobs);
    virtual ~QuitCommand() {}
    void execute() override;
};

class CommandHistoryEntry{
private:
    int seqNum;
    char* command;
public:
    CommandHistoryEntry(int seq, const char *comm);
    ~CommandHistoryEntry();
    void repeatCommand();

    int compareCommand(const char* comm);

    friend std::ostream& operator<<(std::ostream& os, const CommandHistoryEntry& dt);

};

class CommandsHistory {
private:
    CommandHistoryEntry* history[50];
    int top=49;
    int capcitcy=0;
    int seq=1;
protected:

    // TODO: Add your data members
public:
    CommandsHistory();
    ~CommandsHistory();
    void addRecord(const char* cmd_line);
    void printHistory();
};

class HistoryCommand : public BuiltInCommand {
private:
    CommandsHistory *history;
    // TODO: Add your data members
public:
    ~HistoryCommand();
    HistoryCommand(const char* cmd_line, CommandsHistory* history);
    void execute() override;
};



class JobsCommand : public BuiltInCommand {

private:
    JobsList *jobs;
    // TODO: Add your data members
public:
    JobsCommand(const char* cmd_line, JobsList* jobs);
    virtual ~JobsCommand() ;
    void execute() override;
};

class KillCommand : public BuiltInCommand {
private:
    JobsList *jobs;
    // TODO: Add your data members
public:
    KillCommand(const char* cmd_line, JobsList* jobs);
    virtual ~KillCommand();
    void execute() override;
};

class ForegroundCommand : public BuiltInCommand {
private:
    JobsList *jobs;
    // TODO: Add your data members
public:
    ForegroundCommand(const char* cmd_line, JobsList* jobs);
    virtual ~ForegroundCommand();
    void execute() override;
};


class BackgroundCommand : public BuiltInCommand {
private:
    JobsList *jobs;
    // TODO: Add your data members
public:
    BackgroundCommand(const char* cmd_line, JobsList* jobs);
    virtual ~BackgroundCommand();
    void execute() override;
};

class CopyCommand : public BuiltInCommand {
public:
    CopyCommand(const char* cmd_line);
    virtual ~CopyCommand() ;
    void execute() override;
};


class SmallShell {
private:
    CommandsHistory history;
    JobsList jobs;
    char* plastPwd;
    Command* currCommand;
    // TODO: Add your data members
    SmallShell();
public:
    Command *CreateCommand(const char* cmd_line);
    SmallShell(SmallShell const&)      = delete; // disable copy ctor
    void operator=(SmallShell const&)  = delete; // disable = operator
    static SmallShell& getInstance() // make SmallShell singleton
    {
        static SmallShell instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    ~SmallShell();
    void executeCommand(const char* cmd_line);
    void addStoppedJob(int pid);
    // TODO: add extra methods as needed
    int getCurrFg();
};

bool _isBackgroundComamnd(const char* cmd_line);
void _removeBackgroundSign(char* cmd_line);

#endif //SMASH_COMMAND_H_