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
        cout << "\nTask added: " << taskDescription << "\n";
    }

    void completeTask(size_t taskIndex) {
        if (taskIndex < tasks.size() && !tasks[taskIndex].completed) {
            tasks[taskIndex].completed = true;
            cout << "\nTask completed: " << tasks[taskIndex].description << "\n";
        } else {
            cout << "\nInvalid task index or task already completed.\n";
        }
    }

    void uncompleteTask(size_t taskIndex) {
        if (taskIndex < tasks.size() && tasks[taskIndex].completed) {
            tasks[taskIndex].completed = false;
            cout << "\nTask uncompleted: " << tasks[taskIndex].description << "\n";
        } else {
            cout << "\nInvalid task index or task not completed.\n";
        }
    }

    void listTasks() const {
        cout << "\n-------- TASK LIST --------";
        for (size_t i = 0; i < tasks.size(); ++i) {
            cout << "\n" << i + 1 << ". [" << (tasks[i].completed ? "X" : " ") << "] " << tasks[i].description;
        }
        cout << "\n---------------------------\n";
    }

    void deleteTask(const string& param) {
    char confirm; // Onay değişkenini burada tanımlayın.
    if (param == "all") {
        string deleteChoice;
        cout << "\nDelete all tasks or only completed tasks? (all/completed): ";
        getline(cin, deleteChoice);

        if (deleteChoice == "all" || deleteChoice == "completed") {
            cout << "\nAre you sure you want to delete " << (deleteChoice == "all" ? "all tasks" : "completed tasks") << "? (y/n): ";
            cin >> confirm;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (confirm == 'y' || confirm == 'Y') {
                if (deleteChoice == "all") {
                    tasks.clear();
                    cout << "\nAll tasks have been deleted.\n";
                } else if (deleteChoice == "completed") {
                    tasks.erase(remove_if(tasks.begin(), tasks.end(), [](const Task& task) { return task.completed; }), tasks.end());
                    cout << "\nCompleted tasks have been deleted.\n";
                }
            } else {
                cout << "\nDeletion cancelled.\n";
            }
        } else {
            cout << "\nInvalid choice. Please type 'all' or 'completed'.\n";
        }
    } else {
        try {
            size_t taskIndex = stoi(param) - 1;
            if (taskIndex < tasks.size()) {
                cout << "\nAre you sure you want to delete \"" << tasks[taskIndex].description << "\"? (y/n): ";
                cin >> confirm;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (confirm == 'y' || confirm == 'Y') {
                    tasks.erase(tasks.begin() + taskIndex);
                    cout << "\nTask deleted.\n";
                } else {
                    cout << "\nDeletion cancelled.\n";
                }
            } else {
                cout << "\nInvalid task number.\n";
            }
        } catch (const std::invalid_argument& e) {
            cout << "\nInvalid argument. Please enter a valid task number or 'all'.\n";
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
        cout << "\n\nAvailable commands:"
             << "\n  add <task_description> - Add a new task"
             << "\n  list - List all tasks"
             << "\n  complete <task_number> - Mark a task as completed"
             << "\n  uncomplete <task_number> - Mark a task as not completed"
             << "\n  delete <task_number> or \"all\" - Delete a specific task or all tasks"
             << "\n  exit - Exit the to-do list management\n";
    }
};

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
        cout << "\nSelect a service (or type 'exit' to close the program) \n\nPIM>";
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
