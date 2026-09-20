#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <limits>

struct employee {
    int32_t num{};
    char name[10]{};
    double hours{};

    friend std::istream& operator>>(std::istream& in, employee& emp) {
        if (!(in >> emp.num)) {
            throw std::runtime_error("failed to read employee id");
        }
        if (!(in >> emp.name)) {
            throw std::runtime_error("failed to read employee name");
        }
        if (!(in >> emp.hours)) {
            throw std::runtime_error("failed to read employee hours");
        }
        if (emp.hours < 0.0) {
            throw std::logic_error("employee hours cannot be negative");
        }
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const employee& emp) {
        out << emp.num << " " << emp.name << " " << emp.hours;
        return out;
    }
};

#endif