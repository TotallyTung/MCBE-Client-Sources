//
// Created by vastrakai on 9/7/2023.
//

#include "CheckUser.h"

void CheckUser::OnRun(std::vector<std::string> args) {

    // Check the arg count
    if (args.size() != 1) {
        AddCommandMessage("Usage: .checkuser <username>");
        return;
    }

    // Concatenate all the args into one string
    std::string username;
    for (auto &arg : args) {
        username += arg + " ";
    }

    // Remove the last space
    username.pop_back();

    // Send the request
    int statusCode = Internet::CheckUser(username);

    // Check the result
    if (statusCode == 200) {
        AddCommandMessage(username + " is a valid xbox live account");
    } else {
        AddCommandMessage(username + " is not a valid xbox live account (status code " + std::to_string(statusCode) + ")");
    }

}
