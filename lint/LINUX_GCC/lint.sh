#!/usr/bin/env bash
#
# Bash script to perform static code analysis with PC-lint.
#

# Go to the project base directory.
cd ../../source

# Invoke PC-lint (install with Wine).
lint-nt.exe -b -i../lint -i../lint/LINUX_GCC std.lnt -u project.lnt

# Go back to the initial directory.
cd ../lint/LINUX_GCC
