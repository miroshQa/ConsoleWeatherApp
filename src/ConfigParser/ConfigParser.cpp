#include "ConfigParser.h"
#include <sstream>

bool IsInRange(int digit, int left, int right) {
  return digit >= left && digit <= right;
}

int HexToInt(const std::string& hex) {
  int result;
  std::stringstream ss;
  ss << std::hex << hex;
  ss >> result;
  return result;
}

ftxui::Color ParseRgbValue(const json& cfg_data, const std::string& property_name) {
  std::string hex = cfg_data.at(property_name);
  int r = HexToInt(hex.substr(1, 2));
  int g = HexToInt(hex.substr(3, 2));
  int b = HexToInt(hex.substr(5, 2));
  return ftxui::Color(r, g, b);
}

Config ConfigParser::ParseConfig(const std::string& config_path) {
  if (!std::filesystem::exists(config_path)) {
    throw std::runtime_error("Config file doesn't find");
  }
  Config config{};
  std::ifstream temp(config_path);
  json config_data;
  temp >> config_data;

  int days_for_display_by_default = config_data.at("days_amount_for_display_by_default");
  if (!IsInRange(days_for_display_by_default, 1, 16)) {
    throw std::runtime_error("Days for display must be in range [1;16]");
  }
  config.days_amount_for_display_by_default = days_for_display_by_default;

  int max_amount_days_for_display = config_data.at("max_amount_days_for_display");
  if (!IsInRange(max_amount_days_for_display, 1, 16)) {
    throw std::runtime_error("Max amount days for display must be in range [1;16]");
  }
  config.max_amount_days_for_display = max_amount_days_for_display;

  config.cities = config_data.at("cities");
  config.x_api_key = config_data.at("x_api_key");

  int morning_beginning_hour = config_data.at("morning_beginning_hour");
  if (!IsInRange(morning_beginning_hour, 0, 20)) {
    throw std::runtime_error("Morning beginning hour must be in range [0;20]");
  }
  config.morning_beginning_hour = morning_beginning_hour;

  int afternoon_beginning_hour = config_data.at("afternoon_beginning_hour");
  if (!IsInRange(afternoon_beginning_hour, morning_beginning_hour + 1, 21)) {
    throw std::runtime_error("Afternoon beginning hour must be in range (morning_beginning_hour;21]");
  }
  config.afternoon_beginning_hour = afternoon_beginning_hour;

  int evening_beginning_hour = config_data.at("evening_beginning_hour");
  if (!IsInRange(evening_beginning_hour, afternoon_beginning_hour + 1, 22)) {
    throw std::runtime_error("Evening beginning hour must be in range (afternoon_beginning_hour;22])");
  }
  config.evening_beginning_hour = evening_beginning_hour;

  int night_beginning_hour = config_data.at("night_beginning_hour");
  if (!IsInRange(night_beginning_hour, afternoon_beginning_hour + 1, 23)) {
    throw std::runtime_error("Night beginning hour must be in range (evening_beginning_hour;23]");
  }
  config.night_beginning_hour = night_beginning_hour;

  std::string switch_to_prev_city_key = config_data.at("switch_to_prev_city_key");
  config.switch_to_prev_city_key = switch_to_prev_city_key[0];
  std::string switch_to_next_city_key = config_data.at("switch_to_next_city_key");
  config.switch_to_next_city_key = switch_to_next_city_key[0];
  std::string increase_amount_days_to_render_key = config_data.at("increase_amount_days_to_render_key");
  config.increase_amount_days_to_render_key = increase_amount_days_to_render_key[0];
  std::string decrease_amount_days_to_render_key = config_data.at("decrease_amount_days_to_render_key");
  config.decrease_amount_days_to_render_key = decrease_amount_days_to_render_key[0];

  config.cities_cyclic_switch_mode = config_data.at("cyclic_cities_switch_mode");
  config.days_cyclic_increase_decrease_mode = config_data.at("days_cyclic_increase_decrease_mode");

  config.bg_color = ParseRgbValue(config_data, "bg_color");
  config.borders_color = ParseRgbValue(config_data, "borders_color");
  config.text_color = ParseRgbValue(config_data, "text_color");
  config.date_color = ParseRgbValue(config_data, "date_color");
  return config;
}
