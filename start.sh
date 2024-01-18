#!/bin/bash

# Change directory to ./bin
cd ./bin

# Loop through all executable files in the directory and run them in the background
for file in *; do
    if [[ -x "$file" ]]; then
        ./"$file" &
    fi
done

# Optionally, you can wait for all background processes to finish
wait
