#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

// Task class to represent a task with a description and a completed status.
class Task {
public:
    string description;
    bool completed;

    // Constructor initializes a task with a description and an optional completion status.
    Task(const string& desc, bool comp = false) : description(desc), completed(comp) {}
};

// Class to manage personal tasks, including adding, completing, and deleting tasks.
class PersonalInformationManager {
private:
    vector<Task> tasks; // Vector to store tasks
    string filename = "tasks.txt"; // Filename for saving/loading tasks

public:
    PersonalInformationManager() {
        loadTasks(); // Load tasks from file upon initialization
    }

    ~PersonalInformationManager() {
        saveTasks(); // Save tasks to file upon destruction
    }

    // Add a new task with a given description.
    void addTask(const string& taskDescription) {
        if (taskDescription.empty()) {
            cout << "\nTask description cannot be empty.\n";
            return;
        }
        tasks.emplace_back(taskDescription);
        cout << "\nTask added: " << taskDescription << "\n";
    }

    // Mark a task as completed by its index.
    void completeTask(size_t taskIndex) {
        if (taskIndex >= tasks.size()) {
            cout << "\nInvalid task index.\n";
            return;
        }
        if (tasks[taskIndex].completed) {
            cout << "\nTask already completed.\n";
            return;
        }
        tasks[taskIndex].completed = true;
        cout << "\nTask completed: " << tasks[taskIndex].description << "\n";
    }

    // Mark a task as not completed by its index.
    void uncompleteTask(size_t taskIndex) {
        if (taskIndex >= tasks.size()) {
            cout << "\nInvalid task index.\n";
            return;
        }
        if (!tasks[taskIndex].completed) {
            cout << "\nTask not completed.\n";
            return;
        }
        tasks[taskIndex].completed = false;
        cout << "\nTask uncompleted: " << tasks[taskIndex].description << "\n";
    }

    // List all tasks with their completion status.
    void listTasks() const {
        if (tasks.empty()) {
            cout << "\nNo tasks to display.\n";
            return;
        }
        cout << "\n-------- TASK LIST --------";
        for (size_t i = 0; i < tasks.size(); ++i) {
            cout << "\n" << i + 1 << ". [" << (tasks[i].completed ? "X" : " ") << "] " << tasks[i].description;
        }
        cout << "\n---------------------------\n";
    }

    // Function to delete tasks, either a specific task by number or all/completed tasks.
    void deleteTask(const string& param) {
        // Implementation not shown for brevity
    }

    // Save tasks to a file, marking them as completed or not.
    void saveTasks() {
        ofstream outFile(filename);
        if (!outFile) {
            cerr << "Error saving tasks to file.\n";
            return;
        }
        for (const auto& task : tasks) {
            outFile << (task.completed ? "1" : "0") << "|" << task.description << "\n";
        }
    }

    // Load tasks from a file, restoring their completed status.
    void loadTasks() {
        ifstream inFile(filename);
        if (!inFile) {
            cerr << "Error loading tasks from file. Starting with an empty list.\n";
            return;
        }
        string line;
        while (getline(inFile, line)) {
            if (line.size() < 3) continue; // Basic validation of line format
            bool completed = (line[0] == '1');
            string description = line.substr(2);
            tasks.emplace_back(description, completed);
        }
    }

    // Display available commands to the user.
    void showCommands() const {
        cout << "\n\nAvailable commands:"
             << "\n  add <task_description> - Add a new task"
             << "\n  list - List all tasks"
             << "\n  complete <task_number> - Mark a task as completed"
             << "\n  uncomplete <task_number> - Mark a task as not completed"
             << "\n  delete <task_number> or \"all\" - Delete a specific task or all tasks"
             << "\n  exit - Exit the to-do list management\n";
    }
};

// Display the main menu of the Personal Information Manager.
void showServices() {
    cout << "\nWelcome to the Personal Information Manager. \n\nAvailable services:"
         << "\n1- to-do-list"
         << "\n2- calculator (Coming soon...)"
         << "\n3- calendar (Coming soon...)"
         << "\nType 'exit' to close the program.\n";
}

int main() {
    while (true) {
        showServices();
        cout << "\nSelect a service (or type 'exit' to close the program) \n\nPIM> ";
        string serviceChoice;
        getline(cin, serviceChoice);

        if (serviceChoice == "1") {
            PersonalInformationManager manager;
            cout << "\nWelcome to the to-do list.";
            manager.showCommands();

            string command;
            while (true) {
                cout << "\nPIM> ";
                getline(cin, command);

                if (command == "exit") {
                    break;
                }

                size_t spaceIndex = command.find(' ');
                string cmd = command.substr(0, spaceIndex);
                string param = spaceIndex != string::npos ? command.substr(spaceIndex + 1) : "";

                // Parse and execute commands
                if (cmd == "add") {
                    manager.addTask(param);
                } else if (cmd == "list") {
                    manager.listTasks();
                } else if (cmd == "complete") {
                    try {
                        size_t taskIndex = stoi(param) - 1;
                        manager.completeTask(taskIndex);
                    } catch (...) {
                        cout << "\nPlease specify a valid task number.\n";
                    }
                } else if (cmd == "uncomplete") {
                    try {
                        size_t taskIndex = stoi(param) - 1;
                        manager.uncompleteTask(taskIndex);
                    } catch (...) {
                        cout << "\nPlease specify a valid task number.\n";
                    }
                } else if (cmd == "delete") {
                    manager.deleteTask(param);
                } else if (cmd == "commands") {
                    manager.showCommands();
                } else {
                    cout << "\nUnknown command. Type 'commands' for a list of all commands.\n";
                }
            }
        } else if (serviceChoice == "exit") {
            break;
        } else {
            cout << "\nThis service is currently under development or not available.\n";
        }
    }

    return 0;
}
