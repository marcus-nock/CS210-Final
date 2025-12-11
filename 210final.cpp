#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
using namespace std;

//student record
struct Student {
    int id;
    string name;
    string major;
};

//binary search tree
struct BSTNode {
    Student s;
    BSTNode* left;
    BSTNode* right;
    BSTNode(Student st) : s(st), left(nullptr), right(nullptr) {}
};

class StudentBST {
public:
    BSTNode* root = nullptr;

    BSTNode* insertRec(BSTNode* node, Student s) {
        if (!node) return new BSTNode(s);
        if (s.name < node->s.name)
            node->left = insertRec(node->left, s);
        else
            node->right = insertRec(node->right, s);
        return node;
    }

    void insert(Student s) {
        root = insertRec(root, s);
    }

    void inorder(BSTNode* node) {
        if (!node) return;
        inorder(node->left);
        cout << node->s.name << " (ID: " << node->s.id 
             << ", Major: " << node->s.major << ")\n";
        inorder(node->right);
    }

    void printAlphabetical() {
        inorder(root);
    }
};

//student directory
class StudentDirectory {
public:
    vector<Student> students;                     // List
    unordered_map<int, Student> table;            // Hash Table
    StudentBST bst;                               // Tree

    void addStudent(int id, string name, string major) {
        Student s = {id, name, major};
        students.push_back(s);
        table[id] = s;
        bst.insert(s);
        cout << "Student added.\n";
    }

    void deleteStudent(int id) {
        table.erase(id);

        //remove from list
        for (int i = 0; i < students.size(); i++) {
            if (students[i].id == id) {
                students.erase(students.begin() + i);
                break;
            }
        }
        cout << "Student deleted (BST not rebuilt for simplicity).\n";
    }

    void searchByID(int id) {
        if (table.find(id) != table.end()) {
            Student s = table[id];
            cout << "Found: " << s.name 
                 << " (Major: " << s.major << ")\n";
        } else {
            cout << "Student not found.\n";
        }
    }

    void displayAll() {
        for (auto& s : students) {
            cout << s.id << " | " << s.name << " | " << s.major << endl;
        }
    }

    void displayAlphabetical() {
        bst.printAlphabetical();
    }
};

//friendship graph (for friends-of-friends)
class FriendshipGraph {
public:
    unordered_map<int, vector<int>> adj; // student ID adjacency

    void addFriendship(int id1, int id2) {
        adj[id1].push_back(id2);
        adj[id2].push_back(id1);
        cout << "Friendship added between " << id1 << " and " << id2 << "\n";
    }

    void suggestFriends(int id) {
        if (adj.find(id) == adj.end()) {
            cout << "Student has no friends or does not exist.\n";
            return;
        }

        unordered_set<int> visited;
        queue<int> q;
        unordered_set<int> suggestions;

        visited.insert(id);

        // direct friends
        for (int f : adj[id]) {
            visited.insert(f);
            q.push(f);
        }

        // friends-of-friends
        while (!q.empty()) {
            int cur = q.front(); q.pop();
            for (int neighbor : adj[cur]) {
                if (visited.find(neighbor) == visited.end()) {
                    suggestions.insert(neighbor);
                    visited.insert(neighbor);
                }
            }
        }

        if (suggestions.empty()) {
            cout << "No friend suggestions for " << id << "\n";
            return;
        }

        cout << "Friend suggestions for " << id << ": ";
        for (int s : suggestions) cout << s << " ";
        cout << "\n";
    }
};

//main menu
int main() {
    StudentDirectory dir;
    FriendshipGraph friends;

    int choice;

    while (true) {
        cout << "\n======== MAIN MENU ========\n";
        cout << "1. Add Student\n";
        cout << "2. Delete Student\n";
        cout << "3. Search Student by ID\n";
        cout << "4. Display All Students\n";
        cout << "5. Display Students Alphabetically\n";
        cout << "6. Add Friendship\n";
        cout << "7. Suggest Friends\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 0) break;

        if (choice == 1) {
            int id; string name, major;
            cout << "ID: "; cin >> id;
            cout << "Name: "; cin >> name;
            cout << "Major: "; cin >> major;
            dir.addStudent(id, name, major);
        }
        else if (choice == 2) {
            int id; 
            cout << "ID: "; cin >> id;
            dir.deleteStudent(id);
        }
        else if (choice == 3) {
            int id; 
            cout << "ID: "; cin >> id;
            dir.searchByID(id);
        }
        else if (choice == 4) {
            dir.displayAll();
        }
        else if (choice == 5) {
            dir.displayAlphabetical();
        }
        else if (choice == 6) {
            int id1, id2;
            cout << "Student ID 1: "; cin >> id1;
            cout << "Student ID 2: "; cin >> id2;
            friends.addFriendship(id1, id2);
        }
        else if (choice == 7) {
            int id;
            cout << "Student ID: "; cin >> id;
            friends.suggestFriends(id);
        }
    }

    return 0;
}
