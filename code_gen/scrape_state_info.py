import re
import sys
import os
import glob

# Define the global dict to track state structs already defined
# (Set as a global variable because sometimes the state struct is defined in a separate file from the vmsd)
found_states = {}

# Define the global dict to track VMSD information already definted
## NOT FULLY IMPLEMENTED: Currently, just used to track 

# Function for parsing through a source code file and getting 
def parse_state_fields(filename):

    # Regex patterns to match struct definitions or typedefs with 'state' in the name, end of struct, fields, etc.
    pattern = re.compile(r'^\s*(?:typedef\s*)?struct\s+(\w*state\w*)\s*\{\s*$', re.IGNORECASE)
    struct_pattern = re.compile(r"^\s*(?:typedef\s*)?struct\s+\{")
    field_pattern = re.compile(r"^\s*([\w\s]+\w)\s+(\*?\s*[\w\[\]]+)\s*;")
    field_size_pattern = re.compile(r"(\w+)(?:\[(\d+|0x[0-9a-fA-F]+|\w+)\])?")
    end_struct_pattern = re.compile(r"^\s*\}\s*(\w+)?\s*;")
    comment_pattern = re.compile(r"//|/\*|\*/")
    
    try:
        # Open the selected source file
        with open(filename, 'r') as file:
            content = file.readlines()
        
    except Exception as e:
        # print("ERROR: Failed to read file")
        return 0

    current_struct = None
    state_fields = {'fields': [], 'nested': []}
    num_nested = 0
    
    # Search for the first instance of a struct definition with the name 'state' in it
    # State definition in examples thus far has been the first one defined
    found_state = False
    for line in content:

        # Check if the initial state value has been found yet
        if (not found_state):
            if pattern.match(line):
                state_name = pattern.search(line).group(1)
                # print("State struct found")
                found_state = True
            # For cases where name of struct not given till the end, need to check if it's a state struct later
            elif struct_pattern.match(line):
                state_name = None
                found_state = True
        # If the state struct has been found, then parse through each field until the end
        else:
            # If current line is a #define, then ignore it and just continue


            # Filter out any inline comments and newlines
            line = re.sub(r"//.*", "", line).strip()
            line = re.sub(r"/\*.*?\*/", "", line, flags=re.DOTALL)

            # Check if the current line matches any of the following patterns

            # Case 1: Entering a new struct definition
            if (struct_pattern.search(line)):

                # Append new struct to nested structs and set as current struct
                # print("Entering struct")
                state_fields['nested'].append({'name': None, 'fields': []})
                current_struct = num_nested

            # Case 2: Leaving struct, save the name and continue
            elif (end_struct_pattern.search(line)):
                
                # If currently in a nested structure
                if current_struct is not None:
                    # Set the name of the current nested structure and exit structure
                    state_fields['nested'][num_nested]['name'] = end_struct_pattern.search(line).group(1)
                    num_nested += 1
                    current_struct = None
                    # print("Exiting struct", state_fields['nested'][num_nested-1]['name'])
                # If not in a nested structure, reached the end, so exit
                else:
                    # If already found state name (know that it's a state struct), exit
                    if (state_name):
                        # print("Exiting struct")
                        break
                    else:

                        struct_name = end_struct_pattern.search(line).group(1)
                        
                        # If find that current struct was NOT a state struct, reset state_info
                        if (not struct_name or "state" not in struct_name.lower()):
                            state_fields = {'fields': [], 'nested': []}
                            found_state = False
                            current_struct = None
                            num_nested = 0
                        # If find that current struct was a state struct, exit
                        else:
                            state_name = struct_name
                            break
                            

            # Case 3: Datatype field => store info (datatype, name, size, if it's a pointer)
            elif (field_pattern.search(line)):
                field_info = {}

                datatype, field_name = field_pattern.search(line).groups()
                field = field_size_pattern.search(field_name)

                field_info['datatype'] = datatype
                field_info['name'] = field.group(1)
                field_info['size'] = field.group(2) if field.group(2) else '1'

                if str(field_info['size']) != '1' or '*' in line:
                    field_info['ptr'] = True
                else:
                    field_info['ptr'] = False

                # If currently in a nested structure, append to that struct's 
                if current_struct is None:
                    state_fields['fields'].append(field_info)
                else:
                    state_fields['nested'][current_struct]['fields'].append(field_info)
                # print(field_info)

    # Update global struct with information based on state_name
    if (found_state and not found_states.get(state_name)):
        found_states[state_name] = state_fields
        # Only return 1 when find a unique state struct
        return 1
    # Return 2 when already found the state struct
    elif (found_state):
        return 2
    else:
        return 0


# Test function to see if file includes a vmstatedescription
def find_vmsd(filename):
    vmsd_pattern = re.compile(r'^\s*(?:static\s*)?const VMStateDescription.*=.*\{')
    end_struct_pattern = re.compile(r"^\s*\};.*")
    vmsd_presave_pattern = re.compile(r'^\s*\.pre_save\s*=\s*')
    vmsd_postload_pattern = re.compile(r'^\s*\.post_load\s*=\s*')
    vmsd_preload_pattern = re.compile(r'^\s*\.pre_load\s*=\s*')
    
    try:
        # Open the selected source file
        with open(filename, 'r') as file:
            content = file.readlines()
        
    except Exception as e:
        # print("ERROR: Failed to read file")
        return 0

    # TO BE ADDED: Parse the VMSD for information (i.e. if it has defined pre_save, post_load, pre_load functions)
    found_vmsd = False
    vmsd_info = {}
    for line in content:
        if not found_vmsd:
            if vmsd_pattern.match(line):
                found_vmsd = True
        else:
            if end_struct_pattern.match(line):
                return 1

    return 0

# cpu-target.c : State struct defined outside of file
# test-vmstate.c : Test file with no real defined state struct

# Recursively search inputted directory for C and H source files to run parse_state_fields on
def find_source_files(directory):

    c_files = glob.glob(directory + "/**/*.c", recursive=True)
    h_files = glob.glob(directory + "/**/*.h", recursive=True)
    files = c_files + h_files
    vmsd_count = 0
    state_struct_count = 0
    diff_file_def_count = 0
    for file in files:
        found_vmsd = find_vmsd(file)
        found_struct = parse_state_fields(file)
        if (found_vmsd and not found_struct):
            print(file)
            diff_file_def_count += 1
        
        if (found_struct == 1):
            state_struct_count += 1
        if (found_vmsd):
            vmsd_count += 1
         
    print("Number of files defining VMStateDescription found:", vmsd_count)
    print("Number of files defining state struct found:", state_struct_count)
    print("Number of files with VMSD and state struct on different files", diff_file_def_count)

if __name__ == "__main__":

    if (len(sys.argv) <= 1):
        print("Please enter a file name")
    else:
        find_source_files(sys.argv[1])
        exit(-1)
        if (not os.path.isfile(sys.argv[1])):
            print("Error: File does not exist")
            exit(-1)
        
        parse_state_fields(sys.argv[1])
