#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <windows.h>
#include "employee.h"

void run_process(const std::string& program, const std::vector<std::string>& args) {
    std::string cmd = program;
    for (const auto& a : args) {
        cmd += " " + a;
    }

    STARTUPINFO si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(si);

    if (!CreateProcess(NULL, &cmd[0], NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        throw std::runtime_error("failed to start process: " + program);
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

void print_binary_file(const std::string& bin_file_name) {
    std::ifstream fin(bin_file_name, std::ios::binary);
    if (!fin.is_open()) {
        throw std::runtime_error("cannot open binary file for reading: " + bin_file_name);
    }
    employee buff{};
    while (fin.read(reinterpret_cast<char*>(&buff), sizeof(buff))) {
        std::cout << buff << "\n";
    }
}

void print_text_file(const std::string& file_name) {
    std::ifstream fin(file_name);
    if (!fin.is_open()) {
        throw std::runtime_error("cannot open text file for reading: " + file_name);
    }
    std::string line;
    while (std::getline(fin, line)) {
        std::cout << line << "\n";
    }
}

int32_t read_positive_int(const std::string& prompt) {
    std::cout << prompt;
    int32_t value = 0;
    if (!(std::cin >> value)) {
        throw std::runtime_error("invalid integer input");
    }
    if (value <= 0) {
        throw std::logic_error("value must be positive");
    }
    return value;
}

double read_non_negative_double(const std::string& prompt) {
    std::cout << prompt;
    double value = 0.0;
    if (!(std::cin >> value)) {
        throw std::runtime_error("invalid numeric input");
    }
    if (value < 0.0) {
        throw std::logic_error("value cannot be negative");
    }
    return value;
}

std::string read_string(const std::string& prompt) {
    std::cout << prompt;
    std::string value;
    if (!(std::cin >> value)) {
        throw std::runtime_error("invalid string input");
    }
    return value;
}

int main() {
    try {
        std::string bin_file_name = read_string("Enter binary file name: ");
        int32_t count = read_positive_int("Enter number of records: ");

        run_process("Creator.exe", { bin_file_name, std::to_string(count) });

        std::cout << "\nBinary file content:\n";
        print_binary_file(bin_file_name);

        std::string report_name = read_string("\nEnter report file name: ");
        double hours_cost = read_non_negative_double("Enter hourly rate: ");

        run_process("Reporter.exe", { bin_file_name, report_name, std::to_string(hours_cost) });

        std::cout << "\nReport content:\n";
        print_text_file(report_name);

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Main error: " << ex.what() << "\n";
        return 1;
    }
}