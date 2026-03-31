// fair warning, im not great at comments and was bored so some of them are odd
// will write and change comments likely after i do the code
//dont care about comment errors yet. will make neat on last project
//did need to look things up, memory is terrible
// i genuinely dont think i can do this without reference.
//added all of theses before started, dont know if needed yet
// still only one file, but that is my current biggest procrastination
#include <iostream>
#include <fstream>
#include <string> //still think this is dumb
#include <iomanip>
#include <chrono> //easier than other options imo
#include <sstream>
#include <limits> // basically redid everything involving this because not allowed to use vector
#include <windows.h> // needed for GetTickCount() for linked list timing

using namespace std;

struct MenuItem {
    string name;        // Food name, can be anything. the code doesn’t restrict this but real stuff makes more sense
    string type;        // Appetizer, Main, Side, Dessert, Beverage (not enforced)
    string nutrition;   // Healthy, Unhealthy, Varies (ditto the last comment)
    double price;       // Supports cents; don’t include a dollar sign or text, errors will happen
};

class MenuManager {
private:
    MenuItem* menuItems;// did it right this time
    int size;
    int capacity;
    const string filename = "menu.txt"; // if not menu.txt, what are you even doing?

    void resizeArray() {
        capacity *= 2; //reworked
        MenuItem* newArray = new MenuItem[capacity]; // doubles array size

        for (int i = 0; i < size; i++) {
            newArray[i] = menuItems[i];  //copy
        }

        delete[] menuItems;
        menuItems = newArray; //stop leak
    }

public:
    MenuManager() { //memory stuff
        capacity = 10;
        size = 0;
        menuItems = new MenuItem[capacity];
        loadFromFile();
    }

    ~MenuManager() {
        delete[] menuItems;
    }

    void loadFromFile() {
        ifstream file(filename); //opens text file, specifically the one submitted on github

        // If the file can't be opened, get notified, does work, recently tested
        if (!file) {
            cout << "Missing or wrong file, we want menu.txt" << endl;
            return;
        }

        size = 0; // Clear items, i don't remember what for

        string line;
        while (getline(file, line)) {
            string name, type, nutrition, priceStr;
            stringstream ss(line);

            // commas seperate things on text file instead of spaces because i confused myself
            getline(ss, name, ',');
            getline(ss, type, ',');
            getline(ss, nutrition, ',');
            getline(ss, priceStr);

            try { //reworked for project b and on
                double price = stod(priceStr);

                if (size >= capacity)
                    resizeArray(); // the array stuff took too long (for me) to figure out

                menuItems[size++] = {name, type, nutrition, price}; // Add item
            } catch (...) {
                // Skip invalid lines if any are there
            }
        }
    }

    void saveToFile() {
        ofstream file(filename); // will be overwritten. only remove items you added please
        //changing file text
        for (int i = 0; i < size; i++) {
            file << menuItems[i].name << ","
                 << menuItems[i].type << ","
                 << menuItems[i].nutrition << ","
                 << menuItems[i].price << "\n";
        }
    }

    void addItem(const MenuItem& item) {
        if (size >= capacity)
            resizeArray();

        menuItems[size++] = item; // Add new item to menu
    }

    void removeItem(int index) {
        if (index >= 0 && index < size) {
            for (int i = index; i < size - 1; i++) {
                menuItems[i] = menuItems[i + 1];
            }
            size--; // Remove item
        } else {
            cout << "Invalid ID." << "\n";
        }
    }

    // Displays the menu in a table format, i usually dont go out of my way to make the ui look nice but is in rules
    void displayMenu() const {
        cout << "\n"<<"--- Current Menu ---"<<"\n";

        cout << left
             << setw(5)  << "ID"
             << setw(20) << "Name"
             << setw(15) << "Category"
             << setw(15) << "Nutrition"
             << setw(10) << "Price"<<"\n";

        cout << string(65, '-') << "\n"; // seprerator, just to look nice

        for (int i = 0; i < size; i++) { //big loop
            cout << setw(5)  << i + 1
                 << setw(20) << menuItems[i].name
                 << setw(15) << menuItems[i].type
                 << setw(15) << menuItems[i].nutrition
                 << "$" << fixed << setprecision(2)
                 << menuItems[i].price << "\n";
        }
    }

    // Searches menu items by name only (other filters could be added later when i have time)
    void searchByName(const string& query) const {
        auto start = chrono::high_resolution_clock::now(); // Start timing

        bool found = false;
        cout << "\nSearching for: " << query << "\n";

        for (int i = 0; i < size; i++) {
            if (menuItems[i].name.find(query) != string::npos) {
                cout << "Found: "
                     << menuItems[i].name << " ("
                     << menuItems[i].type << ", "
                     << menuItems[i].nutrition << ") - $"
                     << fixed << setprecision(2)
                     << menuItems[i].price << "\n";
                found = true;
            }
        }

        if (!found)
            cout << "Item not found." << "\n";

        auto end = chrono::high_resolution_clock::now(); // End timing

        cout << "Search time: "
             << chrono::duration_cast<chrono::milliseconds>(end - start).count()
             << " ms (O(N))" << "\n";
    }

    // sorting functs
    void sortAscending() { // lowest price first in menu
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (menuItems[j].price > menuItems[j + 1].price) {
                    MenuItem temp = menuItems[j];
                    menuItems[j] = menuItems[j + 1];
                    menuItems[j + 1] = temp;
                }
            }
        }
    }

    void sortDescending() { // highest price first in menu
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (menuItems[j].price < menuItems[j + 1].price) {
                    MenuItem temp = menuItems[j];
                    menuItems[j] = menuItems[j + 1];
                    menuItems[j + 1] = temp;
                }
            }
        }
    }
};

// linked list node stuffs
struct Node {
    string value; // value in node
    Node* next;   // pointer
};

// linked list, which i still don't think i added right even though i watched video to help me
class LinkedList {
private:
    Node* head; // start

public:
    LinkedList() {
        head = nullptr; // empty list
    }

    // add item, specifically at the end
    // come back to this
    void addItem(string value) {
        Node* newNode = new Node{value, nullptr};

        if (!head) { // if list empty, then this
            head = newNode;
            return;
        }

        Node* temp = head;

        while (temp->next) { // go to end
            temp = temp->next;
        }

        temp->next = newNode;
    }

    // remove specific item from list
    void removeItem(string value) {
        Node* temp = head;
        Node* prev = nullptr;

        while (temp) {
            if (temp->value == value) {

                if (!prev) { // removing head
                    head = temp->next;
                } else {
                    prev->next = temp->next;
                }

                delete temp;
                cout << "Item removed.\n";
                return;
            }

            prev = temp;
            temp = temp->next;
        }

        cout << "Item not found.\n";
    }

    // show all items in list
    void displayList() {
        DWORD start = GetTickCount(); // start timing

        Node* temp = head;

        cout << "\nLinked List Items:\n";

        while (temp) { // traversal
            cout << temp->value << "\n";
            temp = temp->next;
        }

        DWORD end = GetTickCount(); // end

        cout << "Traversal Time: " << (end - start)
             << " ms (O(N))\n";
    }

    // search, but for linked, and also it barely functions because lazy. come back to this
    void searchItem(string value) {

        DWORD start = GetTickCount(); // start timing search

        Node* temp = head;
        bool found = false;

        while (temp) {
            if (temp->value == value) {
                found = true;
                break;
            }
            temp = temp->next;
        }

        DWORD end = GetTickCount(); // end

        if (found)
            cout << "Item found.\n";
        else
            cout << "Item not found.\n";

        cout << "Search Time: " << (end - start)
             << " ms (O(N))\n";
    }
};

int main() {
    MenuManager manager; // Handles menu
    LinkedList list; // linked list

    int choice;

    do {
        //options for user, more added in project b
        cout << "\n" << "1. View Menu"
             << "\n" << "2. Add Item to Menu"
             << "\n" << "3. Remove Item from Menu"
             << "\n" << "4. Search for Menu Item"
             << "\n" << "5. Sort Menu By Price Ascending"
             << "\n" << "6. Sort Menu By Price Descending"
             << "\n" << "7. Add Item to Linked List"
             << "\n" << "8. Remove Item from Linked List"
             << "\n" << "9. Display Linked List"
             << "\n" << "10. Search Linked List"
             << "\n" << "11. Quit"
             << "\n" << "Enter choice: ";

        cin >> choice; //user choice
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input

        if (choice == 1) {
            manager.displayMenu();
        }
        else if (choice == 2) {
            MenuItem item;

            cout << "Enter name of food: ";
            getline(cin, item.name); //please capitalize so it looks nice

            cout << "Is it a main, side, dessert, or beverage: ";
            getline(cin, item.type); //could make up you own

            cout << "Is it healthy, unhealthy, or varies: ";
            getline(cin, item.nutrition); // if unsure, use brain

            cout << "Enter price of food: ";
            cin >> item.price; //i tried to make my prices realistic, but you can go nuts
            cin.ignore();

            manager.addItem(item); //add to array
            manager.saveToFile();
        }
        else if (choice == 3) {
            int id;
            cout << "Enter ID number to remove: "; //program will freak out if number below 1 or above item amount is put in
            cin >> id;
            cin.ignore();

            manager.removeItem(id - 1); // Fix IDs
            manager.saveToFile();
        }
        else if (choice == 4) {
            string name;
            cout << "Enter food name to search: "; // case sensitive, come back later
            getline(cin, name);

            manager.searchByName(name);
        }
        else if (choice == 5) {
            manager.sortAscending(); // lowest price first
            manager.displayMenu();
        }
        else if (choice == 6) {
            manager.sortDescending(); // highest price first
            manager.displayMenu();
        }
        else if (choice == 7) {
            string value;
            cout << "Enter item to add to linked list: ";
            getline(cin, value);
            list.addItem(value);
        }
        else if (choice == 8) {
            string value;
            cout << "Enter item to remove: ";
            getline(cin, value);
            list.removeItem(value);
        }
        else if (choice == 9) {
            list.displayList(); // move through linked list
        }
        else if (choice == 10) {
            string value;
            cout << "Enter item to search: ";
            getline(cin, value);
            list.searchItem(value);
        }
        else if (choice == 11) {
            manager.saveToFile();
            cout << "Exiting...\n";
        }
        else {
            cout << "Invalid choice." << "\n";
        }

    } while (choice != 11); //loops until program is quit

    return 0; // end program, probably knew that but every program code i see involves marking this
}
