import re
import sys
import os

# Function for parsing through a source code file and getting 
def parse_state_fields(filename):
    # Regex pattern to match struct definitions or typedefs with 'state' in the name
    pattern = r"(typedef\s+struct\s+[^{]*\{[^}]*\}\s*(\w*state\w*);)"

    # Open the selected source file
    with open(filename, 'r') as file:
        content = file.read()

    # Search for the pattern
    match = re.search(pattern, content, re.DOTALL)
    if match:
        # This will print the first match found
        print("Found datatype with 'state' in its definition:")
        print(match.group())
    else:
        print("No datatype with 'state' found.")

if __name__ == "__main__":

    if (len(sys.argv) <= 1):
        print("Please enter a file name")
    else:
        if (not os.path.isfile(sys.argv[1])):
            print("Error: File does not exist")
            exit(-1)
        
        parse_state_fields(sys.argv[1])
