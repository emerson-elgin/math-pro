import os
import subprocess
from datetime import datetime
import tkinter as tk
from tkinter import ttk
from tkinter import messagebox


def make_commit(date, commit_message):
    try:
        # Set the environment variable for the commit date
        os.environ['GIT_AUTHOR_DATE'] = date
        os.environ['GIT_COMMITTER_DATE'] = date

        # Pull the latest changes from the remote repository to avoid conflicts
        subprocess.run(['git', 'pull', 'origin', 'main'], check=True)

        # Add all changes to the commit
        subprocess.run(['git', 'add', '.'], check=True)

        # Run the Git commit command
        subprocess.run(['git', 'commit', '-m', commit_message], check=True)

        # Push the commit to the main branch on GitHub
        subprocess.run(['git', 'push', 'origin', 'main'], check=True)

        # Show success message
        messagebox.showinfo("Success", f"Commit made with date {date} and message '{commit_message}'")

    except subprocess.CalledProcessError as e:
        messagebox.showerror("Error", f"An error occurred: {e}")


def commit_with_gui():
    def submit_commit():
        # Get the date, time, and message from the input fields
        date = f"{date_entry.get()} {time_entry.get()}"
        commit_message = message_entry.get()

        # Validate the date and time format
        try:
            datetime.strptime(date, '%Y-%m-%d %H:%M:%S')
        except ValueError:
            messagebox.showerror("Error", "Invalid date format. Use YYYY-MM-DD HH:MM:SS")
            return

        if not commit_message:
            messagebox.showerror("Error", "Commit message cannot be empty")
            return

        # Call the make_commit function
        make_commit(date, commit_message)

    # Create the main Tkinter window
    root = tk.Tk()
    root.title("Git Commit GUI")

    # Date input
    tk.Label(root, text="Date (YYYY-MM-DD):").grid(row=0, column=0, padx=10, pady=5, sticky="w")
    date_entry = ttk.Entry(root)
    date_entry.grid(row=0, column=1, padx=10, pady=5)

    # Time input
    tk.Label(root, text="Time (HH:MM:SS):").grid(row=1, column=0, padx=10, pady=5, sticky="w")
    time_entry = ttk.Entry(root)
    time_entry.grid(row=1, column=1, padx=10, pady=5)

    # Commit message input
    tk.Label(root, text="Commit Message:").grid(row=2, column=0, padx=10, pady=5, sticky="w")
    message_entry = ttk.Entry(root, width=40)
    message_entry.grid(row=2, column=1, padx=10, pady=5)

    # Submit button
    submit_button = ttk.Button(root, text="Commit", command=submit_commit)
    submit_button.grid(row=3, column=0, columnspan=2, pady=10)

    # Start the Tkinter event loop
    root.mainloop()


# Run the GUI
commit_with_gui()
