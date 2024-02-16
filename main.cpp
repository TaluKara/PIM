#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include <algorithm>
#include <exception>

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
        cout << "\nTask added: " << taskDescription << "\n\n";
    }

    void completeTask(size_t taskIndex) {
        if (taskIndex < tasks.size() && !tasks[taskIndex].completed) {
            tasks[taskIndex].completed = true;
            cout << "\nTask completed: " << tasks[taskIndex].description << "\n\n";
        } else {
            cout << "\nInvalid task index or task already completed.\n\n";
        }
    }

    void uncompleteTask(size_t taskIndex) {
        if (taskIndex < tasks.size() && tasks[taskIndex].completed) {
            tasks[taskIndex].completed = false;
            cout << "\nTask uncompleted: " << tasks[taskIndex].description << "\n\n";
        } else {
            cout << "\nInvalid task index or task not completed.\n\n";
        }
    }

    void listTasks() const {
        cout << "\n-------- TASK LIST --------\n";
        for (size_t i = 0; i < tasks.size(); ++i) {
            cout << i + 1 << ". [" << (tasks[i].completed ? "X" : " ") << "] " << tasks[i].description << "\n";
        }
        cout << "---------------------------\n\n";
    }

    void deleteTask(const string& param) {
        if (param == "all") {
            cout << "\nDelete (1) all tasks or (2) only completed tasks? ";
            char choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (choice == '1') {
                tasks.clear();
                cout << "\nAll tasks have been deleted.\n\n";
            } else if (choice == '2') {
                auto newEnd = remove_if(tasks.begin(), tasks.end(), [](const Task& task){ return task.completed; });
                tasks.erase(newEnd, tasks.end());
                cout << "\nAll completed tasks have been deleted.\n\n";
            } else {
                cout << "\nInvalid choice. No tasks were deleted.\n\n";
            }
        } else {
            try {
                size_t taskIndex = stoi(param) - 1;
                if (taskIndex >= 0 && taskIndex < tasks.size()) {
                    tasks.erase(tasks.begin() + taskIndex);
                    cout << "\nTask deleted.\n\n";
                } else {
                    cout << "\nInvalid task index.\n\n";
                }
            } catch (const std::exception& e) {
                cout << "\nPlease specify a valid task number or 'all'.\n\n";
            }
        }
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
        cout << "\nAvailable commands:\n"
             << "  add <task_description> - Add a new task\n"
             << "  list - List all tasks\n"
             << "  complete <task_number> - Mark a task as completed\n"
             << "  uncomplete <task_number> - Mark a task as not completed\n"
             << "  delete <task_number> or 'all' - Delete a specific task or all tasks\n"
             << "  exit - Return to service selection\n\n";
    }
};

void showServices() {
    cout << "\nAvailable services:\n"
         << "1- to-do-list\n"
         << "2- lorem (Coming soon...)\n"
         << "3- ipsum (Coming soon...)\n"
         << "Type 'exit' to close the program.\n\n";
}

int main() {
    while (true) {
        showServices();
        cout << "PIM> ";
        string choice;
        getline(cin, choice);

        if (choice == "1") {
            PersonalInformationManager manager;
            cout << "\nWelcome to the To-Do List. Type 'commands' for a list of all commands.\n\n";

            string command;
            while (true) {
                cout << "PIM> ";
                getline(cin, command);

                if (command == "exit") {
                    break;
                } else {
                    size_t spaceIndex = command.find(' ');
                    string cmd = command.substr(0, spaceIndex);
                    string param = spaceIndex != string::npos ? command.substr(spaceIndex + 1) : "";

                    if (cmd == "add") {
                        manager.addTask(param);
                    } else if (cmd == "list") {
                        manager.listTasks();
                    } else if (cmd == "complete") {
                        try {
                            manager.completeTask(stoi(param) - 1);
                        } catch (...) {
                            cout << "\nPlease specify a valid task number.\n\n";
                        }
                    } else if (cmd == "uncomplete") {
                        try {
                            manager.uncompleteTask(stoi(param) - 1);
                        } catch (...) {
                            cout << "\nPlease specify a valid task number.\n\n";
                        }
                    } else if (cmd == "delete") {
                        manager.deleteTask(param);
                    } else if (cmd == "commands") {
                        manager.showCommands();
                    } else {
                        cout << "\nUnknown command. Type 'commands' for a list of all commands.\n\n";
                    }
                }
            }
        } else if (choice == "exit") {
            break;
        } else {
            cout << "\nThis service is currently under development or not available.\n\n";
        }
    }

    return 0;
}
