
#include <string>

#ifndef SLEEPHANDLER_HPP
#define SLEEPHANDLER_HPP

namespace ch {

constexpr auto uS_TO_S_FACTOR = 1000000;

RTC_DATA_ATTR int bootCount = 0;

std::string wakeupReason() {
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0 : return "Wakeup caused by external signal using RTC_IO";
    case ESP_SLEEP_WAKEUP_EXT1 : return "Wakeup caused by external signal using RTC_CNTL";
    case ESP_SLEEP_WAKEUP_TIMER : return "Wakeup caused by timer";
    case ESP_SLEEP_WAKEUP_TOUCHPAD : return "Wakeup caused by touchpad";
    case ESP_SLEEP_WAKEUP_ULP : return "Wakeup caused by ULP program";
    default : return "Wakeup was not caused by deep sleep"; // + wakeup_reason;
  }
}

void deepSleep(unsigned int seconds) {
    esp_sleep_enable_timer_wakeup(seconds * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}

} // namespace ch

#endif
