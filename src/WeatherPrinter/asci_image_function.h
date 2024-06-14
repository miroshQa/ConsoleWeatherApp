#pragma once

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/flexbox_config.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/color_info.hpp"
#include "ftxui/screen/terminal.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/util/ref.hpp"
#include "ftxui/screen/color.hpp"
using namespace ftxui;
using namespace std::chrono_literals;

/*
0	Clear sky OK
1, 2, 3	Mainly clear, partly cloudy, and overcast OK
45, 48	Fog and depositing rime fog OK
51, 53, 55	Drizzle: Light, moderate, and dense intensity OK
56, 57	Freezing Drizzle: Light and dense intensity OK
61, 63, 65	Rain: Slight, moderate and heavy intensity OK
66, 67	Freezing Rain: Light and heavy intensity OK
71, 73, 75	Snow fall: Slight, moderate, and heavy intensity OK
77	Snow grains OK
80, 81, 82	Rain showers: Slight, moderate, and violent OK
85, 86	Snow showers slight and heavy
95 *	Thunderstorm: Slight or moderate
96, 99 *	Thunderstorm with slight and heavy hail
*/

// 0, 1
inline ftxui::Element mainlyclear() {
  return ftxui::vbox({
                         ftxui::text("      \\   /     ") | color(Color(255, 191, 0)),
                         ftxui::text("       .-.      ") | color(Color(255, 191, 0)),
                         ftxui::text("    ― (   ) ―   ") | color(Color(255, 191, 0)),
                         ftxui::text("       `-’      ") | color(Color(255, 191, 0)),
                         ftxui::text("      /   \\     ") | color(Color(255, 191, 0)),
                     });
}

// 2
inline ftxui::Element partlycloudy() {
  return vbox({
                  text(L"   \\  /      ") | color(Color(255, 191, 0)),
                  hbox({
                           text(L" _ /\"\"") | color(Color(255, 191, 0)),
                           text(L".-.    ") | color(Color(250, 250, 250)),
                       }),
                  hbox({
                           text(L"   \\_") | color(Color(255, 191, 0)),
                           text(L"(   ).  ") | color(Color(250, 250, 250)),
                       }),
                  hbox({
                           text(L"   /") | color(Color(255, 191, 0)),
                           text(L"(___(__)      ") | color(Color(250, 250, 250)),
                       }),
              });
}

// 3
inline ftxui::Element overcast() {
  return ftxui::vbox({
                         ftxui::text("     .--.    ") | color(Color(250, 250, 250)),
                         ftxui::text("  .-(    ).  ") | color(Color(250, 250, 250)),
                         ftxui::text(" (___.__)__) ") | color(Color(250, 250, 250)),
                     });
}

//
inline ftxui::Element fog() {
  return ftxui::vbox({
                         ftxui::text("             ") | color(Color(250, 250, 250)),
                         ftxui::text(" _ - _ - _ - ") | color(Color(250, 250, 250)),
                         ftxui::text("  _ - _ - _  ") | color(Color(250, 250, 250)),
                         ftxui::text(" _ - _ - _ - ") | color(Color(250, 250, 250)),
                         ftxui::text("             ") | color(Color(250, 250, 250)),
                     });
}

// 51, 61, 80
inline ftxui::Element weakraindrizzle() {
  return ftxui::vbox({
                         ftxui::text("     .--.    ") | color(Color(250, 250, 250)),
                         ftxui::text("  .-(    ).  ") | color(Color(250, 250, 250)),
                         ftxui::text(" (___.__)__) ") | color(Color(250, 250, 250)),
                         ftxui::text("   ‚ ‚ ‚ ‚   ") | color(Color(137, 207, 240)),
                         ftxui::text("    ’ ’ ’ ’  ") | color(Color(137, 207, 240)),
                     });
}

// 53, 63, 81
inline ftxui::Element midraindrizzle() {
  return ftxui::vbox({
                         ftxui::text("     .--.    ") | color(Color(250, 250, 250)),
                         ftxui::text("  .-(    ).  ") | color(Color(250, 250, 250)),
                         ftxui::text(" (___.__)__) ") | color(Color(250, 250, 250)),
                         ftxui::text("   ‚‘‚ ‚‘‚   ") | color(Color(137, 207, 240)),
                         ftxui::text("    ’  ‚’ ’  ") | color(Color(137, 207, 240)),
                     });
}

// 55, 65, 71, 82
inline ftxui::Element strongraindrizzle() {
  return ftxui::vbox({
                         ftxui::text("     .--.    ") | color(Color(250, 250, 250)),
                         ftxui::text("  .-(    ).  ") | color(Color(250, 250, 250)),
                         ftxui::text(" (___.__)__) ") | color(Color(250, 250, 250)),
                         ftxui::text("   ‚‘‚‘‚‘‚‘  ") | color(Color(0, 0, 139)),
                         ftxui::text("   ‚’‚’‚’‚’  ") | color(Color(0, 0, 139)),
                     });
}

// 56, 661 73
inline ftxui::Element weakfreezing() {
  return ftxui::vbox({
                         ftxui::text("     .--.    ") | color(Color(250, 250, 250)),
                         ftxui::text("  .-(    ).  ") | color(Color(250, 250, 250)),
                         ftxui::text(" (___.__)__) ") | color(Color(250, 250, 250)),
                         ftxui::text("  *     *  ") | color(Color(0, 0, 139)),
                         ftxui::text("   *   *  ") | color(Color(0, 0, 139)),
                     });
}

// 67, 75, 85
inline ftxui::Element midfreezing() {
  return ftxui::vbox({
                         ftxui::text("     .--.    ") | color(Color(250, 250, 250)),
                         ftxui::text("  .-(    ).  ") | color(Color(250, 250, 250)),
                         ftxui::text(" (___.__)__) ") | color(Color(250, 250, 250)),
                         ftxui::text("  *  *   *  ") | color(Color(0, 0, 139)),
                         ftxui::text("   * *  *  ") | color(Color(0, 0, 139)),
                     });
}

// 57, 86
inline ftxui::Element strongfreezing() {
  return ftxui::vbox({
                         ftxui::text("     .--.    ") | color(Color(250, 250, 250)),
                         ftxui::text("  .-(    ).  ") | color(Color(250, 250, 250)),
                         ftxui::text(" (___.__)__) ") | color(Color(250, 250, 250)),
                         ftxui::text("  * * * * *  ") | color(Color(0, 0, 139)),
                         ftxui::text("   * * * * *  ") | color(Color(0, 0, 139)),
                     });
}

// 77
inline ftxui::Element snowgrains() {
  return ftxui::vbox({
                         ftxui::text("  *  *   *  ") | color(Color(0, 0, 139)),
                         ftxui::text("   * *  *  ") | color(Color(0, 0, 139)),
                         ftxui::text("  *  *   *  ") | color(Color(0, 0, 139)),
                         ftxui::text("   * *  *  ") | color(Color(0, 0, 139)),
                     });
}

// 95, 96, 99
inline ftxui::Element thunderstorm() {
  return ftxui::vbox({
                         ftxui::text("     .--.    ") | color(Color(250, 250, 250)),
                         ftxui::text("  .-(    ).  ") | color(Color(250, 250, 250)),
                         ftxui::text(" (___.__)__) ") | color(Color(250, 250, 250)),
                         ftxui::text("    ⚡⚡") | color(Color(0, 0, 139)),
                         ftxui::text("  ⚡       ") | color(Color(0, 0, 139)),
                     });
}

inline ftxui::Element StupidElement() {
  return ftxui::vbox({
                         ftxui::text("█░▀░▀░█▄") | color(Color(250, 250, 250)),
                         ftxui::text("█░▀░░░█─█") | color(Color(255, 215, 0)),
                         ftxui::text("█░▀░░░█─█") | color(Color(255, 215, 0)),
                         ftxui::text("█░░░▀░█▄▀") | color(Color(255, 215, 0)),
                         ftxui::text("▀▀▀▀▀▀▀") | color(Color(218, 165, 32)),
                     });
}




inline std::pair<ftxui::Element, std::string> GetWeatherSymbol(int weather_code) {
  if (weather_code == 0 || weather_code == 1) {
    return {mainlyclear(), "MainlyClear"};
  } else if (weather_code == 2) {
    return {partlycloudy(), "PartlyCloudy"};
  } else if (weather_code == 3) {
    return {overcast(), "OverCast"};
  } else if (weather_code == 45 || weather_code == 48) {
    return {fog(), "Fog"};
  } else if (weather_code == 51 || weather_code == 61 || weather_code == 80) {
    return {weakraindrizzle(), "WeakRainDrizzle"};
  } else if (weather_code == 53 || weather_code == 63 || weather_code == 81) {
    return {midraindrizzle(), "MidRainDrizzle"};
  } else if (weather_code == 55 || weather_code == 65 || weather_code == 71 || weather_code == 82) {
    return {strongraindrizzle(), "StrongRainDrizzle"};
  } else if (weather_code == 56 || weather_code == 66 || weather_code == 73) {
    return {weakfreezing(), "WeakFreezing"};
  } else if (weather_code == 67 || weather_code == 75 || weather_code == 85) {
    return {midfreezing(), "MidFreezing"};
  } else if (weather_code == 57 || weather_code == 86) {
    return {strongfreezing(), "StrongFreezing"};
  } else if (weather_code == 77) {
    return {snowgrains(), "SnowGrains"};
  } else if (weather_code == 95 || weather_code == 96 || weather_code == 99) {
    return {thunderstorm(), "ThunderStorm"};
  } else {
    return {StupidElement(), "WeatherCode " + std::to_string(weather_code)};
  }
}


