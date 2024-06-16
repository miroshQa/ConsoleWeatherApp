#include <filesystem>
#include <cstdlib>
#include <iterator>


#include "src/WeatherPrinter/WeatherPrinter.h"

using json = nlohmann::json;

int main(int argc, char** argv) {
  Config config{};
  const char* exe_path = std::getenv("CONSOLE_WEATHER_APP_EXE_PATH");

  if (exe_path != nullptr) {
    std::filesystem::current_path(std::string(exe_path));
  }


  try {
    config = ConfigParser::ParseConfig("config.json");
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


