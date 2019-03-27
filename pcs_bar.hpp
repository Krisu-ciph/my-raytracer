#ifndef pcs_bar_hpp
#define pcs_bar_hpp

#include <iomanip>
#include <iostream>
#include <string>

class pcs_bar {
public:
    pcs_bar() : bar_length(70), message("Rendering"), symbol('#'), os(std::clog) {}

    void update(double percentage) {
        static int last_percentage = -1;
        int cur_len = percentage / 100.0 * bar_length;
        if ((int)percentage > last_percentage) {
            last_percentage = (int)percentage;
            os << "\r[" << std::setw(3) << (int)percentage << "%] [" << std::string(cur_len, symbol) + std::string(bar_length-cur_len, ' ') << ']' << std::flush;
        }
    }

    ~pcs_bar() {
        update(100.0);
        os << '\n';
    }
private:
    int bar_length;
    std::string message;
    char symbol;
    std::ostream &os;
};

#endif
