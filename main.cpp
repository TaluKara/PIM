#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

class Task {
public:
    string description;
    bool completed;

    Task(const string& desc, bool comp = false) : description(desc), completed(comp) {}
};

class PersonalInformationManager {
private:
    vector<Task> tasks;
    string filename = "tasks.txt";

public:
    PersonalInformationManager() {
        loadTasks();
    }

    ~PersonalInformationManager() {
        saveTasks();
    }

    void addTask(const string& taskDescription) {
        tasks.emplace_back(taskDescription);
        cout << "Task added: " << taskDescription << endl;
    }

    void completeTask(size_t taskIndex) {
        if (taskIndex < tasks.size() && !tasks[taskIndex].completed) {
            tasks[taskIndex].completed = true;
            cout << "Task completed: " << tasks[taskIndex].description << endl;
        } else {
            cout << "Invalid task index or task already completed." << endl;
        }
    }

    void uncompleteTask(size_t taskIndex) {
        if (taskIndex < tasks.size() && tasks[taskIndex].completed) {
            tasks[taskIndex].completed = false;
            cout << "Task uncompleted: " << tasks[taskIndex].description << endl;
        } else {
            cout << "Invalid task index or task not completed." << endl;
        }
    }

    void listTasks() const {
        cout << "-------- TASK LIST --------" << endl;
        cout << "----UNCOMPLETED----" << endl;
        bool hasUncompleted = false, hasCompleted = false;
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (!tasks[i].completed) {
                cout << i + 1 << ". " << tasks[i].description << endl;
                hasUncompleted = true;
            }
        }
        if (!hasUncompleted) {
            cout << "No uncompleted tasks!" << endl;
        }

        cout << "---COMPLETED---" << endl;
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (tasks[i].completed) {
                cout << "c-" << i + 1 << ". " << tasks[i].description << endl;
                hasCompleted = true;
            }
        }
        if (!hasCompleted) {
            cout << "No completed tasks!" << endl;
        }
        cout << "---------------------------" << endl;
    }

    void saveTasks() {
        ofstream outFile(filename);
        for (const auto& task : tasks) {
            outFile << (task.completed ? "1" : "0") << "|" << task.description << "\n";
        }
    }

    void loadTasks() {
        ifstream inFile(filename);
        string line;
        while (getline(inFile, line)) {
            bool completed = (line[0] == '1');
            string description = line.substr(2);
            tasks.emplace_back(description, completed);
        }
    }

    void deleteTask(size_t taskIndex) {
        if (taskIndex < tasks.size()) {
            cout << "Are you sure you want to delete \"" << tasks[taskIndex].description << "\"? (y/n): ";
            char response;
            cin >> response;
            if (response == 'y' || response == 'Y') {
                tasks.erase(tasks.begin() + taskIndex);
                cout << "Task deleted." << endl;
            } else {
                cout << "Deletion cancelled." << endl;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            cout << "Invalid task index." << endl;
        }
    }

    void deleteAllTasks(const string& option) {
        string prompt = "Are you sure you want to delete ";
        prompt += (option == "all") ? "all tasks? (y/n): " : "all completed tasks? (y/n): ";
        cout << prompt;
        char response;
        cin >> response;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (response == 'y' || response == 'Y') {
            if (option == "all") {
                tasks.clear();
            } else if (option == "completed") {
                tasks.erase(remove_if(tasks.begin(), tasks.end(), [](const Task& task) { return task.completed; }), tasks.end());
            }
            cout << "Tasks deleted." << endl;
        } else {
            cout << "Deletion cancelled." << endl;
        }
    }

    void showCommands() const {
        cout << "Available commands:" << endl;
        cout << "  add <task_description> - Add a new task" << endl;
        cout << "  list - List all tasks" << endl;
        cout << "  complete <task_number> - Mark a task as completed" << endl;
        cout << "  uncomplete <task_number> - Mark a task as not completed" << endl;
        cout << "  delete <task_number> - Delete a specific task" << endl;
        cout << "  delete all - Delete all tasks" << endl;
        cout << "  delete completed - Delete all completed tasks" << endl;
        cout << "  commands - List all commands" << endl;
        cout << "  exit - Exit the program" << endl;
    }
};

int main() {
    PersonalInformationManager manager;
    cout << "Welcome to Personal Information Manager. Type 'commands' for a list of all commands." << endl;

    string command;
    while (true) {
        cout << "PIM> ";
        getline(cin, command);

        if (command == "add") {
            string taskDescription = command.substr(4);
            manager.addTask(taskDescription);
        } else if (command == "list") {
            manager.listTasks();
        } else if (command.find("complete ") == 0) {
            size_t taskIndex = stoi(command.substr(9)) - 1;
            manager.completeTask(taskIndex);
        } else if (command.find("uncomplete ") == 0) {
            size_t taskIndex = stoi(command.substr(11)) - 1;
            manager.uncompleteTask(taskIndex);
        } else if (command.find("delete ") == 0) {
            if (command == "delete all") {
                manager.deleteAllTasks("all");
            } else if (command == "delete completed") {
                manager.deleteAllTasks("completed");
            } else {
                size_t taskIndex = stoi(command.substr(7)) - 1;
                manager.deleteTask(taskIndex);
            }
        } else if (command == "commands") {
            manager.showCommands();
        } else if (command == "exit") {
            break;
        } else {
            cout << "Unknown command. Type 'commands' to see all available commands." << endl;
        }
    }

    return 0;
}
