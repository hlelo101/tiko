#include <db.hpp>

void initDB() {
    std::ifstream file(DB_FILE);
    if(!file.good()) {
        std::ofstream out(DB_FILE);
        out << "tiko.hlelo.cc|Tiko; Terminal search engine|34" << std::endl;
        out.close();

        file.open(DB_FILE);
    }
}

std::vector<entry> getRows() {
    std::ifstream db(DB_FILE);
    if(!db.good()) return {{ "Error", "An error occurred while opening the database.", RED }};

    std::vector<entry> entries;
    std::string line;
    while(std::getline(db, line)) {
        auto s1 = line.find('|');
        auto s2 = line.find('|', s1 + 1);
        if (s1 == std::string::npos || s2 == std::string::npos) continue;

        std::string name = line.substr(0, s1);
        std::string description = line.substr(s1 + 1, s2 - s1 - 1);
        std::string colorS = line.substr(s2 + 1);

        uint8_t color = static_cast<uint8_t>(std::stoi(colorS));
        entries.push_back({ name, description, color });
    }

    return entries;
}
