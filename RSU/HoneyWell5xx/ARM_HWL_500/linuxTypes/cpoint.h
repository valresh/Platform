#ifndef CPOINT_H
#define CPOINT_H

#include <iostream>
#include <utility>  // для std::pair
#include <cmath>    // для математических функций
#include <algorithm> // для std::swap
#include <map>

namespace cross {



#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class CPoint {
private:
    double x;
    double y;

public:
    // Конструкторы
    CPoint() : x(0), y(0) {}
    CPoint(double x, double y) : x(x), y(y) {}
    CPoint(const std::pair<double, double>& p) : x(p.first), y(p.second) {}

    // Методы доступа
    double getX() const { return x; }
    double getY() const { return y; }
    void setX(double x) { this->x = x; }
    void setY(double y) { this->y = y; }

    // Операторы сравнения
    bool operator==(const CPoint& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const CPoint& other) const {
        return !(*this == other);
    }

    // Арифметические операторы
    CPoint operator+(const CPoint& other) const {
        return CPoint(x + other.x, y + other.y);
    }
    CPoint operator-(const CPoint& other) const {
        return CPoint(x - other.x, y - other.y);
    }
    CPoint& operator+=(const CPoint& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    CPoint& operator-=(const CPoint& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // Математические методы
    double distanceTo(const CPoint& other) const {
        return std::sqrt(std::pow(x - other.x, 2) + std::pow(y - other.y, 2));
    }
    void rotate(double angle) {
        double rad = angle * M_PI / 180.0;
        double cos_val = std::cos(rad);
        double sin_val = std::sin(rad);
        double new_x = x * cos_val - y * sin_val;
        double new_y = x * sin_val + y * cos_val;
        x = new_x;
        y = new_y;
    }

    // Вывод в поток
    friend std::ostream& operator<<(std::ostream& os, const CPoint& point) {
        return os << "(" << point.x << ", " << point.y << ")";
    }

    // Ввод из потока
    friend std::istream& operator>>(std::istream& is, CPoint& point) {
        return is >> point.x >> point.y;
    }
};

}
#endif // CPOINT_H
