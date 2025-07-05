//
// Created by vastrakai on 10/5/2023.
//

#include <Prax/Module/Modules/Misc/AutoQueue.h>
#include "Queue.h"

void Queue::OnRun(std::vector<std::string> args) {
    AutoQueue::QueueForGame();
}
