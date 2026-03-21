#include "Utils.h"
#include <sstream>

using namespace std;

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
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", tm);
    return string(buffer);
}

bool isDateOverdue(time_t date, time_t current, int daysLimit) {
    return (current - date) > (daysLimit * 24 * 60 * 60);
}