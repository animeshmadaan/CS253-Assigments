#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector <vector <string> > customers_table, employees_table, cars_table;
long long customers_row_num, employees_row_num, cars_row_num, customers_col_num, employees_col_num, cars_col_num;

long long id;
string password;
long long l, r, mid;

int car_col_width[17] = {6, 15, 17, 18, 4, 17, 9, 10, 12, 17, 13, 13, 15, 10, 10, 10, 7};
int customer_col_width[9] = {11, 17, 17, 17, 18, 11, 8, 12, 11};
int employee_col_width[9] = {11, 17, 17, 17, 18, 11, 8, 12, 11};

void readfile(const string& fname, vector < vector <string> >& table);
void writefile(vector< vector <string> > table, string fname);
chrono::system_clock::time_point parse(const string& datetimeString);

class User {
protected:
string Password;
public:
    long long ID;
    string Name;
    string DL;
    string Contact;
    long long Payment_Due;
    long long Fine_Due;
    long long track_record;
    long long num_cars_rented;
    void update_password(string new_password);
    bool verify(string password);
};

class Customer : public User {
    public:
    Customer (long long i, const string& p, const string& n, const string& dl, const string &contact, long long pd, long long fd, long long tr, long long ncr);
    void display_options();
    void display_available_cars();
    void rent_car();
    void return_car();
    void display_curr_rented_cars();
    void display_dues();
    void clear_dues();
    void update_profile();
    string get_pass();
};

class Employee : public User {
    public:
    Employee (long long i, const string& p, const string& n, const string& dl, const string &contact, long long pd, long long fd, long long tr, long long ncr);
    void display_options();
    void display_available_cars();
    void rent_car();
    void return_car();
    void display_curr_rented_cars();
    void display_dues();
    void clear_dues();
    void update_profile();
    string get_pass();
};

class Manager : public User {
    public:
    Manager (long long i, const string& p, const string& n, const string& dl, const string &contact, long long pd, long long fd, long long tr, long long ncr);
    void display_options();
    void manage_customers();
    void manage_employees();
    void manage_cars();
    bool authenticate(string Password);
};

class Car{
    public:
    Car (long long i, const string& lp, const string& make, const string& model, int year, int seating_capacity, const string& fuel_type, const string& transmission_type, int condition, int kilometers_driven, int rent_per_day, int fine_per_day, const string& status, const string& rented_on, const string& due_date, const string& user_type, long long user_id);
    long long ID;
    string License_Plate;
    string Make;
    string Model;
    int Year;
    int Seating_Capacity;
    string Fuel_Type;
    string Transmission_Type;
    int Condition;
    int Kilometers_Driven;
    int Rent_Per_Day;
    int Fine_Per_Day;
    string Status;
    string Rented_On;
    string Due_Date;
    string User_Type;
    long long User_ID;
    void display_car_details();
};


vector <Customer> customers;
vector <Employee> employees;
vector <Car> cars;
