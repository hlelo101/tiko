#include <widgets.hpp>
#include <print>
#include <termios.h>

// termios stuff
static struct termios old, new1;
void initTermios() {
    tcgetattr(0, &old);
    new1 = old;
    new1.c_lflag &= ~ICANON;
    new1.c_lflag &= ~ECHO;
    tcsetattr(0, TCSANOW, &new1);
}

void resetTermios(void) {
    tcsetattr(0, TCSANOW, &old);
}

void initWidgets() {
    initTermios();
}

std::string readInput(const unsigned int len, unsigned int x, unsigned int y, const std::string placeholder, const unsigned int max) {
    std::string input;
    bool done = false;
    GOTO(x + (int)input.size(), y);

    char c;
    while(!done) {
        if(input.empty()) {
            GOTO(x, y);
            std::print("\033[90m{}{}\033[0m", placeholder, std::string(len - placeholder.size(), ' '));
            GOTO(x, y);
        } else {
            GOTO(x, y);
            std::string visible = input.substr(input.size() - std::min<size_t>(len, input.size()));
            std::print("{}{}", visible, std::string(len - visible.size(), ' '));
            GOTO(x + (visible.size() == len ? len - 1 : visible.size()), y);
        }

        std::cout.flush();
        if(read(0, &c, 1) == -1) break;

        switch(c) {
            case '\n':
            case '\r':
                done = true;
                break;
            case 127:
            case '\b':
                if(!input.empty()) {
                    input.pop_back();
                    std::print("\b");
                }
                break;
            default:
                if(isprint(c) && input.length() <= max)
                    input += c;
        }
    }

    return input;
}

void showMsg(const std::string msg) {
    GOTO(0, tSizeH);
    std::print("\033[00;30;43m{}{}\033[00m", msg, std::string(tSizeW - msg.size(), ' '));
    std::cout.flush();
}


void showBox(const std::string title, const std::string content, const unsigned int color) {
    const unsigned int useTextW = std::max({ title.length(), content.length(), 28UL });
    const unsigned int boxWidth = useTextW + 4;

    GOTO(tSizeW / 2 - boxWidth / 2, tSizeH / 2 - 2);
    std::print("\033[00;30;{}m┌─{}{}┐\033[00m", color + 10, title, repeat("─", useTextW - title.length() + 1));
    GOTO(tSizeW / 2 - boxWidth / 2, tSizeH / 2 - 1);
    std::print("\033[00;30;{}m│ {}{} │\033[00m", color + 10, content, std::string(useTextW - content.length(), ' '));
    GOTO(tSizeW / 2 - boxWidth / 2, tSizeH / 2);
    std::print("\033[00;30;{}m│{}│\033[00m", color + 10, std::string(boxWidth - 2, ' '));
    GOTO(tSizeW / 2 - boxWidth / 2, tSizeH / 2 + 1);
    std::print("\033[00;30;{}m│ Press any key to continue...{}│\033[00m", color + 10, std::string(useTextW - 28 + 1, ' '));
    GOTO(tSizeW / 2 - boxWidth / 2, tSizeH / 2 + 2);
    std::print("\033[00;30;{}m└{}┘\033[00m", color + 10, repeat("─", boxWidth - 2));

    std::cout.flush();
    char c;
    read(0, &c, 1);
}
