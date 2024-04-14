#include "read_write.cpp"
#include "time.cpp"

void outputPaddedString(const string& str, int maxLength) {
    string substr = str.substr(0, maxLength);

    cout << setw(maxLength) << left << substr << ' ';
}

/*Methods for User*/

void User::update_password(string new_password) {
    Password = new_password;
}

bool User::verify(string password) {
    if (Password != password) return false;
    return true;
}




/*Methods for Customer*/

Customer::Customer (long long i, const string& p, const string& n, const string& dl, const string &contact, long long pd, long long fd, long long tr, long long ncr) {
    ID = i;
    Password = p;
    Name = n;
    DL = dl;
    Contact = contact;
    Payment_Due = pd;
    Fine_Due = fd;
    track_record = tr;
    num_cars_rented = ncr;
}

void Customer::display_options() {
    cout << "Welcome" << ' ' << Name << "!\n";
    int input = 0;
    while (input != 8) {
        cout << "--------------------------------------------------------\n";
        cout << "Enter '1' to see all available cars.\n";
        cout << "Enter '2' to rent a car.\n";
        cout << "Enter '3' to see cars you are currently renting.\n";
        cout << "Enter '4' to return a car.\n";
        cout << "Enter '5' to display all dues.\n";
        cout << "Enter '6' to clear dues.\n";
        cout << "Enter '7' to update profile.\n";
        cout << "Enter '8' to exit\n";
        cout << "--------------------------------------------------------\n";
        cout << "Input : ";
        while (!(cin >> input)) {
            cout << "Error: Enter a number: ";
            cin.clear();
            cin.ignore(123, '\n');
        }
        while(input < 1 || input > 8) {
            cout << "Invalid input. Please enter a valid input.\n";
            cout << "Input : ";
            while (!(cin >> input)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
        }
        switch(input) {
            case 1:
                display_available_cars();
                break;
            case 2:
                rent_car();
                break;
            case 3:
                display_curr_rented_cars();
                break;
            case 4:
                return_car();
                break;
            case 5:
                display_dues();
                break;
            case 6:
                clear_dues();
                break;
            case 7:
                update_profile();
                break;
            case 8:
                return;
                break;
        }
    }
}

void Customer::display_available_cars() {
    cout << "--------------------------------------------------------\n";
    cout << "Available cars:\n";
    outputPaddedString("Car ID", car_col_width[0]);
    outputPaddedString("Make", car_col_width[2]);
    outputPaddedString("Model", car_col_width[3]);
    outputPaddedString("Seating Capacity", car_col_width[5]);
    outputPaddedString("Rent Per Day", car_col_width[10]);
    outputPaddedString("Fine Per Day", car_col_width[11]);
    cout << "\n";
    for (int i = 0; i < cars.size(); i++) {
        if (cars[i].Status == "yes") continue;
        outputPaddedString(to_string(cars[i].ID), car_col_width[0]);
        outputPaddedString(cars[i].Make, car_col_width[2]);
        outputPaddedString(cars[i].Model, car_col_width[3]);
        outputPaddedString(to_string(cars[i].Seating_Capacity), car_col_width[5]);
        outputPaddedString(to_string(cars[i].Rent_Per_Day), car_col_width[10]);
        outputPaddedString(to_string(cars[i].Fine_Per_Day), car_col_width[11]);
        cout << '\n';
    }
    cout << "--------------------------------------------------------\n";
    cout << "To see more details of a car, enter the car ID or enter 0 to continue: ";
    long long car_id;
    while(!(cin >> car_id)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    for (int i = 0; i < cars.size(); i++) {
        if (cars[i].ID == car_id) {
            cars[i].display_car_details();
            return;
        }
    }
    cout << "Car not found.\n";
}

void Customer::rent_car() {
    if (track_record == 0) {
        cout << "Ugh, your renting history totally doesn't cut it.\n";
        cout << "Pay up 5000 rupees if you wanna keep renting wheels from us.\n";
        cout << "Do you agree to pay the fine? (y/n) : ";
        string ans;
        cin.clear(), cin.ignore(1000, '\n'), getline(cin, ans);
        while(ans != "y" && ans != "n") {
            cout << "Invalid input. Please enter a valid input.\n";
            cout << "Do you agree to pay the fine? (y/n) : ";
            getline(cin, ans);
        }
        if (ans == "y") {
            track_record = 5;
            cout << "Fine paid successfully.\n";
        }
        else
        return;
    }
    if (num_cars_rented == track_record) {
        cout << "You have reached the maximum limit of cars you can rent.\n";
        return;
    }
    cout << "Enter the car ID you want to rent : ";
    long long car_id;
    while(!(cin >> car_id)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    long long car_ind = -1;
    for (int i = 0; i < cars.size(); i++) {
        if (cars[i].ID == car_id) {
            car_ind = i;
        }
    }
    if (car_ind == -1) {
        cout << "Car not found.\n";
        return;
    }
    if (cars[car_ind].Status == "yes") {
        cout << "Car is not available.\n";
        return;
    }
    string rented_on = getCurrentDate();
    cout << "Enter the date you want to return the car (DD-MM-YYYY) : ";
    string due_date;
    cin.clear(), cin.ignore(1000, '\n'), getline(cin, due_date);
    while (parse_time(due_date) <= parse_time(rented_on)) {
        cout << "Invalid date. Please enter a valid date.\n";
        cout << "Enter the date you want to return the car (DD-MM-YYYY) : ";
        getline(cin, due_date);
    }
    cars[car_ind].Status = "yes";
    cars[car_ind].Rented_On = rented_on;
    cars[car_ind].Due_Date = due_date;
    cars[car_ind].User_Type = "Customer";
    cars[car_ind].User_ID = ID;
    num_cars_rented++;
    Payment_Due += daysBetween(rented_on, due_date)*cars[car_ind].Rent_Per_Day;
    cout << "Car rented successfully.\n";
}

void Customer::return_car() {
    if (num_cars_rented == 0) {
        cout << "You are not renting any cars.\n";
        return;
    }
    cout << "Enter the car ID you want to return : ";
    long long car_id;
    while(!(cin >> car_id)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    for (int i = 0; i < cars.size(); i++) {
        if (cars[i].ID == car_id) {
            if (cars[i].Status == "no" || cars[i].User_ID != ID || cars[i].User_Type != "Customer"){
                cout << "Car is not rented by you.\n";
                return;
            }
            string returned_on;
            string due_date = cars[i].Due_Date;
            string rented_on = cars[i].Rented_On;
            cout << "Enter the date you are returning the car (DD-MM-YYYY) : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, returned_on);
            while (parse_time(returned_on) <= parse_time(rented_on)) {
                cout << "Invalid date. Please enter a valid date.\n";
                cout << "Enter the date you are returning the car (DD-MM-YYYY) : ";
                getline(cin, returned_on);
            }
            if (daysBetween(due_date, returned_on) > 0) {
                Fine_Due += (daysBetween(due_date, returned_on))*cars[i].Fine_Per_Day;
            }
            cars[i].Status = "no";
            cars[i].Rented_On = "";
            cars[i].Due_Date = "";
            cars[i].User_Type = "";
            cars[i].User_ID = 0;
            num_cars_rented--;
            cout << "Damage determined by the employee on a scale of 0-5: ";
            int damage;
            while(!(cin >> damage)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            while (damage < 0 || damage > 5) {
                cout << "Invalid input. Please enter a valid input.\n";
                cout << "Damage determined by the employee on a scale of 0-5: ";
                while(!(cin >> damage)) {
                    cout << "Error: Enter a number: ";
                    cin.clear();
                    cin.ignore(123, '\n');
                }
            }
            if (damage == 0) {
                if (track_record < 10) track_record++;
            }
            else track_record -= damage;
            if (track_record < 0) track_record = 0;
            cars[i].Condition -= damage;
            if (cars[i].Condition < 0) cars[i].Condition = 0;
            cout << "Kilometers driven: ";
            long long km;
            while(!(cin >> km)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            while (km < 0) {
                cout << "Invalid input. Please enter a valid input.\n";
                cout << "Kilometers driven: ";
                while(!(cin >> km)) {
                    cout << "Error: Enter a number: ";
                    cin.clear();
                    cin.ignore(123, '\n');
                }
            }
            cars[i].Kilometers_Driven += km;
            cout << "Car returned successfully.\n";
            return;
        }
    }
    cout << "Car not found.\n";
}

void Customer::display_curr_rented_cars() {
    cout << "--------------------------------------------------------\n";
    if (num_cars_rented == 0) {
        cout << "You are not renting any cars.\n";
        return;
    }
    cout << "Cars you are currently renting: ";
    cout << num_cars_rented << "\n";
    outputPaddedString("Car ID", car_col_width[0]);
    outputPaddedString("License Plate", car_col_width[1]);
    outputPaddedString("Make", car_col_width[2]);
    outputPaddedString("Model", car_col_width[3]);
    outputPaddedString("Rented On", car_col_width[13]);
    outputPaddedString("Due Date", car_col_width[14]);
    cout << "\n";
    for (long long i = 0; i < cars.size(); i++) {
        if (cars[i].Status == "yes" && cars[i].User_ID == ID && cars[i].User_Type == "Customer") {
            outputPaddedString(to_string(cars[i].ID), car_col_width[0]);
            outputPaddedString(cars[i].License_Plate , car_col_width[1]);
            outputPaddedString(cars[i].Make, car_col_width[2]);
            outputPaddedString(cars[i].Model, car_col_width[3]);
            outputPaddedString(cars[i].Rented_On, car_col_width[13]);
            outputPaddedString(cars[i].Due_Date, car_col_width[14]);
            cout << '\n';
        }
    }
    cout << "--------------------------------------------------------\n";
}

void Customer::display_dues() {
    cout << "--------------------------------------------------------\n";
    cout << "Payment due: " << Payment_Due << "\n";
    cout << "Fine due: " << Fine_Due << "\n";
    cout << "Total dues: " << Payment_Due + Fine_Due << "\n";
    cout << "--------------------------------------------------------\n";
}

void Customer::clear_dues() {
    cout << "--------------------------------------------------------\n";
    cout << "Enter '1' to clear payment due.\n";
    cout << "Enter '2' to clear fine due.\n";
    cout << "Enter '3' to clear both.\n";
    cout << "Enter '4' to exit.\n";
    cout << "--------------------------------------------------------\n";
    cout << "Input : ";
    int input;
    while(!(cin >> input)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    while(input < 1 || input > 4) {
        cout << "Invalid input. Please enter a valid input.\n";
        cout << "Input : ";
        while(!(cin >> input)) {
            cout << "Error: Enter a number: ";
            cin.clear();
            cin.ignore(123, '\n');
        }
    }
    string ans;
    switch(input) {
        case 1:
            cout << "Do you want to clear payment due of " << Payment_Due << " ? (y/n) : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, ans);
            while(ans != "y" && ans != "n") {
                cout << "Invalid input. Please enter a valid input.\n";
                cout << "Do you want to clear payment due of " << Payment_Due << " ? (y/n) : ";
                getline(cin, ans);
            }
            if (ans == "y") {
                Payment_Due = 0;
            }
            break;
        case 2:
            cout << "Do you want to clear fine due of " << Fine_Due << " ? (y/n) : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, ans);
            while(ans != "y" && ans != "n") {
                cout << "Invalid input. Please enter a valid input.\n";
                cout << "Do you want to clear fine due of " << Fine_Due << " ? (y/n) : ";
                getline(cin, ans);
            }
            if (ans == "y") {
                Fine_Due = 0;
            }
            break;
        case 3:
            cout << "Do you want to clear total due of " << Payment_Due + Fine_Due << " ? (y/n) : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, ans);
            while(ans != "y" && ans != "n") {
                cout << "Invalid input. Please enter a valid input.\n";
                cout << "Do you want to clear total due of " << Payment_Due + Fine_Due << " ? (y/n) : ";
                getline(cin, ans);
            }
            if (ans == "y") {
                Payment_Due = 0;
                Fine_Due = 0;
            }
            break;
        case 4:
            return;
            break;
    }
    cout << "Dues cleared.\n";
    cout << "--------------------------------------------------------\n";
}

void Customer::update_profile() {
    cout << "--------------------------------------------------------\n";
    cout << "Enter '1' to update name.\n";
    cout << "Enter '2' to update password.\n";
    cout << "Enter '3' to update driving license number.\n";
    cout << "Enter '4' to update contact.\n";
    cout << "Enter '5' to exit.\n";
    cout << "--------------------------------------------------------\n";
    cout << "Input : ";
    int input;
    while(!(cin >> input)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    while(input < 1 || input > 5) {
        cout << "Invalid input. Please enter a valid input.\n";
        cout << "Input : ";
        while(!(cin >> input)) {
            cout << "Error: Enter a number: ";
            cin.clear();
            cin.ignore(123, '\n');
        }
    }
    switch(input) {
        case 1:
            cout << "Enter new name : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, Name);
            break;
        case 2:
            cout << "Enter new password : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, Password);
            break;
        case 3:
            cout << "Enter new driving license : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, DL);
            break;
        case 4:
            cout << "Enter new contact : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, Contact);
            break;
        case 5:
            return;
            break;
    }
    cout << "Profile updated.\n";
    cout << "--------------------------------------------------------\n";
}

string Customer::get_pass() {
    return Password;
}







/*Methods for Employee*/

Employee::Employee (long long i, const string& p, const string& n, const string& dl, const string &contact, long long pd, long long fd, long long tr, long long ncr){
    ID = i;
    Password = p;
    Name = n;
    DL = dl;
    Contact = contact;
    Payment_Due = pd;
    Fine_Due = fd;
    track_record = tr;
    num_cars_rented = ncr;
}

void Employee::display_options() {
    cout << "Welcome" << ' ' << Name << "!\n";
    int input = 0;
    while (input != 8) {
        cout << "--------------------------------------------------------\n";
        cout << "Enter '1' to see all available cars.\n";
        cout << "Enter '2' to rent a car.\n";
        cout << "Enter '3' to see cars you are currently renting.\n";
        cout << "Enter '4' to return a car.\n";
        cout << "Enter '5' to display all dues.\n";
        cout << "Enter '6' to clear dues.\n";
        cout << "Enter '7' to update profile.\n";
        cout << "Enter '8' to exit\n";
        cout << "Since you are an employee, you will get a 15 precent discount on the rent.\n";
        cout << "--------------------------------------------------------\n";
        cout << "Input : ";
        while (!(cin >> input)) {
            cout << "Error: Enter a number: ";
            cin.clear();
            cin.ignore(123, '\n');
        }
        while(input < 1 || input > 8) {
            cout << "Invalid input. Please enter a valid input.\n";
            cout << "Input : ";
            while (!(cin >> input)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
        }
        switch(input) {
            case 1:
                display_available_cars();
                break;
            case 2:
                rent_car();
                break;
            case 3:
                display_curr_rented_cars();
                break;
            case 4:
                return_car();
                break;
            case 5:
                display_dues();
                break;
            case 6:
                clear_dues();
                break;
            case 7:
                update_profile();
                break;
            case 8:
                return;
                break;
        }
    }
}

void Employee::display_available_cars() {
    cout << "--------------------------------------------------------\n";
    cout << "Available cars:\n";
    outputPaddedString("Car ID", car_col_width[0]);
    outputPaddedString("Make", car_col_width[2]);
    outputPaddedString("Model", car_col_width[3]);
    outputPaddedString("Seating Capacity", car_col_width[5]);
    outputPaddedString("Rent Per Day", car_col_width[10]);
    outputPaddedString("Fine Per Day", car_col_width[11]);
    cout << "\n";
    for (int i = 0; i < cars.size(); i++) {
        if (cars[i].Status == "yes") continue;
        outputPaddedString(to_string(cars[i].ID), car_col_width[0]);
        outputPaddedString(cars[i].Make, car_col_width[2]);
        outputPaddedString(cars[i].Model, car_col_width[3]);
        outputPaddedString(to_string(cars[i].Seating_Capacity), car_col_width[5]);
        outputPaddedString(to_string(cars[i].Rent_Per_Day), car_col_width[10]);
        outputPaddedString(to_string(cars[i].Fine_Per_Day), car_col_width[11]);
        cout << '\n';
    }
    cout << "--------------------------------------------------------\n";
    cout << "To see more details of a car, enter the car ID or enter 0 to continue: ";
    long long car_id;
    while(!(cin >> car_id)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    for (int i = 0; i < cars.size(); i++) {
        if (cars[i].ID == car_id) {
            cars[i].display_car_details();
            return;
        }
    }
    cout << "Car not found.\n";
}

void Employee::rent_car() {
    if (track_record == 0) {
        cout << "Ugh, your renting history totally doesn't cut it.\n";
        cout << "Pay up 5000 rupees if you wanna keep renting wheels from us.\n";
        cout << "Do you agree to pay the fine? (y/n) : ";
        string ans;
        cin.clear(), cin.ignore(1000, '\n'), getline(cin, ans);
        while(ans != "y" && ans != "n") {
            cout << "Invalid input. Please enter a valid input.\n";
            cout << "Do you agree to pay the fine? (y/n) : ";
            getline(cin, ans);
        }
        if (ans == "y") {
            track_record = 5;
            cout << "Fine paid successfully.\n";
        }
        else
        return;
    }
    if (num_cars_rented == track_record) {
        cout << "You have reached the maximum limit of cars you can rent.\n";
        return;
    }
    cout << "Enter the car ID you want to rent : ";
    long long car_id, car_ind = -1;
    while(!(cin >> car_id)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    for (int i = 0; i < cars.size(); i++) {
        if (cars[i].ID == car_id) {
            car_ind = i;
        }
    }
    if (car_ind == -1) {
        cout << "Car not found.\n";
        return;
    }
    if (cars[car_ind].Status == "yes") {
        cout << "Car is not available.\n";
        return;
    }
    string rented_on = getCurrentDate();
    cout << "Enter the date you want to return the car (DD-MM-YYYY) : ";
    string due_date;
    cin.clear(), cin.ignore(1000, '\n'), getline(cin, due_date);
    while (parse_time(due_date) <= parse_time(rented_on)) {
        cout << "Invalid date. Please enter a valid date.\n";
        cout << "Enter the date you want to return the car (DD-MM-YYYY) : ";
        getline(cin, due_date);
    }
    cars[car_ind].Status = "yes";
    cars[car_ind].Rented_On = rented_on;
    cars[car_ind].Due_Date = due_date;
    cars[car_ind].User_Type = "Employee";
    cars[car_ind].User_ID = ID;
    num_cars_rented++;
    Payment_Due += (85*(daysBetween(rented_on, due_date))*cars[car_ind].Rent_Per_Day)/100;
    cout << "Car rented successfully.\n";
}

void Employee::return_car() {
    if (num_cars_rented == 0) {
        cout << "You are not renting any cars.\n";
        return;
    }
    cout << "Enter the car ID you want to return : ";
    long long car_id;
    while(!(cin >> car_id)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    for (int i = 0; i < cars.size(); i++) {
        if (cars[i].ID == car_id) {
            if (cars[i].Status == "no" || cars[i].User_ID != ID || cars[i].User_Type != "Employee"){
                cout << "Car is not rented by you.\n";
                return;
            }
            string returned_on;
            string due_date = cars[i].Due_Date;
            string rented_on = cars[i].Rented_On;
            cout << "Enter the date you are returning the car (DD-MM-YYYY) : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, returned_on);
            while (parse_time(returned_on) <= parse_time(rented_on)) {
                cout << "Invalid date. Please enter a valid date.\n";
                cout << "Enter the date you are returning the car (DD-MM-YYYY) : ";
                getline(cin, returned_on);
            }
            if (daysBetween(due_date, returned_on) > 0) {
                Fine_Due += (85*(daysBetween(due_date, returned_on))*cars[i].Fine_Per_Day)/100;
            }
            cars[i].Status = "no";
            cars[i].Rented_On = "";
            cars[i].Due_Date = "";
            cars[i].User_Type = "";
            cars[i].User_ID = 0;
            num_cars_rented--;
            cout << "Damage determined by the employee on a scale of 0-5: ";
            int damage;
            while(!(cin >> damage)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            while (damage < 0 || damage > 5) {
                cout << "Invalid input. Please enter a valid input.\n";
                cout << "Damage determined by the employee on a scale of 0-5: ";
                while(!(cin >> damage)) {
                    cout << "Error: Enter a number: ";
                    cin.clear();
                    cin.ignore(123, '\n');
                }
            }
            if (damage == 0) {
                if (track_record < 10) track_record++;
            }
            else track_record -= damage;
            if (track_record < 0) track_record = 0;
            cars[i].Condition -= damage;
            if (cars[i].Condition < 0) cars[i].Condition = 0;
            cout << "Kilometers driven: ";
            long long km;
            while(!(cin >> km)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            while (km < 0) {
                cout << "Invalid input. Please enter a valid input.\n";
                cout << "Kilometers driven: ";
                while(!(cin >> km)) {
                    cout << "Error: Enter a number: ";
                    cin.clear();
                    cin.ignore(123, '\n');
                }
            }
            cars[i].Kilometers_Driven += km;
            cout << "Car returned successfully.\n";
            return;
        }
    }
    cout << "Car not found.\n";
}

void Employee::display_curr_rented_cars() {
    cout << "--------------------------------------------------------\n";
    if (num_cars_rented == 0) {
        cout << "You are not renting any cars.\n";
        return;
    }
    cout << "Cars you are currently renting: ";
    cout << num_cars_rented << "\n";
    outputPaddedString("Car ID", car_col_width[0]);
    outputPaddedString("License Plate", car_col_width[1]);
    outputPaddedString("Make", car_col_width[2]);
    outputPaddedString("Model", car_col_width[3]);
    outputPaddedString("Rented On", car_col_width[13]);
    outputPaddedString("Due Date", car_col_width[14]);
    cout << "\n";
    for (long long i = 0; i < cars.size(); i++) {
        if (cars[i].Status == "yes" && cars[i].User_ID == ID && cars[i].User_Type == "Customer") {
            outputPaddedString(to_string(cars[i].ID), car_col_width[0]);
            outputPaddedString(cars[i].License_Plate , car_col_width[1]);
            outputPaddedString(cars[i].Make, car_col_width[2]);
            outputPaddedString(cars[i].Model, car_col_width[3]);
            outputPaddedString(cars[i].Rented_On, car_col_width[13]);
            outputPaddedString(cars[i].Due_Date, car_col_width[14]);
            cout << '\n';
        }
    }
    cout << "--------------------------------------------------------\n";
}

void Employee::display_dues() {
    cout << "--------------------------------------------------------\n";
    cout << "Payment due: " << Payment_Due << "\n";
    cout << "Fine due: " << Fine_Due << "\n";
    cout << "Total dues: " << Payment_Due + Fine_Due << "\n";
    cout << "--------------------------------------------------------\n";
}

void Employee::clear_dues() {
    cout << "--------------------------------------------------------\n";
    cout << "Enter '1' to clear payment due.\n";
    cout << "Enter '2' to clear fine due.\n";
    cout << "Enter '3' to clear both.\n";
    cout << "Enter '4' to exit.\n";
    cout << "--------------------------------------------------------\n";
    cout << "Input : ";
    int input;
    while(!(cin >> input)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    while(input < 1 || input > 4) {
        cout << "Invalid input. Please enter a valid input.\n";
        cout << "Input : ";
        while(!(cin >> input)) {
            cout << "Error: Enter a number: ";
            cin.clear();
            cin.ignore(123, '\n');
        }
    }
    string ans;
    switch(input) {
        case 1:
            cout << "Do you want to clear payment due of " << Payment_Due << " ? (y/n) : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, ans);
            while(ans != "y" && ans != "n") {
                cout << "Invalid input. Please enter a valid input.\n";
                cout << "Do you want to clear payment due of " << Payment_Due << " ? (y/n) : ";
                getline(cin, ans);
            }
            if (ans == "y") {
                Payment_Due = 0;
            }
            break;
        case 2:
            cout << "Do you want to clear fine due of " << Fine_Due << " ? (y/n) : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, ans);
            while(ans != "y" && ans != "n") {
                cout << "Invalid input. Please enter a valid input.\n";
                cout << "Do you want to clear fine due of " << Fine_Due << " ? (y/n) : ";
                getline(cin, ans);
            }
            if (ans == "y") {
                Fine_Due = 0;
            }
            break;
        case 3:
            cout << "Do you want to clear total due of " << Payment_Due + Fine_Due << " ? (y/n) : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, ans);
            while(ans != "y" && ans != "n") {
                cout << "Invalid input. Please enter a valid input.\n";
                cout << "Do you want to clear total due of " << Payment_Due + Fine_Due << " ? (y/n) : ";
                getline(cin, ans);
            }
            if (ans == "y") {
                Payment_Due = 0;
                Fine_Due = 0;
            }
            break;
        case 4:
            return;
            break;
    }
    cout << "Dues cleared.\n";
    cout << "--------------------------------------------------------\n";
}

void Employee::update_profile() {
    cout << "--------------------------------------------------------\n";
    cout << "Enter '1' to update name.\n";
    cout << "Enter '2' to update password.\n";
    cout << "Enter '3' to update driving license number.\n";
    cout << "Enter '4' to update contact.\n";
    cout << "Enter '5' to exit.\n";
    cout << "--------------------------------------------------------\n";
    cout << "Input : ";
    int input;
    while(!(cin >> input)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    while(input < 1 || input > 5) {
        cout << "Invalid input. Please enter a valid input.\n";
        cout << "Input : ";
        while(!(cin >> input)) {
            cout << "Error: Enter a number: ";
            cin.clear();
            cin.ignore(123, '\n');
        }
    }
    switch(input) {
        case 1:
            cout << "Enter new name : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, Name);
            break;
        case 2:
            cout << "Enter new password : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, Password);
            break;
        case 3:
            cout << "Enter new driving license : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, DL);
            break;
        case 4:
            cout << "Enter new contact : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, Contact);
            break;
        case 5:
            return;
            break;
    }
    cout << "Profile updated.\n";
    cout << "--------------------------------------------------------\n";
}

string Employee::get_pass() {
    return Password;
}






/*Methods for Manger*/

Manager::Manager (long long i, const string& p, const string& n, const string& dl, const string &contact, long long pd, long long fd, long long tr, long long ncr){
    ID = i;
    Password = p;
    Name = n;
    DL = dl;
    Contact = contact;
    Payment_Due = pd;
    Fine_Due = fd;
    track_record = tr;
    num_cars_rented = ncr;
}

void Manager::display_options() {
    cout << "Welcome" << ' ' << Name << "!\n";
    int input = 0;
    while(input != 4) {
        cout << "--------------------------------------------------------\n";
        cout << "Enter '1' to manage customers.\n";
        cout << "Enter '2' to manage employees.\n";
        cout << "Enter '3' to manage cars.\n";
        cout << "Enter '4' to exit\n";
        cout << "--------------------------------------------------------\n";
        cout << "Input : ";
        while (!(cin >> input)) {
            cout << "Error: Enter a number: ";
            cin.clear();
            cin.ignore(123, '\n');
        }
        while(input < 1 || input > 4) {
            cout << "Invalid input. Please enter a valid input.\n";
            cout << "Input : ";
            while (!(cin >> input)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
        }
        switch(input) {
            case 1:
                manage_customers();
                break;
            case 2:
                manage_employees();
                break;
            case 3:
                manage_cars();
                break;
            case 4:
                return;
                break;
        }
    }
}

void Manager::manage_customers() {
    cout << "--------------------------------------------------------\n";
    outputPaddedString("Customer ID", customer_col_width[0]);
    outputPaddedString("Name", customer_col_width[2]);
    outputPaddedString("Driving License", customer_col_width[3]);
    outputPaddedString("Contact", customer_col_width[4]);
    outputPaddedString("Payment Due", customer_col_width[5]);
    outputPaddedString("Fine Due", customer_col_width[6]);
    outputPaddedString("Track Record", customer_col_width[7]);
    outputPaddedString("Cars Rented", customer_col_width[8]);
    cout << "\n";
    for (long long i = 0; i < customers.size(); i++) {
        outputPaddedString(to_string(customers[i].ID), customer_col_width[0]);
        outputPaddedString(customers[i].Name, customer_col_width[2]);
        outputPaddedString(customers[i].DL, customer_col_width[3]);
        outputPaddedString(customers[i].Contact, customer_col_width[4]);
        outputPaddedString(to_string(customers[i].Payment_Due), customer_col_width[5]);
        outputPaddedString(to_string(customers[i].Fine_Due), customer_col_width[6]);
        outputPaddedString(to_string(customers[i].track_record), customer_col_width[7]);
        outputPaddedString(to_string(customers[i].num_cars_rented), customer_col_width[8]);
        cout << '\n'; 
    }
    cout << "--------------------------------------------------------\n";
    cout << "Enter '1' to add a customer.\n";
    cout << "Enter '2' to update a customer.\n";
    cout << "Enter '3' to delete a customer.\n";
    cout << "Enter '4' to exit.\n";
    cout << "--------------------------------------------------------\n";
    cout << "Input : ";
    int input;
    while(!(cin >> input)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    while(input < 1 || input > 4) {
        cout << "Invalid input. Please enter a valid input.\n";
        cout << "Input : ";
        while(!(cin >> input)) {
            cout << "Error: Enter a number: ";
            cin.clear();
            cin.ignore(123, '\n');
        }
    }
    string name, password, dl, contact;
    long long ip;
    Customer c(0, "", "", "", "", 0, 0, 5, 0);
    switch(input) {
        case 1:
            if (customers.size() == 0) id = 1;
            else id = (customers.end() - 1)->ID + 1;
            cout << "Enter Password : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, password);
            cout << "Enter Name : ";
            getline(cin, name);
            cout << "Enter Driving License : ";
            getline(cin, dl);
            cout << "Enter Contact : ";
            getline(cin, contact);
            c.ID = id;
            c.update_password(password);
            c.Name = name;
            c.DL = dl;
            c.Contact = contact;
            customers.push_back(c);
            break;
        case 2:
            cout << "Enter ID : ";
            while (!(cin >> id)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            for (long long i = 0; i < customers.size(); i++) {
                if (customers[i].ID == id) {
                    cout << "--------------------------------------------------------\n";
                    cout << "Enter '1' to update name.\n";
                    cout << "Enter '2' to update password.\n";
                    cout << "Enter '3' to update driving license number.\n";
                    cout << "Enter '4' to update contact.\n";
                    cout << "Enter '5' to exit.\n";
                    cout << "--------------------------------------------------------\n";
                    cout << "Input : ";
                    while(!(cin >> ip)) {
                        cout << "Error: Enter a number: ";
                        cin.clear();
                        cin.ignore(123, '\n');
                    }
                    while(ip < 1 || ip > 5) {
                        cout << "Invalid input. Please enter a valid input.\n";
                        cout << "Input : ";
                        while(!(cin >> ip)) {
                            cout << "Error: Enter a number: ";
                            cin.clear();
                            cin.ignore(123, '\n');
                        }
                    }
                    switch(ip) {
                        case 1:
                            cout << "Enter new name : ";
                            cin.clear(), cin.ignore(1000, '\n'), getline(cin, name);
                            customers[i].Name = name;
                            break;
                        case 2:
                            cout << "Enter new password : ";
                            cin.clear(), cin.ignore(1000, '\n'), getline(cin, password);
                            customers[i].update_password(password);
                            break;
                        case 3:
                            cout << "Enter new driving license : ";
                            cin.clear(), cin.ignore(1000, '\n'), getline(cin, dl);
                            customers[i].DL = dl;
                            break;
                        case 4:
                            cout << "Enter new contact : ";
                            cin.clear(), cin.ignore(1000, '\n'), getline(cin, contact);
                            customers[i].Contact = contact;
                            break;
                        case 5:
                            return;
                            break;
                    }
                    cout << "Profile updated.\n";
                    return;
                }
            }
            cout << "ID not found.\n";
            break;
        case 3:
            cout << "Enter ID : ";
            long long id;
            while (!(cin >> id)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            for (long long i = 0; i < customers.size(); i++) {
                if (customers[i].ID == id) {
                    for (long long j = 0; j < cars.size(); j++) {
                        if (cars[j].Status == "yes" && cars[j].User_ID == id && cars[j].User_Type == "Customer") {
                            cars[j].Status = "no";
                            cars[j].Rented_On = "";
                            cars[j].Due_Date = "";
                            cars[j].User_Type = "";
                            cars[j].User_ID = 0;
                        } 
                    }
                    customers.erase(customers.begin() + i);
                    cout << "Customer deleted.\n";
                    return;
                }
            }
            cout << "ID not found.\n";
            break;
        case 4:
            return;
            break;
    }
}

void Manager::manage_employees() {
    cout << "--------------------------------------------------------\n";
    outputPaddedString("Employee ID", employee_col_width[0]);
    outputPaddedString("Name", employee_col_width[2]);
    outputPaddedString("Driving License", employee_col_width[3]);
    outputPaddedString("Contact", employee_col_width[4]);
    outputPaddedString("Payment Due", employee_col_width[5]);
    outputPaddedString("Fine Due", employee_col_width[6]);
    outputPaddedString("Track Record", employee_col_width[7]);
    outputPaddedString("Cars Rented", employee_col_width[8]);
    cout << "\n";
    for (long long i = 0; i < employees.size(); i++) {
        outputPaddedString(to_string(employees[i].ID), employee_col_width[0]);
        outputPaddedString(employees[i].Name, employee_col_width[2]);
        outputPaddedString(employees[i].DL, employee_col_width[3]);
        outputPaddedString(employees[i].Contact, employee_col_width[4]);
        outputPaddedString(to_string(employees[i].Payment_Due), employee_col_width[5]);
        outputPaddedString(to_string(employees[i].Fine_Due), employee_col_width[6]);
        outputPaddedString(to_string(employees[i].track_record), employee_col_width[7]);
        outputPaddedString(to_string(employees[i].num_cars_rented), employee_col_width[8]);
        cout << '\n'; 
    }
    cout << "--------------------------------------------------------\n";
    cout << "Enter '1' to add an employee.\n";
    cout << "Enter '2' to update an employee.\n";
    cout << "Enter '3' to delete an employee.\n";
    cout << "Enter '4' to exit.\n";
    cout << "--------------------------------------------------------\n";
    cout << "Input : ";
    int input;
    while(!(cin >> input)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    while(input < 1 || input > 4) {
        cout << "Invalid input. Please enter a valid input.\n";
        cout << "Input : ";
        while(!(cin >> input)) {
            cout << "Error: Enter a number: ";
            cin.clear();
            cin.ignore(123, '\n');
        }
    }
    string name, password, dl, contact;
    long long ip;
    Employee e(0, "", "", "", "", 0, 0, 5, 0);
    switch(input) {
        case 1:
            if (employees.size() == 0) id = 1;
            else id = (employees.end() - 1)->ID + 1;
            cout << "Enter Password : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, password);
            cout << "Enter Name : ";
            getline(cin, name);
            cout << "Enter Driving License : ";
            getline(cin, dl);
            cout << "Enter Contact : ";
            getline(cin, contact);
            e.ID = id;
            e.update_password(password);
            e.Name = name;
            e.DL = dl;
            e.Contact = contact;
            employees.push_back(e);
            break;
        case 2:
            cout << "Enter ID : ";
            while (!(cin >> id)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            for (long long i = 0; i < employees.size(); i++) {
                if (employees[i].ID == id) {
                    cout << "--------------------------------------------------------\n";
                    cout << "Enter '1' to update name.\n";
                    cout << "Enter '2' to update password.\n";
                    cout << "Enter '3' to update driving license number.\n";
                    cout << "Enter '4' to update contact.\n";
                    cout << "Enter '5' to exit.\n";
                    cout << "--------------------------------------------------------\n";
                    cout << "Input : ";
                    while(!(cin >> ip)) {
                        cout << "Error: Enter a number: ";
                        cin.clear();
                        cin.ignore(123, '\n');
                    }
                    while(ip < 1 || ip > 5) {
                        cout << "Invalid input. Please enter a valid input.\n";
                        cout << "Input : ";
                        while(!(cin >> ip)) {
                            cout << "Error: Enter a number: ";
                            cin.clear();
                            cin.ignore(123, '\n');
                        }
                    }
                    switch(ip) {
                        case 1:
                            cout << "Enter new name : ";
                            cin.clear(), cin.ignore(1000, '\n'), getline(cin, name);
                            employees[i].Name = name;
                            break;
                        case 2:
                            cout << "Enter new password : ";
                            cin.clear(), cin.ignore(1000, '\n'), getline(cin, password);
                            employees[i].update_password(password);
                            break;
                        case 3:
                            cout << "Enter new driving license : ";
                            cin.clear(), cin.ignore(1000, '\n'), getline(cin, dl);
                            employees[i].DL = dl;
                            break;
                        case 4:
                            cout << "Enter new contact : ";
                            cin.clear(), cin.ignore(1000, '\n'), getline(cin, contact);
                            employees[i].Contact = contact;
                            break;
                        case 5:
                            return;
                            break;
                    }
                    cout << "Profile updated.\n";
                    return;
                }
            }
            cout << "ID not found.\n";
            break;
        case 3:
            cout << "Enter ID : ";
            long long id;
            while (!(cin >> id)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            for (long long i = 0; i < employees.size(); i++) {
                if (employees[i].ID == id) {
                    for (long long j = 0; j < cars.size(); j++) {
                        if (cars[j].Status == "yes" && cars[j].User_ID == id && cars[j].User_Type == "Employee") {
                            cars[j].Status = "no";
                            cars[j].Rented_On = "";
                            cars[j].Due_Date = "";
                            cars[j].User_Type = "";
                            cars[j].User_ID = 0;
                        } 
                    }
                    employees.erase(employees.begin() + i);
                    cout << "employee deleted.\n";
                    return;
                }
            }
            cout << "ID not found.\n";
            break;
        case 4:
            return;
            break;
    }
}

void Manager::manage_cars() {
    cout << "--------------------------------------------------------\n";
    outputPaddedString("Car ID", car_col_width[0]);
    outputPaddedString("License Plate", car_col_width[1]);
    outputPaddedString("Make", car_col_width[2]);
    outputPaddedString("Model", car_col_width[3]);
    outputPaddedString("Status", car_col_width[12]);
    outputPaddedString("Rented On", car_col_width[13]);
    outputPaddedString("Due Date", car_col_width[14]);
    outputPaddedString("User Type", car_col_width[15]);
    outputPaddedString("User ID", car_col_width[16]);
    cout << "\n";
    for (long long i = 0; i < cars.size(); i++) {
        outputPaddedString(to_string(cars[i].ID), car_col_width[0]);
        outputPaddedString(cars[i].License_Plate, car_col_width[1]);
        outputPaddedString(cars[i].Make, car_col_width[2]);
        outputPaddedString(cars[i].Model, car_col_width[3]);
        outputPaddedString(cars[i].Status, car_col_width[12]);
        outputPaddedString(cars[i].Rented_On, car_col_width[13]);
        outputPaddedString(cars[i].Due_Date, car_col_width[14]);
        outputPaddedString(cars[i].User_Type, car_col_width[15]);
        outputPaddedString(to_string(cars[i].User_ID), car_col_width[16]);
        cout << '\n';
    }
    cout << "--------------------------------------------------------\n";
    cout << "Enter '1' to see detailed specifications.\n";
    cout << "Enter '2' to update car specifications.\n";
    cout << "Enter '3' to delete a car.\n";
    cout << "Enter '4' to add a car.\n";
    cout << "Enter '5' to exit.\n";
    cout << "--------------------------------------------------------\n";
    cout << "Input : ";
    int input;
    while(!(cin >> input)) {
        cout << "Error: Enter a number: ";
        cin.clear();
        cin.ignore(123, '\n');
    }
    while(input < 1 || input > 5) {
        cout << "Invalid input. Please enter a valid input.\n";
        cout << "Input : ";
        while(!(cin >> input)) {
            cout << "Error: Enter a number: ";
            cin.clear();
            cin.ignore(123, '\n');
        }
    }
    string car_name, car_make, car_model, car_fuel_type, car_transmission_type;
    int car_ind, car_id, car_year, car_seating_capacity, car_condition, car_kilometers_driven, car_rent_per_day, car_fine_per_day;
    Car cr(0, "", "", "", 0, 0, "", "", 0, 0, 0, 0, "", "", "", "", 0);
    switch (input) {
        case 1:
            cout << "Enter ID of the car you want to see detailed specifications or enter 0 to exit: ";
            while(!(cin >> id)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            if (id == 0) return;
            for (long long i = 0; i < cars.size(); i++) {
                if (cars[i].ID == id) {
                    cars[i].display_car_details();
                    return;
                }
            }
            cout << "Car not found.\n";
            break;
        case 2:
            cout << "Enter ID of the car you want to update : ";
            while (!(cin >> id)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            car_ind = -1;
            for (long long i = 0; i < cars.size(); i++) {
                if (cars[i].ID == id) {
                    car_ind = i;
                    break;
                }
            }
            if (car_ind == -1) {
                cout << "Car not found.\n";
                return;
            }
            cout << "Enter car License Plate Number : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, car_name);
            cout << "Enter car make : ";
            getline(cin, car_make);
            cout << "Enter car model : ";
            getline(cin, car_model);
            cout << "Enter car year : ";
            while(!(cin >> car_year)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter car seating capacity : ";
            while(!(cin >> car_seating_capacity)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter car fuel type : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, car_fuel_type);
            cout << "Enter car transmission type : ";
            getline(cin, car_transmission_type);
            cout << "Enter car condition on a scale of 0-10 : ";
            while(!(cin >> car_condition)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter car kilometers driven : ";
            while(!(cin >> car_kilometers_driven)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter car rent per day : ";
            while(!(cin >> car_rent_per_day)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter car fine per day : ";
            while(!(cin >> car_fine_per_day)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cars[car_ind].License_Plate = car_name;
            cars[car_ind].Make = car_make;
            cars[car_ind].Model = car_model;
            cars[car_ind].Year = car_year;
            cars[car_ind].Seating_Capacity = car_seating_capacity;
            cars[car_ind].Fuel_Type = car_fuel_type;
            cars[car_ind].Transmission_Type = car_transmission_type;
            cars[car_ind].Condition = car_condition;
            cars[car_ind].Kilometers_Driven = car_kilometers_driven;
            cars[car_ind].Rent_Per_Day = car_rent_per_day;
            cars[car_ind].Fine_Per_Day = car_fine_per_day;
            cout << "Car updated successfully.\n";
            break;
        case 3:
            cout << "Enter ID of the car you want to delete : ";
            while (!(cin >> id)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            for (long long i = 0; i < cars.size(); i++) {
                if (cars[i].ID == id) {
                    cars.erase(cars.begin() + i);
                    cout << "Car deleted.\n";
                    return;
                }
            }
            cout << "Car not found.\n";
            break;
        case 4:
            if (cars.size() == 0) car_id = 1;
            else car_id = (cars.end() - 1)->ID + 1;
            cout << "Enter License Plate Number : ";
            cin.clear(), cin.ignore(1000, '\n'), getline(cin, car_name);
            cout << "Enter car make : ";
            getline(cin, car_make);
            cout << "Enter car model : ";
            getline(cin, car_model);
            cout << "Enter car year : ";
            while(!(cin >> car_year)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter car seating capacity : ";
            while(!(cin >> car_seating_capacity)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter car fuel type : ";
            getline(cin, car_fuel_type);
            cout << "Enter car transmission type : ";
            getline(cin, car_transmission_type);
            cout << "Enter car condition on a scale of 0-10 : ";
            while(!(cin >> car_condition)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter car kilometers driven : ";
            while(!(cin >> car_kilometers_driven)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter car rent per day : ";
            while(!(cin >> car_rent_per_day)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cout << "Enter car fine per day : ";
            while(!(cin >> car_fine_per_day)) {
                cout << "Error: Enter a number: ";
                cin.clear();
                cin.ignore(123, '\n');
            }
            cr.ID = car_id;
            cr.License_Plate = car_name;
            cr.Make = car_make;
            cr.Model = car_model;
            cr.Year = car_year;
            cr.Seating_Capacity = car_seating_capacity;
            cr.Fuel_Type = car_fuel_type;
            cr.Transmission_Type = car_transmission_type;
            cr.Condition = car_condition;
            cr.Kilometers_Driven = car_kilometers_driven;
            cr.Rent_Per_Day = car_rent_per_day;
            cr.Fine_Per_Day = car_fine_per_day;
            cr.Status = "no";
            cr.Rented_On = "";
            cr.Due_Date = "";
            cr.User_Type = "";
            cr.User_ID = 0;
            cars.push_back(cr);
            cout << "Car added successfully.\n";
            break;
        case 5:
            return;
            break;
    }
}

bool Manager::authenticate(string Password) {
    if (Password == this->Password) return true;
    return false;
}





/*Methods for Car*/

Car::Car (long long i, const string& lp, const string& make, const string& model, int year, int seating_capacity, const string& fuel_type, const string& transmission_type, int condition, int kilometers_driven, int rent_per_day, int fine_per_day, const string& status, const string& rented_on, const string& due_date, const string& user_type, long long user_id) {
    ID = i;
    License_Plate = lp;
    Make = make;
    Model = model;
    Year = year;
    Seating_Capacity = seating_capacity;
    Fuel_Type = fuel_type;
    Transmission_Type = transmission_type;
    Condition = condition;
    Kilometers_Driven = kilometers_driven;
    Rent_Per_Day = rent_per_day;
    Fine_Per_Day = fine_per_day;
    Status = status;
    Rented_On = rented_on;
    Due_Date = due_date;
    User_Type = user_type;
    User_ID = user_id;
}


void Car::display_car_details() {
    cout << "--------------------------------------------------------\n";
    cout << "Car ID : " << ID << "\n";
    cout << "License Plate Number : " << License_Plate << '\n';
    cout << "Make : " << Make << '\n';
    cout << "Model : " << Model << '\n';
    cout << "Year : " << Year << '\n';
    cout << "Seating Capacity : " << Seating_Capacity << '\n';
    cout << "Fuel Type : " << Fuel_Type << '\n';
    cout << "Transmission Type : " << Transmission_Type << '\n';
    cout << "Condition : " << Condition << "/10\n";
    cout << "Kilometers Driven : " << Kilometers_Driven << '\n';
    cout << "Rent Per Day : " << Rent_Per_Day << '\n';
    cout << "Fine Per Day : " << Fine_Per_Day << '\n';
    cout << "Status : " << Status << '\n';
    cout << "Rented On : " << Rented_On << '\n';
    cout << "Due Date : " << Due_Date << '\n';
    cout << "User Type : " << User_Type << '\n';
    cout << "User ID : " << User_ID << '\n';
    cout << "--------------------------------------------------------\n";
}