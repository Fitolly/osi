#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include "employee.h"

void write_report(const std::string& bin_file_name, const std::string& report_name, double hours_cost) {
    std::ifstream fin(bin_file_name, std::ios::binary);
    if (!fin.is_open()) {
        throw std::runtime_error("cannot open binary file for reading: " + bin_file_name);
    }

    std::vector<employee> employees;
    employee buff{};
    while (fin.read(reinterpret_cast<char*>(&buff), sizeof(buff))) {
        employees.push_back(buff);
    }
    if (employees.empty()) {
        throw std::runtime_error("binary file contains no records: " + bin_file_name);
    }

    std::ofstream fout(report_name);
    if (!fout.is_open()) {
        throw std::runtime_error("cannot open report file for writing: " + report_name);
    }

    fout << "Report for file " << bin_file_name << "\n";
    fout << "Employee id, employee name, hours, salary\n";
    for (const auto& emp : employees) {
        fout << emp.num << " " << emp.name << " " << emp.hours << " "
            << emp.hours * hours_cost << "\n";
        if (!fout) {
            throw std::runtime_error("failed to write report record");
        }
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 4) {
            throw std::invalid_argument("usage: Reporter <binary_file> <report_file> <hours_cost>");
        }

        std::string bin_file_name = argv[1];
        std::string report_name = argv[2];
        double hours_cost = 0.0;
        try {
            hours_cost = std::stod(argv[3]);
        }
        catch (const std::exception&) {
            throw std::invalid_argument("hours cost must be a valid number");
        }
        if (hours_cost < 0.0) {
            throw std::invalid_argument("hours cost cannot be negative");
        }

        write_report(bin_file_name, report_name, hours_cost);
        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Reporter error: " << ex.what() << "\n";
        return 1;
    }
}