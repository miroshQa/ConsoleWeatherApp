#include <cstdlib>

#include "WeatherPrinter.h"
#include "asci_image_function.h"

WeatherPrinter::WeatherPrinter(const Config& config) :
    config_(config) {
  std::ofstream log_file("last_run_log_file.txt");
  WeatherForecaster weather_forecaster(config.x_api_key);
  for (const std::string& city_name : config.cities) {
    try {
      auto city_weather_date = weather_forecaster.GetForecastsStartingToday(city_name,
                                                                            config.max_amount_days_for_display,
                                                                            config.update_frequency_in_hours);

      forecasts_data_for_cities_.push_back(city_weather_date);
    } catch (const std::exception& ex) {
      log_file << "Unable to load weather forecast for:\t" << "[" << city_name << "]" << std::endl;
      log_file << "Error:\t[" << ex.what() << "]" << std::endl;
    }
  }

  if (forecasts_data_for_cities_.empty()) {
    throw std::runtime_error("Can't get data for every city.\nCheck log file");
  }

}

void WeatherPrinter::Run() {
  days_for_render_amount_ = config_.days_amount_for_display_by_default;

  int selected_city_index = 0;
  std::vector<Component> components;
  for (auto& city : forecasts_data_for_cities_) {
    components.push_back(CreateWeatherForecastPage(city));
  }

  auto main_renderer = Renderer([&] {
    return components[selected_city_index]->Render();
  });


  auto screen = ScreenInteractive::TerminalOutput();

  main_renderer |= CatchEvent([&](Event event) {
    if (event == Event::Character(config_.decrease_amount_days_to_render_key)) {
      days_for_render_amount_ = NormalizeNumber(days_for_render_amount_ - 1,
                                                1,
                                                config_.max_amount_days_for_display,
                                                config_.days_cyclic_increase_decrease_mode);
      return true;
    } else if (event == Event::Character(config_.increase_amount_days_to_render_key)) {
      days_for_render_amount_ = NormalizeNumber(days_for_render_amount_ + 1,
                                                1,
                                                config_.max_amount_days_for_display,
                                                config_.days_cyclic_increase_decrease_mode);
      return true;
    } else if (event == Event::Character(config_.switch_to_next_city_key)) {
      selected_city_index =
          NormalizeNumber(selected_city_index + 1, 0, components.size() - 1, config_.cities_cyclic_switch_mode);
      return true;
    } else if (event == Event::Character(config_.switch_to_prev_city_key)) {
      selected_city_index =
          NormalizeNumber(selected_city_index - 1, 0, components.size() - 1, config_.cities_cyclic_switch_mode);
      return true;
    } else if (event == Event::Escape) {
      screen.Exit();
    }
    return false;
  });

  screen.TrackMouse(false);
  screen.Loop(main_renderer);
}

Element WeatherPrinter::CreateDayOfTimeForecastVBox(WForecastForDay& w_forecast,
                                                    int hour,
                                                    const std::string& time_of_day_string) {
  auto symbol = GetWeatherSymbol(std::stoi(w_forecast.GetHourlyInfo(hour, HourlyKey::kWeatherCode)));
  auto asci_image = symbol.first;
  auto image_description = symbol.second;

  auto t_c = color(config_.text_color);

  auto time_of_day_block = hbox(filler(), text(time_of_day_string) | t_c, filler());
  Element weather_data_block = vbox(
      text(image_description) | t_c,
      text(w_forecast.GetHourlyInfo(hour, HourlyKey::kTemperature) + " °C ") | t_c,
      text(w_forecast.GetHourlyInfo(hour, HourlyKey::kWindSpeed) + " km/h") | t_c,
      text(w_forecast.GetHourlyInfo(hour, HourlyKey::kApparentTemperature) + " °C") | t_c,
      text(w_forecast.GetHourlyInfo(hour, HourlyKey::kRelativeHumidity) + "%") | t_c
  );
  auto result = hbox(weather_data_block, filler(), asci_image, filler());

  return vbox(time_of_day_block | flex, separator() | color(config_.borders_color), result | flex)
      | borderStyled(config_.borders_color) | size(WIDTH, Constraint::EQUAL, 40);
}

Component WeatherPrinter::CreateDayWeatherForecastHBox(WForecastForDay& w_forecast) {
  auto border = borderStyled(config_.borders_color);
  Element result = hbox(
      CreateDayOfTimeForecastVBox(w_forecast, config_.morning_beginning_hour, "Morning") | flex,
      CreateDayOfTimeForecastVBox(w_forecast, config_.afternoon_beginning_hour, "Afternoon") | flex,
      CreateDayOfTimeForecastVBox(w_forecast, config_.evening_beginning_hour, "Evening") | flex,
      CreateDayOfTimeForecastVBox(w_forecast, config_.night_beginning_hour, "Night") | flex
  );

  auto data_box = hbox(filler(),
                       text(w_forecast.GetDate()) | color(config_.date_color) | borderStyled(config_.borders_color),
                       filler());
  result = vbox(data_box, filler(),  result );
  auto result_component = Renderer([=] { return result; });
  return result_component;
}

Component WeatherPrinter::CreateWeatherForecastPage(City& city) {
  std::vector<Component> components;
  std::string city_name = city.GetName();
  Component city_label =
      Renderer([=] {
        return hbox(filler(), text(city_name) | color(Color(15, 244, 255)), filler())
            | borderStyled(BorderStyle::HEAVY, config_.borders_color);
      });

  components.push_back(city_label);
  int day_number = 1;
  for (auto& w_forecast : city.GetWeatherForecasts()) {
    auto day_predicate = [this, day_number] { return day_number <= this->days_for_render_amount_; };
    Component day = CreateDayWeatherForecastHBox(w_forecast) | Maybe(day_predicate);
    components.push_back(day);
    ++day_number;
  }

  return Container::Vertical(components) | bgcolor(Color(config_.bg_color));
}
