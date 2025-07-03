#ifndef TPCOMMAND_H
#define TPCOMMAND_H

#include "CommandBase.h"
#include <vector>
#include <string>
#include "../../../Client.h"


class TpCommand : public CommandBase {
public:
    TpCommand(); // Constructor

    // Command execution function
    bool execute(const std::vector<std::string>& args) override;
};

#endif // TPCOMMAND_H
