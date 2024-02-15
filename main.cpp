#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include <algorithm> // remove_if için
#include <cstdlib> // system için

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
    }

    void completeTask(size_t taskIndex) {
        if (taskIndex < tasks.size() && !tasks[taskIndex].completed) {
            tasks[taskIndex].completed = true;
        } else {
            cout << "Invalid task index or task already completed." << endl;
        }
    }

    void listTasks() const {
        cout << "--->UNCOMPLETED<---" << endl;
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (!tasks[i].completed) {
                cout << i + 1 << ". " << tasks[i].description << endl;
            }
        }

        cout << "--->COMPLETED<---" << endl;
        for (size_t i = 0; i < tasks.size(); ++i) {
            if (tasks[i].completed) {
                cout << "c-" << i + 1 << ". " << tasks[i].description << endl;
            }
        }
    }

    void deleteTask(size_t taskIndex) {
        if (taskIndex < tasks.size()) {
            cout << "Are you sure you want to delete this task? (y/n): ";
            char response;
            cin >> response;
            if (response == 'y' || response == 'Y') {
                tasks.erase(tasks.begin() + taskIndex);
                cout << "Task deleted." << endl;
            } else {
                cout << "Deletion cancelled." << endl;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        } else {
            cout << "Invalid task index." << endl;
        }
    }

    void deleteAllTasks(const string& option) {
        cout << "Are you sure you want to delete ";
        if (option == "all") {
            cout << "all tasks? (y/n): ";
        } else if (option == "completed") {
            cout << "all completed tasks? (y/n): ";
        } else {
            cout << "Invalid option." << endl;
            return;
        }
        char response;
        cin >> response;
        if (response == 'y' || response == 'Y') {
            if (option == "all") {
                tasks.clear();
            } else if (option == "completed") {
                auto newEnd = remove_if(tasks.begin(), tasks.end(), [](const Task& task) { return task.completed; });
                tasks.erase(newEnd, tasks.end());
            }
            cout << "Tasks deleted." << endl;
        } else {
            cout << "Deletion cancelled." << endl;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
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

    void showCommands() const {
        cout << "Available commands:" << endl;
        cout << "  add - Add a new task" << endl;
        cout << "  list - List all tasks" << endl;
        cout << "  complete <task_number> - Mark a task as completed" << endl;
        cout << "  delete <task_number> - Delete a specific task" << endl;
        cout << "  delete all - Delete all tasks" << endl;
        cout << "  delete completed - Delete all completed tasks" << endl;
        cout << "  commands - List all commands" << endl;
        cout << "  exit - Exit the program" << endl;
    }
};

int main() {
    PersonalInformationManager manager;
    cout << "Type 'commands' for a list of all commands." << endl;

    string command;
    while (true) {
        cout << "PIM> ";
        getline(cin, command);

        if (command == "add") {
            string taskDescription;
            cout << "Enter task description: ";
            getline(cin, taskDescription);
            manager.addTask(taskDescription);
        } else if (command == "list") {
            manager.listTasks();
        } else if (command.substr(0, 8) == "complete") {
            size_t taskIndex = stoi(command.substr(9)) - 1;
            manager.completeTask(taskIndex);
        } else if (command.substr(0, 6) == "delete") {
            string subcommand = command.substr(7);
            if (subcommand == "all") {
                manager.deleteAllTasks("all");
            } else if (subcommand == "completed") {
                manager.deleteAllTasks("completed");
            } else {
                size_t taskIndex = stoi(subcommand) - 1;
                manager.deleteTask(taskIndex);
            }
        } else if (command == "commands") {
            manager.showCommands();
        } else if (command == "exit") {
            break;
        } else {
            cout << "Unknown command." << endl;
        }
    }

    return 0;
}
