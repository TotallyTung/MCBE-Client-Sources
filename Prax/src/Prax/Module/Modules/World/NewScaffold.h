//
// Created by raine on 4/19/2024.
//

#ifndef PRAXINTERNAL_NEWSCAFFOLD_H
#define PRAXINTERNAL_NEWSCAFFOLD_H

#include <Prax/Module/Module.h>

class NewScaffold : public ModuleBase<NewScaffold> {
public:
    NewScaffold() : ModuleBase("NewScaffold", "Attempted rewrite of scaffold.", "World", 0, false) {

    }


};


#endif //PRAXINTERNAL_NEWSCAFFOLD_H
