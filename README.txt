# CSCE 2100 Project 2

Smart Campus Navigation and Facility Analytics System

TEAM
    Davion Green        -  facilities, custom hash table, ID search
    Benjamin Pickett    - campus graph, BFS
    Sakeena Gordon      - department tree 

PURPOSE
    A C++ program that laods campus data from text files and lets a user:
        - view facilities and search them by ID (custom hash table)
        - view the campus graph, its counts/degrees, and run BFS reachability
        - add, search, and display departments and run a preorder traversal

BUILD
    g++ -std=c++11 -g -Iinclude src/*.cpp -o campus
RUN 
    Always run the program from the project root (the folder that contains
    data/, src/, and include/). The data directory is a relative path, so
    running from any other folder (for example src/) reports the data files
    as missing.

    1. Build (see BUILD above):

    2. Run with the default data directory (data/):

        ./campus

Running in VS Code: use Run > "Run campus" (F5, uses .vscode/launch.json)
or run ./campus in the VS Code terminal. The launch configuration sets the
working directory to the project root so data/ is found. The "Run" button
of extensions such as Code Runner starts the program in a different folder
and will not find data/; use the terminal or F5 instead.

On startup the four files are loaded in this order: buildings.txt,
connections.txt, facilities.txt, departments.txt. Warnings and one load
summary line per file are printed, then the menu appears. Enter a menu
number and press Enter; enter 0 to exit.
