#include <iostream>
#include <print>
#include <widgets.hpp>
#include <results.hpp>
#include <db.hpp>

unsigned int tSizeH;
unsigned int tSizeW;

void drawLogo() {
    std::vector<std::string> logo = {
        "\b\b\b___________.__ __",
        "\\__    ___/|__|  | ______ ",
        "  |    |   |  |  |/ /  _ \\",
        "  |    |   |  |    <  <_> )",
        "  |____|   |__|__|_ \\____/",
        "                   \\/      "
    };

    int logoHeight = std::ssize(logo);
    int startY = (tSizeH / 2 - logoHeight / 2) - 4;

    for (int i = 0; i < logoHeight; ++i) {
        int startX = tSizeW / 2 - std::ssize(logo[i]) / 2;
        std::print("\033[00;3{}m\033[{};{}H{}\033[00m", i, startY + i, startX, logo[i]);
    }
}

std::string repeat(const std::string& str, int times) {
    std::string result;
    result.reserve(str.size() * times);
    for (int i = 0; i < times; ++i) {
        result += str;
    }
    return result;
}

unsigned int searchBar() {
    // Draw
    const unsigned int searchW = static_cast<unsigned int>(0.6 * tSizeW);
    const unsigned int startX = tSizeW / 2 - searchW / 2;

    GOTO(startX, tSizeH / 2);
    std::print("┌{}┐", repeat("─", searchW - 2));
    GOTO(startX, tSizeH / 2 + 1);
    std::print("│{}│", std::string(searchW - 2, ' '));
    GOTO(startX, tSizeH / 2 + 2);
    std::print("└{}┘", repeat("─", searchW - 2));

    return searchW;
}

void showSearchUI() {
    // Get input
    while(1) {
        // Print the terminal size logo
        CLEAR();
        std::println(
            "\033[01mTerminal size\033[00m\n - Width: {}\n - Height: {}\n\n"
            "\033[01mVersion\033[00m\n - " VERSION,
            tSizeW, tSizeH
        );

        drawLogo();
        showMsg("Type \"quit\" to exit.");

        const unsigned int searchSize = searchBar();
        const unsigned int startX = tSizeW / 2 - searchSize / 2;
        GOTO(startX + 1, tSizeH / 2 + 1);
        std::string query = readInput(searchSize - 2, startX + 1, tSizeH / 2 + 1, "Search here...");

        if(query.compare("quit") == 0) {
            CLEAR();
            std::println("Bye!");
            exit(0);
        }
        if(!query.empty()) search(query);
    }
}

int main() {
    // Get the size of the terminal
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    tSizeH = w.ws_row;
    tSizeW = w.ws_col;
    if(tSizeW < 56 || tSizeH < 16) {
        std::println("\033[01;31mError!\033[00;31m\nThe terminal size is too small.\nMinimum is 56x16, got {}x{}\033[00m", tSizeW, tSizeH);
        exit(1);
    }

    initWidgets();
    initDB();
    showSearchUI();
}
