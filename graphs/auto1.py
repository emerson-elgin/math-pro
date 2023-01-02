import os
import subprocess
from datetime import datetime, timedelta
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
from tkcalendar import Calendar
import random


def make_commit(date, commit_message):
    try:
        # Adjust the date to one year ago
        commit_date = datetime.strptime(date, "%Y-%m-%d") - timedelta(days=365)
        adjusted_date = commit_date.strftime("%Y-%m-%d")

        # Generate a random time
        random_commit_time = random_time()

        # Combine adjusted date and random time
        full_datetime = f"{adjusted_date} {random_commit_time}"

        # Set the environment variable for the commit date
        os.environ['GIT_AUTHOR_DATE'] = full_datetime
        os.environ['GIT_COMMITTER_DATE'] = full_datetime

        # Pull the latest changes from the remote repository to avoid conflicts
        subprocess.run(['git', 'pull', 'origin', 'main'], check=True)

        # Add all changes to the commit
        subprocess.run(['git', 'add', '.'], check=True)

        # Run the Git commit command
        subprocess.run(['git', 'commit', '-m', commit_message], check=True)

        # Push the commit to the main branch on GitHub
        subprocess.run(['git', 'push', 'origin', 'main'], check=True)

        # Show success message
        messagebox.showinfo("Success", f"Commit made with date {full_datetime} and message '{commit_message}'")

    except subprocess.CalledProcessError as e:
        messagebox.showerror("Error", f"An error occurred: {e}")


def random_time():
    """Generate a random time in HH:MM:SS format."""
    hour = random.randint(0, 23)
    minute = random.randint(0, 59)
    second = random.randint(0, 59)
    return f"{hour:02}:{minute:02}:{second:02}"


def commit_with_gui():
    def submit_commit():
        # Get the selected date
        date = calendar.get_date()

        # Get the commit message
        commit_message = message_entry.get()

        if not commit_message:
            messagebox.showerror("Error", "Commit message cannot be empty")
            return

        # Call the make_commit function
        make_commit(date, commit_message)

    # Create the main Tkinter window
    root = tk.Tk()
    root.title("Git Commit GUI")

    # Date selection with calendar
    tk.Label(root, text="Select Date:").grid(row=0, column=0, padx=10, pady=5, sticky="w")
    calendar = Calendar(root, date_pattern="yyyy-mm-dd")
    calendar.grid(row=0, column=1, padx=10, pady=5)

    # Commit message input
    tk.Label(root, text="Commit Message:").grid(row=1, column=0, padx=10, pady=5, sticky="w")
    message_entry = ttk.Entry(root, width=40)
    message_entry.grid(row=1, column=1, padx=10, pady=5)

    # Submit button
    submit_button = ttk.Button(root, text="Commit", command=submit_commit)
    submit_button.grid(row=2, column=0, columnspan=2, pady=10)

    # Start the Tkinter event loop
    root.mainloop()


# Run the GUI
commit_with_gui()
