#include <iostream>   // Include the input/output stream library for standard I/O operations.
#include <fstream>    // Include the file stream library for file I/O operations.
#include <sstream>    // Include the string stream library for string stream operations.
#include <vector>     // Include the vector library for using the std::vector container.
#include <algorithm>  // Include the algorithm library for algorithms like std::sort.
#include <cstring>    // Include the C string library for functions like strncpy and strcmp.
#include <cctype>     // Include the C character library for character functions like ::tolower.

using namespace std;  // Bring all standard library names into the global namespace.

struct Item {         // Define a structure to represent an item in the inventory.
    int item_id;                      // Item ID.
    char item_name[100];              // Item name.
    int item_quantity;                // Item quantity.
    char item_registration_date[11];  // Item registration date.
};

void addItem(int item_id, const char *item_name, int item_quantity, const char *item_registration_date) {
    fstream file("inventory.csv", ios::app); // Open the file in append mode. `ios::app` means append to the end of the file.
    if (file.is_open()) {                    // Check if the file is open.
        file << item_id << "," << item_name << "," << item_quantity << "," << item_registration_date << "\n"; // Write the item details to the file.
        file.close();                        // Close the file.
    } else {
        cerr << "Unable to open file for writing.\n";  // Print an error message if the file can't be opened.
    }
}

vector<Item> readInventory() {
    vector<Item> items;                      // Create a vector to store the items.
    fstream file("inventory.csv", ios::in);  // Open the file in input mode. `ios::in` means open for reading.
    if (file.is_open()) {                    // Check if the file is open.
        string line;
        while (getline(file, line)) {        // Read each line from the file.
            stringstream ss(line);           // Create a string stream from the line.
            string token;
            Item item = {};                  // Initialize item with default values.
            getline(ss, token, ','); item.item_id = stoi(token); // Read and convert item_id.
            getline(ss, token, ','); strncpy(item.item_name, token.c_str(), sizeof(item.item_name)); // Read and copy item_name.
            getline(ss, token, ','); item.item_quantity = stoi(token); // Read and convert item_quantity.
            getline(ss, token, ','); strncpy(item.item_registration_date, token.c_str(), sizeof(item.item_registration_date)); // Read and copy item_registration_date.
            items.push_back(item);            // Add the item to the vector.
        }
        file.close();                         // Close the file.
    } else {
        cerr << "Unable to open file for reading.\n";  // Print an error message if the file can't be opened.
    }
    return items;                             // Return the vector of items.
}

// Comparison function for sorting items alphabetically by item_name
bool compareItems(const Item &a, const Item &b) {
    return strcmp(a.item_name, b.item_name) < 0;
}

void listItems() {
    vector<Item> items = readInventory();     // Read all items from the inventory.
    sort(items.begin(), items.end(), compareItems); // Sort items alphabetically by item_name using the comparison function.

    for (size_t i = 0; i < items.size(); ++i) { // Traditional for loop to print each item in the sorted list.
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
    string lowerStr = str;                    // Create a copy of the input string.
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower); // Convert each character to lowercase.
    return lowerStr;                          // Return the lowercase string.
}

int main() {
    string command;
    while (true) {                            // Loop indefinitely until "exit" command is entered.
        cout << "Enter command: ";
        getline(cin, command);                // Read the entire command line.

        string lowerCommand = toLowerCase(command); // Convert the command to lowercase.
        stringstream ss(lowerCommand);        // Create a string stream from the command.

        string cmd;
        ss >> cmd;                            // Extract the command word.

        if (cmd == "itemadd") {               // If the command is "itemadd":
            int item_id, item_quantity;
            char item_name[100], item_registration_date[11];
            ss >> item_id >> item_name >> item_quantity >> item_registration_date; // Extract the arguments.
            addItem(item_id, item_name, item_quantity, item_registration_date);   // Call addItem with the arguments.
        } else if (cmd == "itemslist") {      // If the command is "itemslist":
            listItems();                      // Call listItems.
        } else if (cmd == "help") {           // If the command is "help":
            showHelp();                       // Call showHelp.
        } else if (cmd == "exit") {           // If the command is "exit":
            break;                            // Exit the loop.
        } else {
            cerr << "Unknown command.\n";     // Print an error message for unknown commands.
        }
    }

    return 0;
}
