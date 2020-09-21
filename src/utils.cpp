#include <iomanip>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

#include "utils.hpp"

std::size_t get_terminal_width() {
    struct winsize ws {};
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws); // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
    return ws.ws_col;
}

void show_progress(int percent, std::size_t total_length) {
    // 1 space char + 1 [ char + 5 chars for percentage display + 1 ] char = 8 chars
    auto bar_length = total_length > 8 ? total_length - 8 : 0; // NOLINT(readability-magic-numbers)
    auto progress = static_cast<double>(percent * bar_length) / 100.0;
    if (bar_length > 0) {
        std::cout << " [";
        for (std::size_t i = 0; i < bar_length; ++i) {
            if (i < progress) {
                std::cout << '#';
            } else {
                std::cout << '-';
            }
        }
        std::cout << ']';
    }
    // 1 space char + 3 digits + 1 % char = 5 chars
    if (total_length >= 5) { // NOLINT(readability-magic-numbers)
        std::cout << ' ' << std::setfill('0') << std::setw(3) << percent << '%';
    }
    std::cout << std::flush << '\r';
}

void show_render_progress(int percent) {
    static const auto PREFIX_STRING = std::string("Rendering:");
    static const auto TOTAL = get_terminal_width();
    std::cout << "Rendering:";
    show_progress(percent, TOTAL - PREFIX_STRING.length());
}
