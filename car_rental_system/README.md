# Car Rental System

This is a car rental system implemented in C++ using object-oriented programming principles. The system allows users to rent cars through a terminal interface.

## How to Use

To use the car rental system, follow these steps:

1. **Compile the Code:** Use a C++ compiler to compile the provided source code files. Use ```g++ -o main main.cpp``` if have a ```g++``` compiler. A compiled file is already included. **Note:** The *main* function includes a function ```system(clear)``` which works on Unix-based Operating Systems.
   
2. **Run the Program:** Execute the compiled program in your terminal using ```./main```.

3. **Follow On-Screen Instructions:** The program will provide instructions and prompts for interacting with the system. Users can navigate through the menu options using the terminal.

4. **Testing:** To test the system please refer the database to login as the desired user. The manager is constructed in *main* function. So, refer the credentials to login as manager.

**Note:** The system takes the start date of the rental as the system date. The user has to specify the expected return date while renting and the actual return date while returning.

## Features

### 1. Customers

- Customers can login by providing necessary details such as id and password. **Note:** Only manager can register (add) customers. So, it is assumed that registration process is done through the manager.
- Customers can browse available cars, check details, and rent cars based on availability.

### 2. Employees

- Employees can log in to the system using their credentials.
- Employees can request cars, check availability, clear dues, and return rented cars just as customers.
- Employees receive a discount of $15$% on every booking. 

### 3. Manager

- Manager has access to all mangerial functionalities.
- Managers can add/update/delete customers, employees, and cars.

### 4. Car Management

- The system maintains a database of cars available for rent.
- Each car has associated attributes such as make, model, year, rental price, and availability status.
- Cars can be added, updated, or removed from the system by authorized employees or managers.

## Classes

The system consists of the following classes:

1. **User:** Contains attributes associated with all users of the system. This is a parent class of Customer, Employee, and Manager. 

2. **Customer:** Represents a customer of the car rental system. Contains methods for login, browsing cars, renting cars etc. This is a child class of User.

3. **Employee:** Represents an employee of the car rental company. Contains methods for login, browsing cars, renting cars etc. This is a child class of User.

4. **Manager:** Represents a manager of the car rental company. Inherits methods from the Employee class and includes additional functionalities for administrative tasks. This is a child class of User.

5. **Car:** Represents cars in the rental agency. Contains attributes such as make, model, year, rental price, and availability status etc.

## Dependencies

The system does not rely on any external libraries or dependencies beyond the standard C++ libraries.
