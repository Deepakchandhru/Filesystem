#include <iostream>
#include <fstream>
#include <stack>
#include <direct.h>
#include <string>
#include <vector>
#include <algorithm>

#include "quick.cpp"

using namespace std;

// Structure to represent a file or directory
struct Node {
    string name;
    bool isFile;
    vector<Node*> children;

    Node(string _name, bool _isFile) : name(_name), isFile(_isFile) {}
};

// Class representing the file management system
class FileManager {
private:
    Node* root;

    void deleteNode(Node* node) {
        if (node == nullptr)
            return;
        for (Node* child : node->children) {
            deleteNode(child);
        }
        delete node;
    }

    // Helper function to recursively search for a node
    Node* searchNode(Node* node, string path) {
        if (node == nullptr || path.empty())
            return nullptr;

        size_t pos = path.find("/");
        string token = path.substr(0, pos);

        for (Node* child : node->children) {
            if (child->name == token) {
                if (pos == string::npos || pos == path.length() - 1) {
                    return child; // Found the node
                } else {
                    return searchNode(child, path.substr(pos + 1));
                }
            }
        }

        return nullptr; // Node not found
    }

public:
    FileManager() {
        root = new Node("D:/", false); // Root directory
    }

    Node* getRoot() {
        return root;
    }

    // Function to insert a file or directory

    void insert(string path, bool isFile) {
        string p=path;
        Node* current = root;
        string delimiter = "/";
        size_t pos = 0;
        string token;

        // Get the parent directory of the destination path
        string parentPath = path.substr(0, path.find_last_of("/"));
        Node* parentNode = searchNode(root, parentPath);


        while ((pos = path.find(delimiter)) != string::npos) {
            token = path.substr(0, pos);
            auto it = find_if(current->children.begin(), current->children.end(),
                          [&token](Node* node) { return node->name == token; });

            if(it == current->children.end()) {
                Node* newNode = new Node(token, false);
                current->children.push_back(newNode);
                current = newNode;
            }else {
                current = *it;
            }

            path.erase(0, pos + delimiter.length());
        }

        // Check if the current node is a file
        if (current->isFile) {
            cout << "Cannot insert into a file." << endl;
            return;
        }

        // Insert the file or directory at the end
        Node* newNode = new Node(path, isFile);
        current->children.push_back(newNode);
        if (isFile) {
            ofstream outfile("D:/" + p);
            outfile.close();
        }else {
            _mkdir(("D:/" + p).c_str());
        }
    }

    // Function to earch for a file or directory
    Node* search(string path) {
        return searchNode(root, path);
    }

    Node* findNode(Node* node, string path) {
        if (node == nullptr)
            return node;

        stack<Node*> st;

        // Push the starting vertex onto the stack
        st.push(node);

        while (!st.empty()) {
            // Pop a vertex from the stack
            Node* current = st.top();
            st.pop();

            if(current->name == path)
                return current;

            for (Node* child : current->children)
                st.push(child);
        }
        return nullptr;
    }

    // Function to display the file structure
    void display(Node* node, int depth) {
        if (node == nullptr)
            return;

        for (int i = 0; i < depth; ++i)
            cout << "\t";
        cout << node->name << (node->isFile ? " (File)" : " (Directory)") << endl;

        for (Node* child : node->children)
            display(child, depth + 1);
    }

    void writecontent(string file_abs_path){
        ofstream File("D:/" + file_abs_path);
        if (File.is_open()) {
            string content;
            int choice=1;
            while (choice > 0) {
                cout<<"Enter content : " ;
                cin>>content;
                cout<<"Do you want to continue (-1 to end) : ";
                cin>>choice;
                File << content << endl;
            }
            File.close();
        } else {
            cout << "Error: Unable to open input file." << endl; // Exit with error
        }
    }

    void readcontent(string file_abs_path){
        ifstream inputFile("D:/" + file_abs_path);
        if (inputFile.is_open()) {
            string content;
            while (getline(inputFile,content)) {
                cout<<content<<endl;
            }
            inputFile.close();
        } else {
            cout << "Error: Unable to open input file." << endl; // Exit with error
        }
    }

    void quickbar(Node_s* root, int space=0) {
        if (root == nullptr)
            return;

        // Increase distance between levels
        space += 5;

        // Process right child first
        quickbar(root->right, space);

        // Print current node after space
        cout << endl;
        for (int i = 5; i < space; i++)
            cout << " ";
        cout << root->path << "\n";

        // Process left child
        quickbar(root->left, space);
    }

    // Destructor to free memory
    ~FileManager() {
        deleteNode(root);
    }
};

bool isTextfile(string path){
    return (path.find('.txt') < path.length());
}

// Helper function to perform a right rotation (Zig)

int main() {
    FileManager fileManager;
    Node_s* root = nullptr;
    vector <string> v;
    vector<string>::iterator it;

    int k=0;
    int key;

    int choice;
    string path, destinationPath;

    do {
        cout << "\nFile Management System Menu:" << endl;
        cout << "1. Insert a text file/directory" << endl;
        cout << "2. Search a text file/directory" << endl;
        cout << "3. Display File manager structure" << endl;
        cout << "4. Write into text file" << endl;
        cout << "5. Display text file content" << endl;
        cout << "6. Display folder content" << endl;
        cout << "7. Quick access bar" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter the path to insert: ";
                cin >> path;
                bool isFile;
                if(isTextfile(path))
                    isFile=true;
                else
                    isFile=false;
                fileManager.insert(path, isFile);
                root = insert(root,k++,path);
                v.push_back(path);
                break;
            case 2:
                cout << "Enter the path to search: ";
                cin >> path;
                if(fileManager.findNode(fileManager.getRoot(),path)){
                    it = find(v.begin(), v.end(), path);
                    key = it-v.begin();
                    root = search(root, key);
                    if(isTextfile(path))
                        cout << "File found." << endl;
                    else
                        cout << "Directory found." << endl;
                }else{
                    if(isTextfile(path))
                        cout << "File not found." << endl;
                    else
                        cout << "Directory not found." << endl;
                }
                break;
            case 3:
                cout << "File Structure:" << endl;
                fileManager.display(fileManager.getRoot(), 0);
                break;
            case 4:
                cout << "Enter the file to write : ";
                cin >> path;
                if (fileManager.search(path)){
                    if(isTextfile(path)) {
                        it = find(v.begin(), v.end(), path);
                        key = it-v.begin();
                        root = search(root, key);
                        fileManager.writecontent(path);
                    }else{
                        cout<<"Cannot open folder as file"<<endl;
                    }
                }else{
                    cout << "File not found." << endl;
                }
                break;
            case 5:
                cout << "Enter the file to read : ";
                cin >> path;
                if (fileManager.search(path)){
                    if(isTextfile(path)){
                        it = find(v.begin(), v.end(), path);
                        key = it-v.begin();
                        root = search(root, key);
                        fileManager.readcontent(path);
                    }else{
                        cout<<"Cannot open folder as file"<<endl;
                    }
                }else{
                    cout << "File not found." << endl;
                }
                break;
            case 6:
                cout << "Enter the folder to search: ";
                cin >> path;
                if (fileManager.search(path)){
                    if(isTextfile(path)) {
                        cout<<"It is a file"<<endl;
                    }else {
                        it = find(v.begin(), v.end(), path);
                        key = it-v.begin();
                        root = search(root, key);
                        fileManager.display(fileManager.search(path), 0);
                    }
                }else{
                    cout << "Directory not found." << endl;
                }
                break;
            case 7:
                 cout<<"Quick Access bar : "<<endl;
                 fileManager.quickbar(root);
                 cout << endl;
                 break;
            case 8:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 8);
    return 0;
}

