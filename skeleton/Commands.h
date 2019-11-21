#ifndef SMASH_COMMAND_H_
#define SMASH_COMMAND_H_

#include <vector>
#include <cstring>

#define COMMAND_ARGS_MAX_LENGTH (200)
#define COMMAND_MAX_ARGS (20)
#define HISTORY_MAX_RECORDS (50)

class Command {
protected:
    char ** args;
    int argsNum;
 public:
  Command(const char* cmd_line);
  virtual ~Command();
  virtual void execute() = 0;
  //virtual void prepare();
  //virtual void cleanup();
  // TODO: Add your extra methods if needed
};

class BuiltInCommand : public Command {
 public:
  BuiltInCommand(const char* cmd_line);
  virtual ~BuiltInCommand() {}
};

class ExternalCommand : public Command {
 public:
  ExternalCommand(const char* cmd_line);
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
// TODO: Add your data members public:
public:
  ChangeDirCommand(const char* cmd_line);
  virtual ~ChangeDirCommand();
  void execute() override;
};

class GetCurrDirCommand : public BuiltInCommand {
 public:
  GetCurrDirCommand(const char *cmd_line);
  virtual ~GetCurrDirCommand();
  void execute() override;
};

class ShowPidCommand : public BuiltInCommand {
 public:
  ShowPidCommand(const char* cmd_line);
  virtual ~ShowPidCommand() {}
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
  virtual ~QuitCommand();
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
        int seqNum;
        char* command;
        CommandHistoryEntry* history[50];
    int top=0;
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

class JobsList {
 public:
  class JobEntry {

      int jobPID;
      int jobSeqID;
      char* jobcommand;
      int seconds;
  public:
      JobEntry(int jobPID, int jobSeqID, const char* jobcomm){
        this->jobPID=jobPID;
        this->jobSeqID=jobSeqID;
        /*
        this->jobcommand= (char*)malloc(strlen(jobcomm)+1);
        if(!jobcommand){
          ; //???????????
        }
         */
        this->seconds=0; //not sure this should be initiated here...
      }

      int getPID(){
          return jobPID;
      }

   // TODO: Add your data members
  };
 // TODO: Add your data members
 public:
  JobsList();
  ~JobsList();
  void addJob(Command* cmd,int pid, bool isStopped = false);
  void printJobsList();
  void killAllJobs();
  void removeFinishedJobs();
  JobEntry * getJobById(int jobId);
  bool removeJobById(int jobId);
  JobEntry * getLastJob(int* lastJobId);
  JobEntry *getLastStoppedJob(int *jobId);
  // TODO: Add extra methods or modify exisitng ones as needed
};

class JobsCommand : public BuiltInCommand {
 // TODO: Add your data members
 public:
  JobsCommand(const char* cmd_line, JobsList* jobs);
  virtual ~JobsCommand() {}
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
 // TODO: Add your data members
 public:
  BackgroundCommand(const char* cmd_line, JobsList* jobs);
  virtual ~BackgroundCommand() {}
  void execute() override;
};

class CopyCommand : public BuiltInCommand {
 public:
  CopyCommand(const char* cmd_line);
  virtual ~CopyCommand() {}
  void execute() override;
};


class SmallShell {
 private:
    CommandsHistory history;
    JobsList jobs;
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
  // TODO: add extra methods as needed
};

#endif //SMASH_COMMAND_H_
