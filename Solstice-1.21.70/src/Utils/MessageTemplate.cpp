//
// Created by vastrakai on 7/19/2024.
//

#include "MessageTemplate.hpp"

#include <fstream>

std::string MessageTemplate::getTemplatePath() const
{
    return FileUtils::getSolsticeDir() + "templates\\" + mTemplateName + ".txt";
}

void MessageTemplate::createTemplateFile(const std::string& defaultMessage) const
{
    if (FileUtils::fileExists(getTemplatePath())) return;

    std::ofstream file(getTemplatePath());
    if (!file.is_open())
    {
        spdlog::error("Failed to create template file: {}", getTemplatePath());
    }

    file << "// This is a template file for the " << mTemplateName << " message.\n";

    if (!mVariableDescriptions.empty())
    {
        file << "// Use the following variables to insert values into the message:\n";

        for (const auto& [variable, description] : mVariableDescriptions)
        {
            file << "// " << variable << " - " << description << "\n";
        }
    }

    file << "// Any lines starting with // will be ignored.\n";
    file << "// If you add more than one entry, one will be randomly selected.\n";

    if (!defaultMessage.empty())
    {
        file << defaultMessage;
    }

    file.close();
}

std::vector<std::string> MessageTemplate::getAllEntries() const
{
    std::ifstream file(getTemplatePath());
    if (!file.is_open())
    {
        spdlog::error("Failed to open template file: {}", getTemplatePath());
        return {};
    }

    std::vector<std::string> entries;
    std::string line;
    while (std::getline(file, line))
    {
        // Continue if the line is empty OR if it starts with a comment
        if (line.empty() || line.starts_with("//")) continue;

        entries.push_back(line);
    }

    return entries;
}

std::string MessageTemplate::getEntry() {
    const auto entries = getAllEntries();
    if (entries.empty()) return "";

    auto entry = entries[MathUtils::random(0, entries.size() - 1)];
    for (const auto& [variable, value] : mVariables)
    {
        entry = StringUtils::replaceAll(entry, variable, value);
    }

    return entry;
}

void MessageTemplate::defineVariable(const std::string& variableName, const std::string& variableValue)
{
    mVariables["!" + variableName + "!"] = variableValue;
}

void MessageTemplate::setVariableValue(const std::string& variableName, const std::string& variableValue)
{
    mVariables["!" + variableName + "!"] = variableValue;
}
