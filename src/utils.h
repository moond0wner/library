#ifndef UTILS_H
#define UTILS_H

#include <ctime>
#include <string>
#include <optional>

time_t getCurrentDate();
time_t parseDate(std::string dateStr);
std::string dateToString(time_t date);
bool isDateOverdue(time_t date, time_t current, int daysLimit);
std::string toLower(const std::string& text);
std::optional<std::string> validateEmailContact(const std::string& email);
std::optional<std::string> validateNumberPhoneContact(const std::string& numberPhone);

#endif

