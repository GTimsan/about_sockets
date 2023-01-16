#include "input.h"

bool Input::ready = false;

void Input::start() {
    std::thread t1(&Input::first_thread, this);
    std::thread t2(&Input::second_thread, this);
    t1.join();
    t2.join();
}

void Input::first_thread() {
    std::string tmp;

    while (true) {
        std::cin >> tmp;
        std::cin.ignore();
        if (!validation(tmp)) {
            continue;
        }
        std::sort(tmp.begin(), tmp.end(), std::greater<int>());
        find_even(tmp);
        {
            std::unique_lock<std::mutex> ul(mtx);
            buffer = tmp;
            ready = true;
            cv.notify_one();
        }
    }
}

void Input::second_thread() {
    std::string tmp;
    uint16_t result;
    Client client;

    while (true) {
        {
            std::unique_lock<std::mutex> ul(mtx);
            cv.wait(ul, []{return ready;});
            tmp = buffer;
            buffer = "";
            ready = false;
        }

        std::cout << tmp << std::endl;
        result = summ(tmp);
        u_char arr[2] = {0};
        arr[0] = result >> 8;
        arr[1] = result;
        // -------------------------------------------------------------------
        // client.send_message(arr, 9812, "192.168.100.12");
        client.send_message(arr);
        // -------------------------------------------------------------------
    }
}

void find_even(std::string &str) {
    std::string tmp;
    tmp.resize(str.size() * 2, '\0');
    int intg;
    for (int i, j = 0; i < (int)str.size(); ++i, ++j) {
        intg = str[i] - '0';
        if (intg % 2 == 0) {
            tmp[j++] = 'K';
            tmp[j] = 'B';
        } else {
            tmp[j] = str[i];
        }
    }
    str = tmp;
}

bool validation(const std::string &str) {
    bool flag = true;
    if (!is_numeric(str)) {
        flag = false;
        std::cerr << "The string must contain only numbers. Enter again." << std::endl;
    } else if (str.size() > 64) {
        flag = false;
        std::cerr << "The number of characters exceeds 64. Please try again." << std::endl;
    }
    return flag;
}

uint16_t summ(const std::string &str) {
    uint16_t summ = 0;
    for (int i = 0; i < (int)str.size(); ++i) {
        if (str[i] == '\0') break;
        if (isdigit(str[i])) {
            summ += str[i] - '0';
        } else {
            i++;
        }
    }
    return summ;

}

bool is_numeric(const std::string &str) {
    bool flag = true;
    for (int i = 0; i < (int)str.size(); ++i) {
        if (!(str[i] <= 57 && str[i] >= 48)) {
            flag = false;
            break;
        }
    }
    return flag;
}


int main() {

	Input i;
	i.start();

    return 0;
}