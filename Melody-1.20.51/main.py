import os
import datetime

def get_subdirectories(folder_path):
    return [d for d in os.listdir(folder_path) if os.path.isdir(os.path.join(folder_path, d))]

def blacklist_directories(subdirectories):
    print("Available directories:")
    for i, dir_name in enumerate(subdirectories, 1):
        print(f"{i}: {dir_name}")
    
    blacklist_input = input("Enter the numbers of the directories to blacklist (space-separated), or press Enter to skip: ")
    if not blacklist_input.strip():
        return []
    
    blacklist_numbers = [int(x) for x in blacklist_input.split()]
    return [subdirectories[i-1] for i in blacklist_numbers if 1 <= i <= len(subdirectories)]

def add_credits(folder_path, author_name, blacklist):
    current_time = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    
    for root, dirs, files in os.walk(folder_path):
        # Remove blacklisted directories from dirs to prevent walking into them
        dirs[:] = [d for d in dirs if d not in blacklist]
        
        for filename in files:
            if filename.endswith(('.h', '.cpp')):
                file_path = os.path.join(root, filename)
                
                with open(file_path, 'r') as file:
                    content = file.read()
                
                credit_line = f"// Created by {author_name} on {current_time}\n\n"
                
                if not content.startswith(credit_line):
                    new_content = credit_line + content
                    
                    with open(file_path, 'w') as file:
                        file.write(new_content)
                    
                    print(f"Added credits to {file_path}")
                else:
                    print(f"Credits already exist in {file_path}")

# Get the folder path from the user
folder_path = input("Enter the path to the folder you want to process: ")

# Get subdirectories
subdirectories = get_subdirectories(folder_path)

# Get blacklist from user
blacklist = blacklist_directories(subdirectories)

# Get author name
author_name = input("Enter your name: ")

# Run the script
add_credits(folder_path, author_name, blacklist)