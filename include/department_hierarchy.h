#ifndef DEPARTMENT_HIERARCHY_H
#define DEPARTMENT_HIERARCHY_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "Stringutils.h"
using namespace std;

// One department in the tree.
struct DepartmentNode {
    string name;
    DepartmentNode* parent = nullptr;
    vector<DepartmentNode*> children;   // insertion order
};

class DepartmentTree {

private:
    DepartmentNode* root = nullptr;

    // Finds a department by name, ignoring capitalization. Returns nullptr if missing.
    DepartmentNode* findNode(DepartmentNode* node, string name) {
        if (node == nullptr) {
            return nullptr;
        }

        if (equalsIgnoreCase(node->name, name)) {
            return node;
        }

        for (DepartmentNode* child : node->children) {
            DepartmentNode* found = findNode(child, name);

            if (found != nullptr) {
                return found;
            }
        }

        return nullptr;
    }

    // Deletes a node and all of its descendants.
    void deleteTree(DepartmentNode* node) {
        if (node == nullptr) {
            return;
        }

        for (DepartmentNode* child : node->children) {
            deleteTree(child);
        }

        delete node;
    }

    // Collects names in preorder (parent first, then children in order).
    void preorderList(DepartmentNode* node, vector<string>& names) {
        if (node == nullptr) {
            return;
        }

        names.push_back(node->name);

        for (DepartmentNode* child : node->children) {
            preorderList(child, names);
        }
    }

    // Prints a node and its children, indented four spaces per level.
    void showTree(DepartmentNode* node, int level) {
        if (node == nullptr) {
            return;
        }

        for (int i = 0; i < level; i++) {
            cout << "    ";
        }

        cout << node->name << endl;

        for (DepartmentNode* child : node->children) {
            showTree(child, level + 1);
        }
    }

public:
    DepartmentTree() {
        root = nullptr;
    }

    // Frees every node when the tree is destroyed.
    ~DepartmentTree() {
        deleteTree(root);
    }

    // The tree owns raw pointers, so copying is not allowed.
    DepartmentTree(const DepartmentTree&) = delete;
    DepartmentTree& operator=(const DepartmentTree&) = delete;

    // True when no departments are loaded.
    bool isEmpty() const {
        return root == nullptr;
    }

    // True when a department with this name exists (ignoring capitalization).
    bool hasDepartment(string name) {
        return findNode(root, name) != nullptr;
    }

    // Adds a department. The first one must use parent "NONE" and becomes the root.
    // Returns false for a duplicate name, an unknown parent, or a bad root.
    bool addDepartment(string name, string parentName) {
        if (findNode(root, name) != nullptr) {
            return false;
        }

        if (root == nullptr) {
            if (!equalsIgnoreCase(parentName, "NONE")) {
                return false;
            }

            root = new DepartmentNode;
            root->name = name;
            return true;
        }

        DepartmentNode* parent = findNode(root, parentName);

        if (parent == nullptr) {
            return false;
        }

        DepartmentNode* child = new DepartmentNode;
        child->name = name;
        child->parent = parent;
        parent->children.push_back(child);

        return true;
    }

    // Shows the hierarchy, one department per line, in preorder.
    void displayHierarchy() {
        if (root == nullptr) {
            cout << "There are no departments found." << endl;
            return;
        }

        showTree(root, 0);
    }

    // Shows the preorder sequence on one line, without indentation.
    void preorder() {
        if (root == nullptr) {
            cout << "There are no departments found." << endl;
            return;
        }

        vector<string> names;
        preorderList(root, names);

        for (size_t i = 0; i < names.size(); i++) {
            cout << names[i];
            if (i + 1 < names.size()) {
                cout << " -> ";
            }
        }
        cout << endl;
    }

    // Loads departments.txt (DepartmentName|ParentName), with warnings and a summary.
    void loadDepartments(string filename) {
        ifstream file(filename);
        int accepted = 0;
        int rejected = 0;

        if (!file.is_open()) {
            cout << "Error: could not open " << filename << " (treated as empty)." << endl;
        }
        else {
            string line;
            int lineNumber = 0;

            while (getline(file, line)) {
                lineNumber++;
                line = trim(line);               // removes spaces and \r

                if (line.empty() || line[0] == '#') {
                    continue;
                }

                vector<string> fields = splitFields(line, '|');

                if (fields.size() != 2) {
                    cout << "Warning: " << filename << " line " << lineNumber
                         << ": expected 2 fields" << endl;
                    rejected++;
                    continue;
                }

                string name = trim(fields[0]);
                string parentName = trim(fields[1]);

                if (name.empty() || parentName.empty()) {
                    cout << "Warning: " << filename << " line " << lineNumber
                         << ": empty required field" << endl;
                    rejected++;
                    continue;
                }

                if (findNode(root, name) != nullptr) {
                    cout << "Warning: " << filename << " line " << lineNumber
                         << ": duplicate department '" << name << "'" << endl;
                    rejected++;
                    continue;
                }

                if (addDepartment(name, parentName)) {
                    accepted++;
                }
                else {
                    cout << "Warning: " << filename << " line " << lineNumber
                         << ": unknown parent '" << parentName
                         << "' or invalid root (first department must use NONE)" << endl;
                    rejected++;
                }
            }
        }

        cout << filename << ": " << accepted << " accepted, " << rejected << " rejected" << endl;
    }

    // Shows name, parent, direct-child count, and depth of a department.
    void searchDepartment(string name) {
        DepartmentNode* node = findNode(root, name);

        if (node == nullptr) {
            cout << "Department could not be found." << endl;
            return;
        }

        cout << "Name: " << node->name << endl;

        if (node->parent == nullptr) {
            cout << "Parent: None" << endl;
        }
        else {
            cout << "Parent: " << node->parent->name << endl;
        }

        cout << "Children: " << node->children.size() << endl;

        int depth = 0;
        DepartmentNode* current = node->parent;

        while (current != nullptr) {
            depth++;
            current = current->parent;
        }
        cout << "Depth: " << depth << endl;
    }
};

#endif