#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"
#include "nlohmann/json.hpp"

using json = nlohmann::json;


struct Config {
    int days_amount_for_display_by_default;
    int max_amount_days_for_display;
    int update_frequency_in_hours;
    std::vector<std::string> cities;
    std::string x_api_key;
    int morning_beginning_hour;
    int afternoon_beginning_hour;
    int evening_beginning_hour;
    int night_beginning_hour;
    char switch_to_prev_city_key;
    char switch_to_next_city_key;
    char increase_amount_days_to_render_key;
    char decrease_amount_days_to_render_key;
    bool cities_cyclic_switch_mode;
    bool days_cyclic_increase_decrease_mode;
    ftxui::Color bg_color;
    ftxui::Color  borders_color;
    ftxui::Color text_color;
    ftxui::Color date_color;
};

class ConfigParser {
public:
   static Config ParseConfig(const std::string& config_path);
};