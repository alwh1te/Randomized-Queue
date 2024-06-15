#pragma once

#include "RandomizedQueue.hpp"

class Subset {
public:
    Subset(int k, std::istream& input, std::ostream& output)
        : k(k), input(input), output(output) {}

    void run() {
        std::string line;
        while (std::getline(input, line)) {
            rq.enqueue(line);
        }

        int count = 0;
        while (!rq.empty() && count < k) {
            output << rq.dequeue() << std::endl;
            count++;
        }
    }

private:
    int k;
    RandomizedQueue<std::string> rq;
    std::istream& input;
    std::ostream& output;
};

inline void subset(int k, std::istream& input, std::ostream& output) {
    Subset subset(k, input, output);
    subset.run();
}
