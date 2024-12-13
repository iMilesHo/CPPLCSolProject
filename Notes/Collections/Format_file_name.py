import os
import re

def rename_markdown_files(root_dir):
    # Regex to match and extract the number and title from the first line
    pattern = re.compile(r'^## (\d+)\. (.+)$')
    
    for dirpath, dirnames, filenames in os.walk(root_dir):
        for filename in filenames:
            # Check if the file might be a text file (does not have a specific extension)
            if '.' not in filename or filename.endswith('.md'):
                filepath = os.path.join(dirpath, filename)
                try:
                    with open(filepath, 'r', encoding='utf-8') as file:
                        first_line = file.readline().strip()
                        match = pattern.match(first_line)
                        if match:
                            # Construct the new filename
                            problem_number = match.group(1)
                            problem_title = match.group(2).replace(' ', '_')
                            new_filename = f"LC{problem_number}_{problem_title}.md"
                            new_filepath = os.path.join(dirpath, new_filename)
                            # Rename the file
                            os.rename(filepath, new_filepath)
                            print(f"Renamed '{filename}' to '{new_filename}'")
                except Exception as e:
                    print(f"Error processing {filename}: {e}")

# Example usage
root_directory = './'
rename_markdown_files(root_directory)
