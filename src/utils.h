#ifndef UTILS_H
#define UTILS_H

#include <ctime>
#include <string>

time_t getCurrentDate();
time_t parseDate(std::string dateStr);
std::string dateToString(time_t date);
bool isDateOverdue(time_t date, time_t current, int daysLimit);

#endif

