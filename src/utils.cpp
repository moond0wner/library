#include "Utils.h"
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

const string NUMBERS = "0123456789";

time_t getCurrentDate() {
    return time(nullptr);
}

time_t parseDate(string dateStr) {
    // Формат: "2025-03-21"
    struct tm tm = {};
    istringstream ss(dateStr);
    ss >> tm.tm_year >> tm.tm_mon >> tm.tm_mday;
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    tm.tm_hour = 12;
    return mktime(&tm);
}

string dateToString(time_t date) {
    struct tm* tm = localtime(&date);
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm);
    return string(buffer);
}

bool isDateOverdue(time_t date, time_t current, int daysLimit) {
    return (current - date) > (daysLimit * 24 * 60 * 60);
}

string toLower(const string& text) {
    string result;
    for (char ch : text) {
        result += tolower(ch);
    }
    return result;
}

optional<string> validateEmailContact(const string& email) {
    if (email.empty()) return "Email не может быть пустым!";

    size_t atPos = email.find("@");
    if (atPos == string::npos) return "Неверный email";

    vector<string> domains{"bk.ru", "yandex.ru", "gmail.com", "mail.ru", "ya.ru", "yahoo.com", "rambler.ru"};
    string domainPart = email.substr(atPos + 1);

    for (string& domain : domains) {
        if (domainPart == domain) {
            return nullopt;
        }
    }
    return "Неверный домен!";
}

optional<string> validateNumberPhoneContact(const string& numberPhone) {
    if (numberPhone.empty()) return "Номер телефона не может быть пустым!";
    if (numberPhone.substr(0, 2) != "+7") return "Номер должен начинаться с +7";

    string digits = numberPhone.substr(2);
    if (digits.length() != 10) return "После +7 должно быть ровно 10 цифр";

    for (char ch : digits) {
        if (NUMBERS.find(ch) == string::npos)
            return "Номер должен состоять только из цифр!";
    }
    return nullopt;

}