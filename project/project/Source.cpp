#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <sstream> 
#include <iomanip>
#include <exception>

using namespace std;
class Date {
public:
	Date() { Year = -1, Month = -1, Day = -1; }; // по умолчанию
	Date(const int& year, const int& month, const int& day) { // параметризованный
		Year = year;
		Month = month;
		Day = day;
	}
	int GetYear() const {
		return Year;
	}
	int GetMonth() const {
		return Month;
	}
	int GetDay() const {
		return Day;
	}
private: 
	int Year;
	int Month;
	int Day;
};

bool operator == (const Date& date1, const Date& date2) {
	return date1.GetYear() == date2.GetYear() &&
		date1.GetMonth() == date2.GetMonth() &&
		date1.GetDay() == date2.GetDay();
}
bool operator < (const Date date1, const Date date2) {
	if (date1.GetYear() != date2.GetYear()) {
		return date1.GetYear() < date2.GetYear();
	}
	else if (date1.GetMonth() != date2.GetMonth()) {
		return date1.GetMonth() < date2.GetMonth();
	}
	else {
		return date1.GetDay() < date2.GetDay();
	}
}
ostream& operator << (ostream& out, const Date& date) {
	//stringstream stream;
	out << setfill('0');
	out << setw(4) << date.GetYear() << '-' << setw(2) << date.GetMonth() << '-' << setw(2) << date.GetDay();
	return out;
}

class Database { // должна хранить в себе массив?
private:
	map <Date, set <string>> m;
public:
	void AddEvent(const Date& date, const string& event) { // Здесь обработка ошибки Лучше чем в main
		m[date].insert(event);
	}
	bool DeleteEvent(const Date& date, const string& event) {
		bool is_in_map = false;
		if (m.count(date) > 0 && m[date].count(event) > 0) {
			is_in_map = true;
			m[date].erase(event);
		}
		return is_in_map;
	}

	int DeleteDate(const Date& date) {
		int cnt = 0;
		if (m.count(date) > 0) {
			cnt = m[date].size();
			m.erase(date);
		}
		return cnt;
	}

	void Find(const Date& date) const {
		for (const auto key : m) {
			if (key.first == date) {
				for (const auto item : key.second) {
					cout << item << '\n';
				}
			}
		}
	}

	void Print() const {
		int cnt = 0;
		for (auto key : m) {
			for (auto event : key.second) {
				cout << key.first << ' ' << event << '\n';
			}
		}
	}
};

Date ParseData(const string& date) {
	stringstream date_stream(date);
	int year, month, day;
	date_stream >> year;
	if (date_stream.peek() != '-') {
		throw runtime_error("Wrong date format: " + date);
	}
	date_stream.ignore(1);
	date_stream >> month;
	if (date_stream.peek() != '-') {
		throw runtime_error("Wrong date format: " + date);
	}

	date_stream.ignore(1);
	bool c = true && (date_stream >> day);
	if (c == false) throw runtime_error("Wrong date format: " + date);
	if (!date_stream.eof()) {
		throw runtime_error("Wrong date format: " + date);
	}
	///////////////////////////////////////////////////////////
	if (month < 1 || month > 12) {
		throw runtime_error("Month value is invalid: " + to_string(month));
	}
	else if (day < 1 || day > 31) {
		throw runtime_error("Day value is invalid: " + to_string(day));
	}
	return Date({ year, month, day });
}

int main() {
	// После обработки любой из описанных ошибок ввода и печати сообщения программа должна завершать своё выполнение.
	try {
		Database db;
		string command_line;
		while (getline(cin, command_line)) {
			Date date;
			stringstream stream(command_line);
			string command;
			stream >> command;
			if (command == "") continue; // могут поступать пустые строки
			else if (command == "Add") {
				string date_str, event; // try then command.str() and stream >> date_str >> event
				stream >> date_str >> event;
				Date date = ParseData(date_str);
				db.AddEvent(date, event);
			}
			else if (command == "Print") {
				db.Print();
			}
			else if (command == "Find") {
				string date_str;
				stream >> date_str;
				date = ParseData(date_str);
				db.Find(date);
			}
			else if (command == "Del") {
				string date_str;
				stream >> date_str;
				date = ParseData(date_str);
				if (stream.eof()) { // удаляем всю дату
					int n = db.DeleteDate(date);
					cout << "Deleted " << n << " events" << '\n';
				}
				else { // удаляем событие в дате
					string ev;
					stream >> ev;
					if (db.DeleteEvent(date, ev)) {
						cout << "Deleted successfully" << '\n';
					}
					else {
						cout << "Event not found" << '\n';
					}
				}
			}
			else if (command.empty()) break;
			else throw runtime_error("Unknown command: " + command);
		}
	}
	catch (exception& ex) {
		cout << ex.what() << endl;
	}
	return 0;
}


// Add 1-1- event ломает ParseData
/*
Add 1-1-1 event1 correct
Add -1-1-1 event2 correct
Add 1--1-1 event3 month incorrect: -1
Add 1---1-1 event4 incorrect month with --
Add 1-+1-+1 event5 correct
// all works
*/
/*
Add 0-1-2 event1
Print
*/
/* correct input:
* 
Add 0-1-2 event1
Add 1-2-3 event2
Find 0-1-2

Del 0-1-2
Print
Del 1-2-3 event2
Del 1-2-3 event2
*/
/*
Add 10-10-10 event1
Find 10-10-9
Print
*/