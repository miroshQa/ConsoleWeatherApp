#pragma once
#include <ctime>
#include <vector>
#include <string>
#include <unordered_map>

class WForecastForDay;
class City;

class WeatherForecaster {
public:
    explicit WeatherForecaster(const std::string& x_api_key);

    City GetForecastsStartingToday(const std::string& city_name,
                               int forecast_required_days_amount,
                               int max_amount_hours_from_load_date_of_forecast_data);


private:
    const std::string x_api_key_;

    /*
     Получаем прогноз погоды на 16 дней вперед. Используем эти данные на протяжении 16 дней позволяя
     программе работать в режиме оффлайн. Загружаем данные раньше прошествия 16 дней если этого все же
     требует частота обновления данных указанная в конфиге или количество дней которые хочет видеть
     пользователь (также указанныx в конфиге)  не могут покрыть существующие данные. Координаты города
     загружаются единственный раз. Ниже указаные директории в которыx это все хранится, если директорий
     не существовала (первый запуск программы или они были удалены), то они будут созданы
     */

    static inline const std::string forecasts_data_dir_path = "./forecasts_data/";
    static inline const std::string cities_coordinates_dir_path = forecasts_data_dir_path + "cities_coordinates/";
    static inline const std::string cities_forecasts_dir_path = forecasts_data_dir_path + "cities_forecasts/";
    static inline const std::string load_datetime_format_ = "%Y-%m-%d %H:%M:%S";

    static void InitForecastDirectories();

    void LoadCityForecast(const std::string& city_name);
    void LoadCityCoordinates(const std::string& city_name);
    std::pair<double, double> GetCityLongitudeAndLatitude(const std::string& city_name);
};


enum class HourlyKey {
    kTemperature,
    kApparentTemperature,
    kRelativeHumidity,
    kWindSpeed,
    kWeatherCode,
};

class WForecastForDay {
public:
    WForecastForDay(const std::unordered_map<HourlyKey, std::vector<std::string>>& hourly_info,
                    const std::string &date);

    inline std::string GetHourlyInfo(int hour, HourlyKey key) {return hourly_info_[key][hour];}
    inline std::string GetDate() {return date_;};
private:
    std::unordered_map<HourlyKey, std::vector<std::string>> hourly_info_;
    const std::string date_;
};

class City {
public:
    City(const std::vector<WForecastForDay>& forecasts, const std::string& city_name) : forecasts_(forecasts), city_name_(city_name) {}
    inline std::vector<WForecastForDay> GetWeatherForecasts() {return forecasts_;}
    inline std::string GetName() const {return city_name_;}
    inline std::string GetCountry() const {return country_;}

private:
    std::vector<WForecastForDay> forecasts_;
    const std::string city_name_;
    const std::string country_;
};
