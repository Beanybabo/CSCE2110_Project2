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

The four files are loaded in this order: buildings.txt, connections.txt, facilities.txt, departments.txt.
Warnings and one load summary line per file are printed, then the menu appears.

