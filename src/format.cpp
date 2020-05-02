#include <string>
#include <iostream>
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  char buffer[50];
  int h, m, s;
  const int hourInSeconds {3600};
  const int minuteInSeconds {60};
  h = seconds / hourInSeconds;
  seconds %= hourInSeconds;
  m = seconds / minuteInSeconds;
  s = seconds % minuteInSeconds;
  sprintf(buffer, "%02d:%02d:%02d", h, m, s);
  return string(buffer);
}