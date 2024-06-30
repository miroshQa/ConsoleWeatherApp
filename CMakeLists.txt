cmake_minimum_required(VERSION 3.25)

project(labwork7_miron2363)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
set(BUILD_STATIC_CURL ON)
set(BUILD_STATIC_FTXUI ON)
set(BUILD_STATIC_LIBS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(.)

add_executable(mWeather
        src/main.cpp
        src/WeatherForecastsLoader/WeatherForecaster.h
        src/WeatherForecastsLoader/WeatherForecaster.cpp
        src/datetime_help_functions.h
        src/WeatherPrinter/WeatherPrinter.cpp
        src/WeatherPrinter/WeatherPrinter.h
        src/ConfigParser/ConfigParser.h
        src/ConfigParser/ConfigParser.cpp
        src/WeatherPrinter/asci_image_function.h)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)
target_link_libraries(mWeather PRIVATE nlohmann_json::nlohmann_json)
include_directories(${json_INCLUDE_DIRECTORY})
message("JSON INCLUDE DIRECTORY IS  ${json_INCLUDE_DIRECTORY}")

FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git)
FetchContent_MakeAvailable(cpr)
target_link_libraries(mWeather PRIVATE cpr::cpr)
include_directories(${cpr_INCLUDE_DIRECTORY})

FetchContent_Declare(ftxui
        GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
        GIT_TAG main # Important: Specify a version or a commit hash here.
)
FetchContent_MakeAvailable(ftxui)

target_link_libraries(mWeather
        PRIVATE ftxui::screen
        PRIVATE ftxui::dom
        PRIVATE ftxui::component # Not needed for this example.
)
