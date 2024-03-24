#include "header.hpp"

//reference : https://iq.opengenus.org/read-and-write-in-csv-in-cpp/#google_vignette
void readfile(const string& fname, vector < vector <string> >& table) {
    vector <string> row;
    string line, word;

    fstream file(fname, ios::in);
    if (file.is_open()) {
        while (getline(file, line)) {
            row.clear();
            stringstream str(line);
            while (getline(str, word, ',')) 
                row.push_back(word);
            table.push_back(row);
        }
    }
    else {
        cerr << "Could not open the file\n";
    }
    file.close();
}

void writefile(vector< vector <string> > table, string fname){
    fstream file(fname, ios::out);
    int row_num = table.size(), col_num = table[0].size();
    for (int i = 0; i < row_num; i++) {
        for (int j = 0; j < col_num; j++) {
            file << table[i][j] << ',';
        }
        file << '\n';
    }
    file.close();
}

void make_objects() {
    for (int i = 1; i < customers_row_num; i++) {
        Customer c(stoll(customers_table[i][0]), customers_table[i][1], customers_table[i][2], customers_table[i][3], customers_table[i][4], stoll(customers_table[i][5]), stoll(customers_table[i][6]), stoll(customers_table[i][7]), stoll(customers_table[i][8]));
        customers.push_back(c);
    }
    for (int i = 1; i < employees_row_num; i++) {
        Employee e(stoll(employees_table[i][0]), employees_table[i][1], employees_table[i][2], employees_table[i][3], employees_table[i][4], stoll(employees_table[i][5]), stoll(employees_table[i][6]), stoll(employees_table[i][7]), stoll(employees_table[i][8]));
        employees.push_back(e);
    }
    for (int i = 1; i < cars_row_num; i++) {
        Car cr(stoll(cars_table[i][0]), cars_table[i][1], cars_table[i][2], cars_table[i][3], stoi(cars_table[i][4]), stoi(cars_table[i][5]), cars_table[i][6], cars_table[i][7], stoi(cars_table[i][8]), stoi(cars_table[i][9]), stoi(cars_table[i][10]), stoi(cars_table[i][11]), cars_table[i][12], cars_table[i][13], cars_table[i][14], cars_table[i][15], stoll(cars_table[i][16]));
        cars.push_back(cr);
    }
}

void update_tables() {
    customers_table.erase(customers_table.begin() + 1, customers_table.end());
    employees_table.erase(employees_table.begin() + 1, employees_table.end());
    cars_table.erase(cars_table.begin() + 1, cars_table.end());
    for (int i = 0; i < customers.size(); i++) {
        vector <string> temp;
        temp.push_back(to_string(customers[i].ID));
        temp.push_back(customers[i].get_pass());
        temp.push_back(customers[i].Name);
        temp.push_back(customers[i].DL);
        temp.push_back(customers[i].Contact);
        temp.push_back(to_string(customers[i].Payment_Due));
        temp.push_back(to_string(customers[i].Fine_Due));
        temp.push_back(to_string(customers[i].track_record));
        temp.push_back(to_string(customers[i].num_cars_rented));
        customers_table.push_back(temp);
    }
    for (int i = 0; i < employees.size(); i++) {
        vector <string> temp;
        temp.push_back(to_string(employees[i].ID));
        temp.push_back(employees[i].get_pass());
        temp.push_back(employees[i].Name);
        temp.push_back(employees[i].DL);
        temp.push_back(employees[i].Contact);
        temp.push_back(to_string(employees[i].Payment_Due));
        temp.push_back(to_string(employees[i].Fine_Due));
        temp.push_back(to_string(employees[i].track_record));
        temp.push_back(to_string(employees[i].num_cars_rented));
        employees_table.push_back(temp);
    }

    for (int i = 0; i < cars.size(); i++) {
        vector <string> temp;
        temp.push_back(to_string(cars[i].ID));
        temp.push_back(cars[i].License_Plate);
        temp.push_back(cars[i].Make);
        temp.push_back(cars[i].Model);
        temp.push_back(to_string(cars[i].Year));
        temp.push_back(to_string(cars[i].Seating_Capacity));
        temp.push_back(cars[i].Fuel_Type);
        temp.push_back(cars[i].Transmission_Type);
        temp.push_back(to_string(cars[i].Condition));
        temp.push_back(to_string(cars[i].Kilometers_Driven));
        temp.push_back(to_string(cars[i].Rent_Per_Day));
        temp.push_back(to_string(cars[i].Fine_Per_Day));
        temp.push_back(cars[i].Status);
        temp.push_back(cars[i].Rented_On);
        temp.push_back(cars[i].Due_Date);
        temp.push_back(cars[i].User_Type);
        temp.push_back(to_string(cars[i].User_ID));
        cars_table.push_back(temp);
    }
}