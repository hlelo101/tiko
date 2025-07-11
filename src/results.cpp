#include <results.hpp>
#include <db.hpp>
#include <vector>

using Clock = std::chrono::steady_clock;

void drawResults(std::vector<entry> entries, unsigned int selected) {
    GOTO(0, 4);
    std::println("\033[01;30;41mResults:\033[00m");

    for(unsigned int i = 0; i < entries.size(); ++i) {
        if(selected != i) {
            std::println(
                "\033[01;{}m{}\n\033[00;{}m> {}\033[00m",
                entries[i].color,
                entries[i].name,
                entries[i].color,
                entries[i].desc
            );
        } else {
            std::println(
                "\033[01;30;{}m{}\n\033[00;{}m> {}\033[00m",
                entries[i].color + 10,
                entries[i].name,
                entries[i].color,
                entries[i].desc
            );
        }
    }
}

void drawArrows(unsigned int on, unsigned int total) {
    const std::string str = std::format("← Page {} of {} →", on, total);
    GOTO(0, tSizeH - 1);
    std::print("{}", std::string(tSizeW, ' '));
    GOTO(tSizeW / 2 - str.length() / 2, tSizeH - 1);
    std::print("{}", str);
}

template <typename T>
void vecCopy(const std::vector<T>& src, std::vector<T>& dest, unsigned int from, unsigned int to) {
    dest.clear();
    for(unsigned int i = from; i < to; ++i) {
        if(src.size() <= i) break;
        dest.push_back(src[i]);
    }
}


std::string toLower(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return out;
}

// This is... Not optimal at all but I don't really care
std::vector<entry> sort(std::vector<entry> entries, const std::string query) {
    std::vector<entry> result;
    std::vector<bool> added(entries.size(), false);
    std::string q = toLower(query);

    // Name starts with query 
    for(size_t i = 0; i < entries.size(); i++) {
        if(toLower(entries[i].name).starts_with(q)) {
            result.push_back(entries[i]);
            added[i] = true;
        }
    }
    // Name contains query
    for(size_t i = 0; i < entries.size(); i++) {
        if(added[i]) continue;
        if(toLower(entries[i].name).contains(q)) {
            result.push_back(entries[i]);
            added[i] = true;
        }
    }
    // Description contains query
    for(size_t i = 0; i < entries.size(); i++) {
        if(added[i]) continue;
        if(toLower(entries[i].desc).contains(q)) {
            result.push_back(entries[i]);
            added[i] = true;
        }
    }

    return result;
}

void search(std::string q) {
    CLEAR();
    DISABLE_CURSOR();

    std::println("──────┬{}", repeat("─", tSizeW - 7));
    std::string tikoStr;
    for(int i = 0; i < 4; i++) {
        tikoStr += std::format("\033[01;3{}m{}", i + 1, std::string("Tiko")[i]);
    }
    std::println(
        " {}\033[00m │ \033[01;37mQuery\033[00m: {}",
        tikoStr,
        q.size() < tSizeW - 15 ? q : q.substr(0, tSizeW - 19).append("...")
    );
    std::println("──────┴{}", repeat("─", tSizeW - 7));
    showMsg("Searching...");
    auto start = Clock::now();

    std::vector<entry> entries = sort(getRows(), q);
    std::vector<entry> visible;
    const unsigned int numVisible = (tSizeH - 6) / 2;
    const unsigned int totalPages = entries.size() / numVisible;
    unsigned int at = 0;

    vecCopy(entries, visible, numVisible * at, numVisible * at + numVisible);

    unsigned int selected = 0;
    drawResults(visible, selected);
    drawArrows(at + 1, totalPages + 1);
 
    auto end = Clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    showMsg(std::format("Done. Took {}ms. | q: Quit | n: New search", duration));
    char c;
    while(1) {
        if(read(0, &c, 1) == -1) break;
        if(c == '\033') {
            char seq[2];
            if(read(0, &seq[0], 1) == -1) break;
            if(read(0, &seq[1], 1) == -1) break;

            if(seq[0] == '[') {
                switch(seq[1]) {
                    case 'A':
                        selected--;
                        if(selected >= visible.size()) selected = visible.size() - 1;
                        drawResults(visible, selected);
                        break;
                    case 'B':
                        selected++;
                        if(selected >= visible.size()) selected = 0;
                        drawResults(visible, selected);
                        break;
                    case 'C':
                        at++;
                        if(at >= totalPages) at = totalPages;
                        drawArrows(at + 1, totalPages + 1);
                        vecCopy(entries, visible, numVisible * at, numVisible * at + numVisible);
                        if(selected > visible.size()) selected = visible.size() - 1;
                        GOTO(0, 4);
                        std::print("{}", std::string(tSizeW * (tSizeH - 5), ' '));
                        drawResults(visible, selected);
                        break;
                    case 'D':
                        at--;
                        if(at >= totalPages) at = 0;
                        drawArrows(at + 1, totalPages + 1);
                        vecCopy(entries, visible, numVisible * at, numVisible * at + numVisible);
                        if(selected > visible.size()) selected = visible.size() - 1;
                        GOTO(0, 4);
                        std::print("{}", std::string(tSizeW * (tSizeH - 5), ' '));
                        drawResults(visible, selected);
                        break;
                }
            }
        } else {
            switch(c) {
                case '\r':
                case '\n':
                    showBox(visible[selected].name, visible[selected].desc, visible[selected].color);
                    GOTO(0, 4);
                    std::print("{}", std::string(tSizeW * (tSizeH - 5), ' '));
                    drawResults(visible, selected);
                    break;
                case 'n':
                    ENABLE_CURSOR();
                    return;
                case 'q':
                    CLEAR();
                    std::println("Bye!");
                    exit(0);
            }
        }
    }

    ENABLE_CURSOR();
}
