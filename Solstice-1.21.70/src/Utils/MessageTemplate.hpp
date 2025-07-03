#pragma once
//
// Created by vastrakai on 7/19/2024.
//


/// <summary>
/// Represents a message template that can be used to generate messages.
/// Can be used for things like vote messages, dimension change messages, etc.
/// </summary>
class MessageTemplate {
public:
    std::string mTemplateName = "";
    std::map<std::string, std::string> mVariables = {};
    std::map<std::string, std::string> mVariableDescriptions = {};

    template <typename... Args>
    explicit MessageTemplate(const std::string& templateName, const std::string defaultMessage, Args... args)
        : mTemplateName(templateName)
    {
        mVariableDescriptions = { args... };
        if (!FileUtils::fileExists(getTemplatePath()))
        {
            // Create the file
            createTemplateFile(defaultMessage);
        }

        spdlog::info("Loaded message template {} with {} variables", templateName, mVariableDescriptions.size());
    }

    void createTemplateFile(const std::string& defaultMessage = "") const;
    [[nodiscard]] std::string getTemplatePath() const;
    [[nodiscard]] std::string getEntry();
    [[nodiscard]] std::vector<std::string> getAllEntries() const;
    [[nodiscard]] std::string getVariableDescription(const std::string& variableName) const;
    [[nodiscard]] std::string getVariableValue(const std::string& variableName) const;
    void defineVariable(const std::string& variableName, const std::string& variableValue);
    void setVariableValue(const std::string& variableName, const std::string& variableValue);
};