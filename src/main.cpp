#include <filesystem>


#include "src/WeatherPrinter/WeatherPrinter.h"

using json = nlohmann::json;

int main(int argc, char** argv) {
  Config config{};
  try {

    try {
      config = ConfigParser::ParseConfig("config.json");
    } catch (...) {
      config = ConfigParser::ParseConfig("../config.json");
    }

  } catch (const std::exception& ex) {
    std::cout << "\nConfig parser error!\t" << "[" << ex.what() << "]" << std::endl;
    std::cout << "Fix errors in config and try again" << std::endl;
    std::cout << "You can download config example here: https://github.com/is-itmo-c-23/labwork7-miron2363"
              << std::endl;
    std::cout << "Program working directory was: " << std::filesystem::current_path() << std::endl;
    return 0;
  }

  try {
    WeatherPrinter weather_printer(config);
    weather_printer.Run();
  } catch (const std::exception& ex) {
    std::cout << ex.what();
  }
}


