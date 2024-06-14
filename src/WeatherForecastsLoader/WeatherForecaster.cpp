#include "WeatherForecaster.h"
#include "src/HelpFunctions/datetime_help_functions.h"

WeatherForecaster::WeatherForecaster(const std::string& x_api_key)
    : x_api_key_(x_api_key) {
  InitForecastDirectories();
}

void WeatherForecaster::InitForecastDirectories() {
  std::vector<std::string>
      directories = {forecasts_data_dir_path, cities_coordinates_dir_path, cities_forecasts_dir_path};
  for (auto& dir_path : directories) {
    if (!std::filesystem::exists(dir_path)) {
      std::filesystem::create_directory(dir_path);
    }
  }
}

void WeatherForecaster::LoadCityCoordinates(const std::string& city_name) {
  cpr::Response r = cpr::Get(
      cpr::Url{"https://api.api-ninjas.com/v1/city?name=" + city_name},
      cpr::Header({{"X-Api-Key", x_api_key_}})
  );

  try {
    auto response_data = json::parse(r.text.substr(1, r.text.size() - 2));
    std::string file_path = cities_coordinates_dir_path + city_name + ".json";
    std::ofstream out_file(file_path);
    out_file << std::setw(4) << response_data;
  } catch (const std::exception& ex) {
    throw std::runtime_error("Unable to load city coordinates. Check your x-api-key and city name correctness");
  }
}

std::pair<double, double> WeatherForecaster::GetCityLongitudeAndLatitude(const std::string& city_name) {
  if (!std::filesystem::exists(cities_coordinates_dir_path + city_name + ".json")) {
    LoadCityCoordinates(city_name);
  }

  std::ifstream file_with_coordinates_for_city(cities_coordinates_dir_path + city_name + ".json");
  json data;
  file_with_coordinates_for_city >> data;
  return {data.at("longitude"), data.at("latitude")};
}

void WeatherForecaster::LoadCityForecast(const std::string& city_name) {
  auto longitude_and_latitude = GetCityLongitudeAndLatitude(city_name);

  try {
    cpr::Response r = cpr::Get(cpr::Url{"https://api.open-meteo.com/v1/forecast"},
                               cpr::Parameters{
                                   {"latitude", std::to_string(longitude_and_latitude.second)},
                                   {"longitude", std::to_string(longitude_and_latitude.first)},
                                   {"forecast_days", "16"},
                                   {"hourly",
                                    "temperature_2m,relative_humidity_2m,apparent_temperature,wind_speed_10m,weather_code"}}
    );

    auto weather_forecast_data = json::parse(r.text);
    std::ofstream out_file(cities_forecasts_dir_path + city_name + ".json");
    json out_data;
    out_data["load_datetime"] = GetCurrentDatetimeAsString(load_datetime_format_);
    out_data["data"] = weather_forecast_data;
    out_file << std::setw(4) << out_data;
  } catch (const std::exception& ex) {
    throw std::runtime_error(
        "Unable to load forecast data. Probably you exceed open meteo requests limit. Try again later (Next day AHAHA)");
  }
}

City WeatherForecaster::GetForecastsStartingToday(const std::string& city_name,
                                                  int forecast_required_days_amount,
                                                  int max_amount_hours_from_load_date_of_forecast_data) {

  const std::string file_path_with_forecast_for_city = cities_coordinates_dir_path + city_name + ".json";
  if (!std::filesystem::exists(file_path_with_forecast_for_city)) {
    LoadCityForecast(city_name);
  }

  // Now we must check if data load datetime satisfy requirements and load again if doesn't
  std::ifstream file_with_city_forecast(cities_forecasts_dir_path + city_name + ".json");
  json data;
  file_with_city_forecast >> data;
  std::tm load_datetime = ParseDatetime(data["load_datetime"], load_datetime_format_);
  int amount_hours_passed_until_now = CalcAmountHoursPassedUntilNow(load_datetime);
  int amount_days_passed_until_now = amount_hours_passed_until_now / 24;

  if (amount_hours_passed_until_now > max_amount_hours_from_load_date_of_forecast_data) {
    LoadCityForecast(city_name);
  } else if (16 - amount_days_passed_until_now < forecast_required_days_amount) {
    LoadCityForecast(city_name);
  }

  // After open / update / load our weather forecast data for 16 days we can cut it in separate days
  data = data.at("data").at("hourly");
  auto apparent_temperature = ConvertToStrVector<double>(data.at("apparent_temperature"));
  auto relative_humidity_2m = ConvertToStrVector<int>(data.at("relative_humidity_2m"));
  auto temperature_2m = ConvertToStrVector<double>(data.at("temperature_2m"));
  auto wind_speed_10m = ConvertToStrVector<double>(data.at("wind_speed_10m"));
  std::vector<std::string> weather_codes;

  for (auto code : data.at("weather_code")) { // auto const&
    if (code.is_number_integer()) {
      weather_codes.push_back(std::to_string((int)code));
    } else {
      weather_codes.push_back(0);
    }
  }

  std::vector<WForecastForDay> weather_forecasts;
  for (int i = 0; i != forecast_required_days_amount; ++i) {
    int l = 0 + 24 * (i + amount_days_passed_until_now); // 24 - магическое число
    int r = l + 24;
    std::unordered_map<HourlyKey, std::vector<std::string>> hourly_info = {
        {HourlyKey::kApparentTemperature, {apparent_temperature.begin() + l, apparent_temperature.begin() + r}},
        {HourlyKey::kRelativeHumidity, {relative_humidity_2m.begin() + l, relative_humidity_2m.begin() + r}},
        {HourlyKey::kTemperature, {temperature_2m.begin() + l, temperature_2m.begin() + r}},
        {HourlyKey::kWindSpeed, {wind_speed_10m.begin() + l, wind_speed_10m.begin() + r}},
        {HourlyKey::kWeatherCode, {weather_codes.begin() + l, weather_codes.begin() + r}},
    };

    auto day_date = AddDays(load_datetime, amount_days_passed_until_now + i);
    weather_forecasts.emplace_back(hourly_info, GetStringDataRepr(day_date, "%Y.%m.%d"));
  }

  return City(weather_forecasts, city_name);
}




WForecastForDay::WForecastForDay(const std::unordered_map<HourlyKey, std::vector<std::string>>& hourly_info,
                                 const std::string& date) : hourly_info_(hourly_info), date_(date) {}
