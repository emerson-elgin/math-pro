import os
import subprocess
from datetime import datetime

def make_commit(date, commit_message):
    # Set the environment variable for the commit date
    os.environ['GIT_AUTHOR_DATE'] = date
    os.environ['GIT_COMMITTER_DATE'] = date

    # Pull the latest changes from the remote repository to avoid conflicts
    subprocess.run(['git', 'pull', 'origin', 'main'])

    # Add all changes to the commit
    subprocess.run(['git', 'add', '.'])
    
    # Run the Git commit command
    subprocess.run(['git', 'commit', '-m', commit_message])

    # Push the commit to the main branch on GitHub
    subprocess.run(['git', 'push', 'origin', 'main'])

def get_commit_details():
    # Ask for date and time in a specific format
    commit_date = input("Enter the commit date and time (YYYY-MM-DD HH:MM:SS): ")
    # Ask for commit message
    commit_message = input("Enter the commit message: ")

    # Make sure the date format is correct
    try:
        # Validate the date format
        datetime.strptime(commit_date, '%Y-%m-%d %H:%M:%S')
    except ValueError:
        print("Invalid date format. Please use YYYY-MM-DD HH:MM:SS")
        return

    # Make the commit
    make_commit(commit_date, commit_message)
    print(f"Commit made with date {commit_date} and message '{commit_message}'")

# Run the function
get_commit_details()