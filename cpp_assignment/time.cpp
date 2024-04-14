#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

string DateTime(const chrono::system_clock::time_point& timePoint);
long long daysBetween(string startDateStr, string endDateStr);
long long parse_time(string date);
string getCurrentDate();

// reference: https://www.geeksforgeeks.org/date-and-time-parsing-in-cpp/
chrono::system_clock::time_point parse(const string& datetimeString) {
    const string format = "%d-%m-%Y";
    tm tmStruct = {};
    istringstream ss(datetimeString);
    ss >> get_time(&tmStruct, format.c_str());
    return chrono::system_clock::from_time_t(
    mktime(&tmStruct));
}

string DateTime(const chrono::system_clock::time_point& timePoint) {
    const string format = "%d-%m-%Y";
    time_t time = chrono::system_clock::to_time_t(timePoint);
    tm* timeinfo = localtime(&time);
    char buffer[70];
    strftime(buffer, sizeof(buffer), format.c_str(),timeinfo);
    return buffer;
}

long long daysBetween(string startDateStr, string endDateStr) {
    time_t startDate = chrono::system_clock::to_time_t(parse(startDateStr));
    time_t endDate = chrono::system_clock::to_time_t(parse(endDateStr));
    if (startDate > endDate) return -1;
    else return (endDate - startDate + 10000)/86400;
}

long long parse_time(string date) {
    return chrono::system_clock::to_time_t(parse(date));
}

string getCurrentDate() {
  time_t now = time(0);
  tm *ltm = localtime(&now);

  // Format date as DD-MM-YYYY
  char buffer[20];
  strftime(buffer, 20, "%d-%m-%Y", ltm);

  return string(buffer);
}