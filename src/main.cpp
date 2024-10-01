#include <cstdlib>
#include <iostream>

#include "src/WeatherPrinter/WeatherPrinter.h"


int main(int argc, char** argv) {
  Config config{};


  char* home_dir_path = std::getenv("HOME");
  if (home_dir_path == nullptr) {
    std::cout << "Env variable 'home' is missing. Error";
    return 1;
  }

  std::string config_path = std::string(home_dir_path) + "/mwConfig.json";
  try {
    config = ConfigParser::ParseConfig(config_path);
  } catch (const std::exception& ex) {
    std::cout << "\nConfig parser error!\t" << "[" << ex.what() << "]" << std::endl;
    std::cout << "Fix errors in config and try again" << std::endl;
    std::cout << "You can download config example here: https://github.com/miron2363/ConsoleWeatherApp" << std::endl;
    return 0;
  }

  try {
    WeatherPrinter weather_printer(config);
    weather_printer.Run();
  } catch (const std::exception& ex) {
    std::cout << ex.what();
  }


}


