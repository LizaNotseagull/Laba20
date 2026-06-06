#include "Money.h"

// перенос лишних копеек в рубли, защита от отрицательных копеек
void Money::normalize() {
    if (kopeks >= 100) {
        rubles += kopeks / 100;
        kopeks %= 100;
    }
    else if (kopeks < 0) {
        // Сколько рублей нужно занять, чтобы сделать копейки неотрицательными
        int needed = (-kopeks + 99) / 100;
        if (rubles >= (unsigned int)needed) {
            rubles -= needed;
            kopeks += needed * 100;
        }
        else {
            // Недостаточно рублей – обнуляем сумму (защита)
            rubles = 0;
            kopeks = 0;
        }
    }
    // Финальная страховка
    if (kopeks < 0) kopeks = 0;
}

// Конструктор по умолчанию
Money::Money() : rubles(0), kopeks(0) {}

// Конструктор с параметрами – нормализует некорректные копейки
Money::Money(unsigned int rub, short int kop) : rubles(rub), kopeks(kop) {
    if (kopeks < 0 || kopeks >= 100) normalize();
    // Если после нормализации всё ещё плохо – сбрасываем
    if (kopeks < 0 || kopeks >= 100) { rubles = 0; kopeks = 0; }
}

// Конструктор копирования
Money::Money(const Money& other) : rubles(other.rubles), kopeks(other.kopeks) {}

// Добавление произвольного количества копеек (без изменения исходного объекта)
Money Money::addKopeks(unsigned int kops) const {
    unsigned long long total = static_cast<unsigned long long>(rubles) * 100 + kopeks + kops;
    return Money(static_cast<unsigned int>(total / 100),
        static_cast<short int>(total % 100));
}

// Вывод в формате "X руб. Y коп."
std::ostream& operator<<(std::ostream& os, const Money& m) {
    os << m.rubles << " руб. " << m.kopeks << " коп.";
    return os;
}

// Префиксный ++: добавить 1 копейку
Money& Money::operator++() {
    ++kopeks;
    normalize();
    return *this;
}

// Префиксный --: вычесть 1 копейку, защита от отрицания
Money& Money::operator--() {
    if (rubles == 0 && kopeks == 0) return *this; // уже ноль – ничего не делаем
    --kopeks;
    normalize();
    return *this;
}

// Постфиксный ++
Money Money::operator++(int) {
    Money tmp = *this;
    ++(*this);
    return tmp;
}

// Постфиксный --
Money Money::operator--(int) {
    Money tmp = *this;
    --(*this);
    return tmp;
}

// Неявное приведение к unsigned int – возвращает только рубли (копейки отбрасываются)
Money::operator unsigned int() const {
    return rubles;
}

// Явное приведение к double – возвращает долю рубля (копейки/100)
Money::operator double() const {
    return static_cast<double>(kopeks) / 100.0;
}

// Вычитание заданного числа копеек (беззнаковое целое)
Money Money::operator-(unsigned int kops) const {
    long long total = static_cast<long long>(rubles) * 100 + kopeks - kops;
    if (total < 0) {
        std::cerr << "Ошибка: отрицательный результат – заменено на 0 руб.\n";
        return Money(0, 0);
    }
    return Money(static_cast<unsigned int>(total / 100),
        static_cast<short int>(total % 100));
}

// Вычитание другого объекта Money
Money Money::operator-(const Money& other) const {
    long long total = static_cast<long long>(rubles) * 100 + kopeks -
        (static_cast<long long>(other.rubles) * 100 + other.kopeks);
    if (total < 0) {
        std::cerr << "Ошибка: отрицательный результат – заменено на 0 руб.\n";
        return Money(0, 0);
    }
    return Money(static_cast<unsigned int>(total / 100),
        static_cast<short int>(total % 100));
}