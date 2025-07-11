#pragma once
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <search.hpp>

#define GOTO(x, y) std::print("\033[{};{}H", (y), (x))
#define CLEAR() std::print("\033[2J\033[H")

void initWidgets();
std::string readInput(const unsigned int len, unsigned int x, unsigned int y, const std::string placeholder, const unsigned int max=200);
void showMsg(const std::string msg);
void showBox(const std::string title, const std::string content, const unsigned int color);
void quit();
