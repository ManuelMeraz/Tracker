#! /usr/bin/env python3
import argparse
import os
import subprocess
import sys

PROJECT_PATH = os.environ["TRACKER_PROJECT"]

if not PROJECT_PATH:
    print("Please source the set_env.bash script in the scripts/ directory to "
          "set the $TRACKER_PROJECT environment variable, which keeps track of"
          " the project directory.")
    sys.exit(1)


def should_configure(options):
    """
    @brief Parse through options to determine if the build folder
           should be configured

    @param options The argseparse options containing the
                   options passed in

    @return A booloean whether to configure or not
    """

    if options.configure:
        return True

    conaninfo = os.path.join(PROJECT_PATH, options.build_folder,
                             "conaninfo.txt")

    if os.path.isfile(conaninfo):
        profile = os.path.join(os.environ["HOME"], ".conan/profiles",
                               options.profile)

        profile_data = None
        with open(profile) as f:
            profile_data = set(f.read().split())

        conaninfo_data = None
        with open(conaninfo) as f:
            f = [line.strip() for line in f]
            conaninfo_data = set(f)

        is_same_profile = profile_data.issubset(conaninfo_data)

        return not is_same_profile

    return True


def execute_command(command):
    """
    @brief execute the following command

    @param command A comand to build or configure a build
    """
    try:
        process = subprocess.run(
            command,
            # stdout=subprocess.PIPE,
            # stderr=subprocess.PIPE,
            check=True,
            universal_newlines=True,
        )

    except FileNotFoundError:
        print("\nAttempted to use " + command[0] + " , but it looks like"
              "it's not installed!\n")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "-pr",
        "--profile",
        default="clang60-debug",
        help="Recongfigure project with new profile and rebuild project",
    )

    parser.add_argument(
        "-bf",
        "--build-folder",
        default="build",
        help="The build folder where conan installa and build will run",
    )

    parser.add_argument("-c",
                        "--configure",
                        help="Configure the build directory",
                        action="store_true")

    options = parser.parse_args()

    build_path = os.path.join(PROJECT_PATH, options.build_folder)

    if should_configure(options):
        command = [
            "conan",
            "install",
            PROJECT_PATH,
            "--install-folder",
            build_path,
            "--profile",
            options.profile,
            "--build=missing",
            "--build=outdated",
        ]
        execute_command(command)

    command = ["conan", "build", PROJECT_PATH, "--build-folder", build_path]
    execute_command(command)