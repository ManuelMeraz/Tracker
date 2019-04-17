#! /usr/bin/env python3
import os
import sys
import subprocess
import argparse

project_dir = os.environ['TRACKER_PROJECT']

if not project_dir:
    print("Please source the set_env.bash script in the scripts/ directory to "
          "set the $TRACKER_PROJECT environment variable, which keep track of "
          "the project directory.")
    sys.exit(1)


def cmake_filter(file):
    '''
    @brief Used for checking if a file is a cmake file
    @param file A potential file to be filtered
    @return true if file is a cmake file
    '''
    return 'CMakeLists.txt'in file or file.endswith('.cmake')


def cpp_filter(file):
    '''
    @brief Used for checking if a file is a C++ file
    @param file A potential file to be filtered
    @return true if file is a C++ file
    '''
    cpp_extensions = ['.h', '.hpp', '.hh', '.hxx', '.cc', '.cpp', '.tpp']
    return [extension for extension in cpp_extensions if extension in file]


def get_files_if(file_filter):
    '''
    @brief Searches for all files in tracker project and formats
           them if they pass through the filter

    @param file_filter a function that returns true if file
                       name satisfies the requirements

    @return A Set of full file paths that passed the filter
    '''

    # Don't format the files in these directories
    ignore_dirs = ['extern', 'include', 'scripts', 'build']

    filtered_files = set([])
    for root, dirs, files in os.walk(project_dir):

        # Ignore hidden directories and if it is an ignored directory
        if (os.path.basename(root).startswith('.')
                or [d for d in ignore_dirs if d in root]):
            continue

        # In place modify the directories we will explore
        # same as above, ignore hidden directories and if they're
        # in the ignore list
        dirs[:] = [d for d in dirs if not d.startswith('.')]
        dirs[:] = [d for ignore in ignore_dirs for d in dirs
                   if ignore not in root + '/' + d]

        files[:] = [f for f in files if file_filter(f)]

        for file in files:
            filtered_files.add(root + '/' + file)

    return filtered_files


def execute_command(command, files):
    '''
    @brief execute the following command on the given files
    @param command The following will be a list where the first
                   item in the list is the command, followed by
                   the arguments
    @param files   A list of full paths to files to execute the
                   command on, where the file is the last arg

    Example:
    cmake-format -i /path/to/file
    '''
    print("Finding your " + command[0] + " files to format....")
    # Make sure we don't get any repeats with a set

    print("\nFormatting the following files:")
    for file in files:
        print(file)

        try:
            subprocess.run(command + [file],
                           stdout=subprocess.PIPE,
                           check=True,
                           universal_newlines=True)

        except FileNotFoundError:
            print("\nAttempted to use " + command[0] + " , but it looks like"
                  "it's not installed!\n")


# Grab the tracker project directory path
# if it user called set_env.bash
if __name__ == '__main__':
    parser = argparse.ArgumentParser()

    parser.add_argument("--all",
                        help="Format cmake files, and run both clang-format"
                        "and clang-tidy on files",
                        action="store_true")

    parser.add_argument("-cmake",
                        "--cmake-format",
                        help="Run cmake-format on cmake files in code base",
                        action="store_true")

    parser.add_argument("-cf",
                        "--clang-format",
                        help="Run clang-format on C++ files in code base",
                        action="store_true")

    parser.add_argument("-ct",
                        "--clang-tidy",
                        help="Run clang-tidy on C++ files in code base",
                        action="store_true")

    if len(sys.argv) < 2:
        parser.print_help()
        sys.exit(1)

    options = parser.parse_args()

    if options.all:
        options.cmake_format = True
        options.clang_format = True
        options.clang_tidy = True

    if options.cmake_format:
        command = ["cmake-format", "-i"]
        cmake_files = get_files_if(cmake_filter)
        execute_command(command, cmake_files)

    if options.clang_format:
        command = ["clang-format", "-i", "-style=file"]
        cpp_files = get_files_if(cpp_filter)
        execute_command(command, cpp_files)

    print("Done!\nPlease do a 'git diff' to make sure the files were "
          "formatted to your liking.\nUse 'git checkout -- /path/to/file' "
          "to undo any changes or 'git add -p' to\ninteractively add the "
          "changes you do want to keep.")

    print("\nIf 'git diff' shows no changes then the cmake files were already "
          "formatted correctly.")
