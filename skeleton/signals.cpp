#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"

using namespace std;

void ctrlZHandler(int sig_num) {
    // TODO: Add your implementation
    std::cout<<"smash: got ctrl-Z"<< std::endl;
    SmallShell& smash = SmallShell::getInstance();
    int fg= smash.getCurrFg();
    //std::cout
    if (fg!=-1){
        smash.addStoppedJob(fg);
        kill(fg, SIGSTOP);
        std::cout<< "smash: process " << fg << " was stopped" << std::endl;
    }


}

void ctrlCHandler(int sig_num) {
    // TODO: Add your implementation
    std::cout<<"smash: got ctrl-C"<< std::endl;
    SmallShell& smash = SmallShell::getInstance();
    int fg= smash.getCurrFg();
    if (fg!=-1) {
        kill(fg, SIGKILL);
        std::cout << "smash: process " << fg << " was killed" << std::endl;
    }
}