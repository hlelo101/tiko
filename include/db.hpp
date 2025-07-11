#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>

#define DB_FILE "./db.txt"
#define BLACK 30
#define RED 31
#define GREEN 32
#define YELLOW 33
#define BLUE 34
#define MAGENTA 35
#define CYAN 36
#define WHITE 37

typedef struct {
    std::string name;
    std::string desc;
    uint8_t color;
} entry;

void initDB();
std::vector<entry> getRows();


