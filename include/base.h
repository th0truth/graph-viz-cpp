#pragma once

#include <iostream>
#include <algorithm>
#include <cctype>
#include <chrono>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iomanip>
#include <sys/stat.h>
#include <sys/types.h>
#include <queue>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef int32_t i32;

#define TRACE(visualize, x) do { if (visualize) { cout << x; } } while(0)
