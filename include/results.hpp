#pragma once
#include <iostream>
#include <print>
#include <widgets.hpp>
#include <search.hpp>
#include <chrono>
#include <format>
#include <algorithm>
#include <cctype>

#define DISABLE_CURSOR() std::print("\033[?25l")
#define ENABLE_CURSOR() std::print("\033[?25h")

void search(std::string q);
