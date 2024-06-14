#pragma once
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

inline std::tm ParseDatetime(const std::string& datetime_string, const std::string& date_time_format) {
    std::tm datetime = {};
    std::istringstream ss(datetime_string);
    ss >> std::get_time(&datetime, date_time_format.c_str());
    return datetime;
}

inline std::string GetStringDataRepr(std::tm date_time, const std::string& format) {
    char buffer[80];
    std::strftime(buffer, 80, format.c_str(), &date_time);
    return std::string(buffer);
}

inline std::tm AddDays(const std::tm& date_time, int days_to_add) {
    std::tm new_date = date_time;
    time_t time = std::mktime(&new_date);
    time += days_to_add * 24 * 60 * 60;
    new_date = *std::localtime(&time);
    return new_date;
}

inline int CalcAmountHoursPassedUntilNow(std::tm prev_time) {
    std::time_t now = std::time(0);   // get time now
    int difference = std::difftime(now, std::mktime(&prev_time)) / (60 * 60);
    return difference;
}

inline std::string GetCurrentDatetimeAsString(const std::string& format) {
    const std::time_t currentTime = std::time(nullptr);
    std::tm* curr_datetime = std::localtime(&currentTime);
    return GetStringDataRepr(*curr_datetime, format);
}
