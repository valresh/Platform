#include <iostream>
#include <utility>
#include <algorithm>
#include <cassert>
#include <locale.h>

class Ccross::RECT {
public:
    // Типы для координат
    using CoordType = int;
    
    // Конструкторы
    Ccross::RECT() = default;
    
    Ccross::RECT(CoordType left, CoordType top, CoordType right, CoordType bottom)
        : m_left(left), m_top(top), m_right(right), m_bottom(bottom) {
        normalize();
    }
    
    // Конструктор из пары точек
    Ccross::RECT(std::pair<CoordType, CoordType> topLeft, std::pair<CoordType, CoordType> bottomRight)
        : m_left(topLeft.first), m_top(topLeft.second),
          m_right(bottomRight.first), m_bottom(bottomRight.second) {
        normalize();
    }
    
    // Методы доступа
    CoordType left() const { return m_left; }
    CoordType top() const { return m_top; }
    CoordType right() const { return m_right; }
    CoordType bottom() const { return m_bottom; }
    
    // Установка координат
    void setLeft(CoordType value) { m_left = value; normalize(); }
    void setTop(CoordType value) { m_top = value; normalize(); }
    void setRight(CoordType value) { m_right = value; normalize(); }
    void setBottom(CoordType value) { m_bottom = value; normalize(); }
    
    // Нормализация прямоугольника
    void normalize() {
        if (m_left > m_right) std::swap(m_left, m_right);
        if (m_top > m_bottom) std::swap(m_top, m_bottom);
    }
    
    // Сравнение
    bool operator==(const Ccross::RECT& other) const {
        return m_left == other.m_left && m_top == other.m_top &&
               m_right == other.m_right && m_bottom == other.m_bottom;
    }
    
    bool operator!=(const Ccross::RECT& other) const {
        return !(*this == other);
    }
    
    // Объединение прямоугольников
    // Ccross::RECT operator|(const Ccross::RECT& other) const {


    //     return Ccross::RECT(
    //         std::min(m_left, other.m_left),
    //         std::min(m_top, other.m_top),
    //         std::max(m_right, other.m_right),
    //         std::max(m_bottom, other.m_bottom)
    //     );
    // }
    
    // Проверка точки внутри прямоугольника
    bool contains(CoordType x, CoordType y) const {
        return x >= m_left && x <= m_right &&
               y >= m_top && y <= m_bottom;


    }
    
    // Проверка точки внутри прямоугольника через пару
    bool contains(const std::pair<CoordType, CoordType>& point) const {
        return contains(point.first, point.second);
    }
    
private:
    CoordType m_left;
    CoordType m_top;
    CoordType m_right;
    CoordType m_bottom;
};
