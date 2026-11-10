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
    Run this command from the project root (Linux / UNT CSE CELL):
 
        g++ -std=c++11 -g -Iinclude src/*.cpp -o campus
 
    This compiles every .cpp file in src/ and creates the program ./campus.
    Run it again after changing any code.
 
    Folder names are case-sensitive: the headers folder must be "include"
    and the data folder "data".
 
    In VS Code you can also build with Ctrl+Shift+B (uses
    .vscode/tasks.json), which runs the same command.
 
RUN
    Always run the program from the project root (the folder that contains
    data/, src/, and include/). The data directory is a relative path, so
    running from any other folder (for example src/) reports the data files
    as missing.
 
    1. Build the program (see BUILD above):
 
        g++ -std=c++11 -g -Iinclude src/*.cpp -o campus
 
    2. Run with the default data directory (data/):
 
        ./campus
 
TESTING
    The tests are recorded in test_results.txt (test number, description,
    input, expected result, actual result, pass or fail). To repeat a test,
    build the program, then run it from the project root with the data
    directory named in the test and type the listed menu input. Example:
 
        ./campus testdata/messy
 
    Press Enter after each entry, and finish with 0 to exit.
 
    Test data folders (use them as the optional run argument):
        testdata/collision      IDs that collide in the hash table
        testdata/messy          bad records of every kind
        testdata/crlf           Windows line endings, extra spaces, # in a name
        testdata/empty_files    all four files empty
        testdata/missing_files  facilities.txt is absent
        testdata/root_only      one-department tree