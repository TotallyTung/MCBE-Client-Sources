import os
import re

# Define the base directory for your project
BASE_DIR = r"MelodyV2"

# Define the directories for modules, commands, and hooks
MODULE_DIRS = {
    "Combat": os.path.join(BASE_DIR, "Client", "ModuleManager", "Modules", "Combat"),
    "Misc": os.path.join(BASE_DIR, "Client", "ModuleManager", "Modules", "Misc"),
    "Render": os.path.join(BASE_DIR, "Client", "ModuleManager", "Modules", "Render"),
    "Movement": os.path.join(BASE_DIR, "Client", "ModuleManager", "Modules", "Movement"),
    "Player": os.path.join(BASE_DIR, "Client", "ModuleManager", "Modules", "Player"),
    "Client": os.path.join(BASE_DIR, "Client", "ModuleManager", "Modules", "Client"),
}
COMMAND_DIR = os.path.join(BASE_DIR, "Client", "CommandManager", "Commands")
HOOK_DIR = os.path.join(BASE_DIR, "Client", "HookManager", "Hooks")

# Template for module header (.h) file
MODULE_H_TEMPLATE = """
#pragma once
#include "../Module.h"

class {class_name} : public Module {{
private:
    // Add private members here

public:
    {class_name}();
    
    // Add public methods here
    
    virtual void onEnable() override;
    virtual void onDisable() override;
    virtual void onNormalTick(Actor* actor) override;
}};
"""

# Template for module implementation (.cpp) file
MODULE_CPP_TEMPLATE = """
#include "{class_name}.h"
#include "../../../Client.h"

{class_name}::{class_name}() : Module("{module_name}", "Description here", Category::{category}, {key}) {{
    // Add settings here
}}

void {class_name}::onEnable() {{
    // Add enable logic here
}}

void {class_name}::onDisable() {{
    // Add disable logic here
}}

void {class_name}::onNormalTick(Actor* actor) {{
    // Add tick logic here
}}
"""

# Template for command header (.h) file
COMMAND_H_TEMPLATE = """
#pragma once
#include "../../Command.h"

class {class_name} : public Command {{
public:
    {class_name}();
    virtual void execute(const std::vector<std::string>& args) override;
}};
"""

# Template for command implementation (.cpp) file
COMMAND_CPP_TEMPLATE = """
#include "{class_name}.h"
#include "../../../../Client.h"

{class_name}::{class_name}() : Command("{command_name}", "Description here", {{"alias1", "alias2"}}, "<args>") {{}}

void {class_name}::execute(const std::vector<std::string>& args) {{
    // Add command execution logic here
}}
"""

# Template for hook header (.h) file
HOOK_H_TEMPLATE = """
#pragma once
#include "../../../../Utils/MemoryUtils.h"

class {class_name} {{
protected:
    using func_t = void(__fastcall*)(/* Add parameters here */);
    static inline func_t func;

    static void hookFunction(/* Add parameters here */) {{
        // Add hook logic here
    }}

public:
    static void init(uintptr_t address) {{
        MemoryUtils::CreateHook("{hook_name}", address, (void*)&{class_name}::hookFunction, (void*)&func);
    }}
}};
"""

def create_module():
    print("Creating a new module...")
    module_name = input("Enter module name: ")
    class_name = module_name.replace(" ", "")
    category = input("Enter module category (Combat/Misc/Render/Movement/Player/Client): ")
    key = input("Enter module key (e.g., VK_F): ")

    if category not in MODULE_DIRS:
        print("Invalid category. Please choose from: Combat, Misc, Render, Movement, Player, Client")
        return

    directory = MODULE_DIRS[category]
    h_file_path = os.path.join(directory, f"{class_name}.h")
    cpp_file_path = os.path.join(directory, f"{class_name}.cpp")

    with open(h_file_path, "w") as h_file:
        h_file.write(MODULE_H_TEMPLATE.format(class_name=class_name))

    with open(cpp_file_path, "w") as cpp_file:
        cpp_file.write(MODULE_CPP_TEMPLATE.format(class_name=class_name, module_name=module_name, category=category, key=key))

    print(f"Module '{module_name}' created successfully.")

def create_command():
    print("Creating a new command...")
    command_name = input("Enter command name: ")
    class_name = command_name.capitalize() + "Command"

    # Create a new directory for the command
    command_dir = os.path.join(COMMAND_DIR, command_name)
    os.makedirs(command_dir, exist_ok=True)

    h_file_path = os.path.join(command_dir, f"{class_name}.h")
    cpp_file_path = os.path.join(command_dir, f"{class_name}.cpp")

    with open(h_file_path, "w") as h_file:
        h_file.write(COMMAND_H_TEMPLATE.format(class_name=class_name))

    with open(cpp_file_path, "w") as cpp_file:
        cpp_file.write(COMMAND_CPP_TEMPLATE.format(class_name=class_name, command_name=command_name))

    print(f"Command '{command_name}' created successfully in its own directory.")

def create_hook():
    print("Creating a new hook...")
    hook_name = input("Enter hook name: ")
    class_name = hook_name + "Hook"

    directory = HOOK_DIR
    h_file_path = os.path.join(directory, f"{class_name}.h")

    with open(h_file_path, "w") as h_file:
        h_file.write(HOOK_H_TEMPLATE.format(class_name=class_name, hook_name=hook_name))

    print(f"Hook '{hook_name}' created successfully.")

def main():
    while True:
        print("\nWhat would you like to create?")
        print("1. Module")
        print("2. Command")
        print("3. Hook")
        print("4. Exit")
        
        choice = input("Enter your choice (1-4): ")
        
        if choice == "1":
            create_module()
        elif choice == "2":
            create_command()
        elif choice == "3":
            create_hook()
        elif choice == "4":
            break
        else:
            print("Invalid choice. Please try again.")

if __name__ == "__main__":
    main()