import os
import json

# Load JSON data from file
with open('Top150Leetcode.json', 'r') as f:
    data = json.load(f)

# Function to create files with standard default content
def create_files(problem_folder, problem_info):
    problem_id = f"{int(problem_info['questionFrontendId']):04d}"

    files = {
        'Solution.cpp': '#include "Solution.h"\n\n// TODO: Implement your solution methods here\n',
        'Solution.h': f"""#ifndef SOLUTION_{problem_id}_H
#define SOLUTION_{problem_id}_H

#include <vector>

class Solution {{
public:
    // TODO: Define your methods here
}};

#endif
""",
        'test.cpp': f"""#include <gtest/gtest.h>
#include \"Solution.h\"

TEST(Problem{problem_info['questionFrontendId']}Test, BasicTest) {{
    Solution sol;
    // TODO: Add your basic tests here
}}
""",
        'notes.md': f"""# {problem_info['title']}

- **ID:** {problem_info['questionFrontendId']}
- **Difficulty:** {problem_info['difficulty']}
- **Topic Tags:** {', '.join([tag['name'] for tag in problem_info['topicTags']])}
- **Link:** [LeetCode Problem](https://leetcode.com/problems/{problem_info['titleSlug']}/description/)

## Problem Description

(TODO: Paste problem description here)

## Approach

(TODO: Outline your approach here)

## Complexity

- **Time Complexity:** (TODO)
- **Space Complexity:** (TODO)
"""
    }

    for filename, content in files.items():
        with open(os.path.join(problem_folder, filename), 'w') as file:
            file.write(content)

# Function to sanitize folder names
def sanitize_name(name):
    return name.replace(" ", "_").replace("/", "_").replace("-", "_")

# Main function to generate directories with order and standard content
def generate_directory_structure(data, root_folder="problems"):
    if not os.path.exists(root_folder):
        os.mkdir(root_folder)

    for i, group in enumerate(data, start=1):
        topic_name = f"{i:02d}_{sanitize_name(group['name'])}"
        topic_folder = os.path.join(root_folder, topic_name)

        if not os.path.exists(topic_folder):
            os.mkdir(topic_folder)

        for j, question in enumerate(group['questions'], start=1):
            question_folder_name = f"{j:02d}_{int(question['questionFrontendId']):04d}_{sanitize_name(question['title'])}"
            question_folder = os.path.join(topic_folder, question_folder_name)

            if not os.path.exists(question_folder):
                os.mkdir(question_folder)

            create_files(question_folder, question)

# Generate the directories
generate_directory_structure(data)

print("Ordered directory structure with standard content generated successfully.")
