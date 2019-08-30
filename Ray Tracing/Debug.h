#pragma once
bool debugging_enabled = 1;
#define DEBUG(x) do { \
  if (debugging_enabled) { std::cerr <<"->"<< x << std::endl; } \
} while (0)