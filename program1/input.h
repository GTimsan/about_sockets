#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>
#include <condition_variable>
#include "client.h"


class Input {
public:
    void start();

private:
    void first_thread();
    void second_thread();

private:
    std::mutex mtx;
    std::condition_variable cv;
    static bool ready;
    std::string buffer;

};

void find_even(std::string &str);
bool validation(const std::string &str);
uint16_t summ(const std::string &str);
bool is_numeric(const std::string &str);
