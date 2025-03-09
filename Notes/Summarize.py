# Load JSON data from a file
import json

with open('Top150Leetcode.json', 'r') as f:
    data = json.load(f)

# Summarize each group in the data
for group in data:
    analyze_group = group

    group_name = group.get("name", group.get("slug"))
    group_questions = group.get("questions", [])

    question_count = len(group_questions := group['questions'])
    difficulty_count = {'EASY': 0, 'MEDIUM': 0, 'HARD': 0}
    topic_counts = {}

    for question in group_questions:
        difficulty_count[question["difficulty"]] += 1

        for tag in question["topicTags"]:
            tag_name = tag["name"]
            topic_counts[tag_name] = topic_counts.get(tag_name, 0) + 1

    print(f"\n=== {group.get('name', group.get('slug'))} ===")
    print(f"Total questions: {len(group_questions)}")
    print(f"Difficulty: {difficulty_count}")

    print("Top Tags:")
    for tag, count in sorted(topic_counts.items(), key=lambda item: item[1], reverse=True):
        print(f"- {tag}: {count}")
