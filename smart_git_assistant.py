import subprocess

def run_git_command(cmd):
    result = subprocess.run(cmd, capture_output=True, text=True)
    print(result.stdout)
    if result.stderr:
        print(f"Error: {result.stderr}")
    return result

def main():
    # Checks if there are any changes in the repository
    print("Checking for changes...")
    run_git_command(["git", "status"])

    # Asks the user if they want to add all changes
    add_all = input("\nShould I stage all changes? (y/n): ").lower()
    if add_all == 'y':
        run_git_command(["git", "add", "."])
    else:
        print("Aborting. No changes staged.")
        return
    
    # Asks the user for a commit message
    print("Suggested commit message: 'Update: general changes'")
    commit_message = input("Press Enter to accept, or type your own message: \n")
    if not commit_message.strip():
        commit_message = "Update: general changes"
    

    # Final confirmation
    print("\n Final summary:")
    run_git_command(["git", "diff", "--cached", "--name-only"])
    print(f"Commit Message: {commit_message}")

    confirm = input("\nShould I procced with the commit and push? (y/n): ").lower()
    if confirm == "y":
        run_git_command(["git", "commit", "-m", commit_message])
        run_git_command(["git", "push"])
        print("\n✅ Done! Changes committed and pushed.")

if __name__ == "__main__":
    main()