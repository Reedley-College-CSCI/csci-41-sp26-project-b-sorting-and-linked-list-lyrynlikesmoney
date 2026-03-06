//did need to look things up, memory is terrible
//added all of theses before started, dont know if needed yet
#include <iostream>
#include <fstream>
#include <string> //still think this is dumb
#include <iomanip>
#include <chrono> //easier than other options imo
#include <sstream>
#include <limits> // basically redid everything involving this because not allowed to use vector

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
        ifstream file(filename); //opens text file

        // If the file can't be opened, get notified, not sure it works but no errs have appeared
        if (!file) {
            cout << "Missing or wrong file, we want menu.txt" << endl;
            return;
        }

        size = 0; // Clear items just in case

        string line;
        while (getline(file, line)) {
            string name, type, nutrition, priceStr;
            stringstream ss(line);

            // commas seperate things on text file
            getline(ss, name, ',');
            getline(ss, type, ',');
            getline(ss, nutrition, ',');
            getline(ss, priceStr);

            try { //reworked
                double price = stod(priceStr);

                if (size >= capacity)
                    resizeArray(); // the array stuff took too long to figure out

                menuItems[size++] = {name, type, nutrition, price}; // Add item
            } catch (...) {
                // Skip invalid lines if any are there
            }
        }
    }

    void saveToFile() {
        ofstream file(filename); // will be overwritten. only remove items you added please

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

    // Displays the menu in a table format, i usually dont go out of my way to make the ui look nice but i want a solid grade
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
};

int main() {
    MenuManager manager; // Handles menu
    int choice;

    do {
        //options for user
        cout << "\n" << "1. View Menu"
             << "\n" << "2. Add Item to Menu"
             << "\n" << "3. Remove Item from Menu"
             << "\n" << "4. Search for Menu Item"
             << "\n" << "5. Quit"
             << "\n" << "Enter choice: ";

        cin >> choice; //user choice
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input

        if (choice == 1) {
            manager.displayMenu();
        }
        else if (choice == 2) {
            MenuItem item;

            cout << "Enter name of food: ";
            getline(cin, item.name);

            cout << "Is it a main, side, dessert, or beverage: ";
            getline(cin, item.type);

            cout << "Is it healthy, unhealthy, or varies: ";
            getline(cin, item.nutrition);

            cout << "Enter price of food: ";
            cin >> item.price;
            cin.ignore();

            manager.addItem(item); //add to array
            manager.saveToFile();
        }
        else if (choice == 3) {
            int id;
            cout << "Enter ID number to remove: ";
            cin >> id;
            cin.ignore();

            manager.removeItem(id - 1); // Fix IDs
            manager.saveToFile();
        }
        else if (choice == 4) {
            string name;
            cout << "Enter food name to search: ";
            getline(cin, name);

            manager.searchByName(name);
        }
        else if (choice == 5) {
            manager.saveToFile();
            cout << "Exiting...\n";
        }
        else {
            cout << "Invalid choice." << "\n";
        }

    } while (choice != 5); //loops until program is quit

    return 0; // end program, probably knew that but every program code i see involves marking this
}
