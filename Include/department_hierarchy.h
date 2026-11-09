
#ifndef DEPARTMENT_HIERARCHY_H
#define DEPARTMENT_HIERARCHY_H
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;



struct DepartmentNode {
    string name;
    DepartmentNode* parent;
    vector<DepartmentNode*> children;
};

class DepartmentTree {


private:
    DepartmentNode* root = nullptr;
    DepartmentNode* findNode(DepartmentNode* node, string name) {
        if (node == nullptr) {
            return nullptr;
        }

        if (node->name == name) {
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
    void deleteTree(DepartmentNode* node) {
        if (node == nullptr) {
            return;
        }

        for (DepartmentNode* child : node->children) {
            deleteTree(child);
        }

        delete node;
    }


public:
    DepartmentTree() {
        root = nullptr;
    }

    DepartmentTree() {
        deleteTree(root);
    }
    bool addDepartment(string name, string parentName) {
        if (findNode(root, name) != nullptr) {
            return false;
        }

        if (root == nullptr) {
            if (parentName == "NONE") {
                root = new DepartmentNode;
                root->name = name;
                root->parent = nullptr;
                return true;
            }

            if (parentName.empty()) {
                root = new DepartmentNode;
                root->name = name;
                root->parent = nullptr;
                return true;
            }

            return false;
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
    void displayHierarchy() {
        if (root == nullptr) {
            cout << "There are no departments found." << endl;
            return;
        }

        showTree(root, 0);
    }
    void loadDepartments(string filename) {
        ifstream file(filename);

        if (!file.is_open()) {
            cout << "Could not open the file " << filename << endl;
            return;
        }

        string line;
        int lineNumber = 0;
        int loaded = 0;

        while (getline(file, line)) {
            lineNumber++;

            if (line.empty()) {
                continue;
            }

            if (line[0] == '#') {
                continue;
            }

            int pos = line.find('|');

            if (pos == string::npos) {
                cout << filename << " line " << lineNumber
                     << ": Missing |" << endl;
                continue;
            }

            string name = line.substr(0, pos);
            string parentName = line.substr(pos + 1);

            if (name.empty()) {
                cout << filename << " line " << lineNumber
                     << ": Name is missing" << endl;
                continue;
            }

            if (findNode(root, name) != nullptr) {
                cout << filename << " line " << lineNumber
                     << ": Duplicate department" << endl;
                continue;
            }

            if (addDepartment(name, parentName)) {
                loaded++;
            }
            else {
                cout << filename << " line " << lineNumber
                     << ": Parent not found or invalid root" << endl;
            }
        }

        cout << "Departments loaded: " << loaded << endl;
    }
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
