#pragma once

#include <string>
#include <vector>

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include "src/ConfigParser/ConfigParser.h"
#include "src/WeatherForecastsLoader/WeatherForecaster.h"

using namespace ftxui;

class WeatherPrinter {
 public:
  explicit WeatherPrinter(const Config& config);

  void Run();

 private:
  const Config& config_;
  std::vector<City> forecasts_data_for_cities_;
  int days_for_render_amount_;

  Component CreateWeatherForecastPage(City& city);
  Component CreateDayWeatherForecastHBox(WForecastForDay& w_forecast);
  Element CreateDayOfTimeForecastVBox(WForecastForDay& w_forecast, int hour, const std::string& time_of_day_string);
};
