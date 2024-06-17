#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cctype>

using namespace std;

struct Item {
    int item_id;
    char item_name[100];
    int item_quantity;
    char item_registration_date[11];
};

void addItem(int item_id, const char *item_name, int item_quantity, const char *item_registration_date) {
    fstream file("inventory.csv", ios::app);
    if (file.is_open()) {
        file << item_id << "," << item_name << "," << item_quantity << "," << item_registration_date << "\n";
        file.close();
    } else {
        cerr << "Unable to open file for writing.\n";
    }
}

vector<Item> readInventory() {
    vector<Item> items;
    fstream file("inventory.csv", ios::in);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string token;
            Item item = {};
            getline(ss, token, ','); item.item_id = stoi(token);
            getline(ss, token, ','); strncpy(item.item_name, token.c_str(), sizeof(item.item_name));
            getline(ss, token, ','); item.item_quantity = stoi(token);
            getline(ss, token, ','); strncpy(item.item_registration_date, token.c_str(), sizeof(item.item_registration_date));
            items.push_back(item);
        }
        file.close();
    } else {
        cerr << "Unable to open file for reading.\n";
    }
    return items;
}

bool compareItems(const Item &a, const Item &b) {
    return strcmp(a.item_name, b.item_name) < 0;
}

void listItems() {
    vector<Item> items = readInventory();
    sort(items.begin(), items.end(), compareItems);

    for (size_t i = 0; i < items.size(); ++i) {
        cout << "Item ID:" << items[i].item_id << "\t"
             << "Item Name:" << items[i].item_name << "\t"
             << "Quantity :" << items[i].item_quantity << "\t"
             << "Reg Date :" << items[i].item_registration_date << "\n";
    }
}

void showHelp() {
    cout << "--------------------------\n";
    cout << "* Commands syntaxes      *\n";
    cout << "--------------------------\n";
    cout << "itemadd <item_id> <item_name> <quantity> <registration_date>\n";
    cout << "itemslist\n";
    cout << "help\n";
    cout << "exit\n";
}

string toLowerCase(const string &str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

int main() {
    string command;
    while (true) {
        cout << "Enter command: ";
        getline(cin, command);

        string lowerCommand = toLowerCase(command);
        stringstream ss(lowerCommand);

        string cmd;
        ss >> cmd;

        if (cmd == "itemadd") {
            int item_id, item_quantity;
            char item_name[100], item_registration_date[11];
            ss >> item_id >> item_name >> item_quantity >> item_registration_date;
            addItem(item_id, item_name, item_quantity, item_registration_date);
        } else if (cmd == "itemslist") {
            listItems();
        } else if (cmd == "help") {
            showHelp();
        } else if (cmd == "exit") {
            break;
        } else {
            cerr << "Unknown command.\n";
        }
    }

    return 0;
}
