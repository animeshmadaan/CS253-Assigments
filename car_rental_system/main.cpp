#include "classes.cpp"

int main() {
    readfile("customers.csv", customers_table);
    readfile("employees.csv", employees_table);
    readfile("cars.csv", cars_table);

    customers_row_num = customers_table.size(), employees_row_num = employees_table.size(), cars_row_num = cars_table.size();
    customers_col_num = customers_table[0].size(), employees_col_num = employees_table[0].size(), cars_col_num = cars_table[0].size();

    Manager m(1234, "manager", "JohnDoe", "DL123", "1234567890", 0, 0, 10, 0);

    make_objects();

    //system dependent function to clear the screen
    system("clear");

    cout << customers[0].ID << endl;

    cout << "--------------------------------------------------------\n";
    cout << "Welcome to the Car Rental System.\n";
    cout << "Enter '1' to login as a customer\n";
    cout << "Enter '2' to login as an employee\n";
    cout << "Enter '3' to login as a manager\n";
    cout << "Enter '4' to exit\n";
    cout << "--------------------------------------------------------\n";
    cout << "Input : ";
    int input;
    while (!(cin >> input)){
        cout << "Error: enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    while(input < 1 || input > 4) {
        cout << "Invalid input. Please enter a valid input.\n";
        cout << "Input : ";
        while (!(cin >> input)){
            cout << "Error: enter a number: ";
            cin.clear();
            cin.ignore(123, '\n');
        }
    }
    long long index;
    switch(input) {
        case 1:
            cout << "Enter ID : ";
            while (!(cin >> id)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter password : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, password);
            index = -1;
            for (long long i = 0; i < customers.size(); i++) {
                if (customers[i].ID == id) {
                    index = i;
                    break;
                }
            }
            if (index >= 0) {
                if (customers[index].verify(password)) {
                    cout << "Login successful.\n";
                    customers[index].display_options();
                }
                else cout << "Incorrect password.\n";
            }
            else cout << "ID not found.\n";
        break;
        case 2:
            cout << "Enter ID : ";
            while (!(cin >> id)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter password : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, password);
            index = -1;
            for (long long i = 0; i < employees.size(); i++) {
                if (employees[i].ID == id) {
                    index = i;
                    break;
                }
            }
            if (index >= 0) {
                if (employees[index].verify(password)) {
                    cout << "Login successful.\n";
                    employees[index].display_options();
                }
                else cout << "Incorrect password.\n";
            }
            else cout << "ID not found.\n";
        break;
        case 3:
            cout << "Enter ID : ";
            while (!(cin >> id)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter password : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, password);
            if (id == m.ID && m.authenticate(password)) {
                cout << "Login successful.\n";
                m.display_options();
            }
            else cout << "Incorrect ID or password.\n";
        break;
        case 4:
        break;
    }
    update_tables();

    writefile(customers_table, "customers.csv");
    writefile(employees_table, "employees.csv");
    writefile(cars_table, "cars.csv");
    return 0;
}