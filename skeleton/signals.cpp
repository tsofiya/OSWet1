#include <iostream>
#include <signal.h>
#include "signals.h"
#include "Commands.h"

using namespace std;

void ctrlZHandler(int sig_num) {
    // TODO: Add your implementation
    SmallShell& smash = SmallShell::getInstance();
    int fg= smash.getCurrFg();
    if (fg!=-1){
        kill(fg, SIGSTOP);
    }


}

void ctrlCHandler(int sig_num) {
  // TODO: Add your implementation
    SmallShell& smash = SmallShell::getInstance();
    int fg= smash.getCurrFg();
    if (fg!=-1)
        kill(fg, SIGKILL);
}
