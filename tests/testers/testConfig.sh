#!/bin/bash

# Define the executable and folder paths
executable_path="../../webserv"
folder_path="../configuration-files/errors/"

# Print the current directory's contents
ls .

# Iterate through files in the folder_path
for filename in $(ls ${folder_path}); do
    # Check if the file ends with ".conf"
    if [[ "${filename}" == *.conf ]]; then
        conf_file_path="${folder_path}${filename}"
        echo -e "\n----------\nLoading program with ${conf_file_path}"

        # Execute the program with the configuration file
        ${executable_path} ${conf_file_path} &

        # Get the PID of the last background process
        subprocess_pid=$!

        # Find all child processes and terminate them
        pkill -P ${subprocess_pid}

        # Terminate the parent process
        kill ${subprocess_pid}
        # Wait for a bit before continuing
        #sleep 1
        echo "Program terminated"
    fi
done
