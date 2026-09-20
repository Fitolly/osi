
#include <iostream>
#include <fstream>
#include <string>
#include <cstdint>
#include <stdexcept>
#include "employee.h"

void create_binary_file(const std::string& bin_file_name, int32_t count) {
    std::ofstream fout(bin_file_name, std::ios::binary);
    if (!fout.is_open()) {
        throw std::runtime_error("cannot open binary file for writing: " + bin_file_name);
    }

    std::cout << "Enter data for " << count << " employees (id, name, hours):\n";
    for (int32_t i = 0; i < count; ++i) {
        employee buff{};
        std::cin >> buff;
        fout.write(reinterpret_cast<const char*>(&buff), sizeof(buff));
        if (!fout) {
            throw std::runtime_error("failed to write employee record to binary file");
        }
    }
}

int main(int argc, char* argv[]) {
    try {

        if (argc < 3) {
            throw std::invalid_argument("usage: Creator <binary_file> <count>");
        }

        std::string bin_file_name = argv[1];
        int32_t count = 0;
        try {
            count = std::stoi(argv[2]);
        }
        catch (const std::exception&) {
            throw std::invalid_argument("count must be a valid integer");
        }
        if (count <= 0) {
            throw std::invalid_argument("count must be positive");
        }

        create_binary_file(bin_file_name, count);
        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Creator error: " << ex.what() << "\n";
        return 1;
    }
}