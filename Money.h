#ifndef MONEY_H
#define MONEY_H

#include <iostream>

// Класс для работы с денежными суммами (рубли + копейки)
class Money {
private:
    unsigned int rubles;   // рубли (целая часть)
    short int kopeks;      // копейки (0..99)

    // Приводит копейки к корректному диапазону (0..99), переносит в рубли
    void normalize();

public:
    // Конструкторы
    Money();                               // по умолчанию (0 руб, 0 коп)
    Money(unsigned int rub, short int kop); // с проверкой копеек
    Money(const Money& other);              // копирования

    // Метод согласно варианту: добавить произвольное количество копеек
    Money addKopeks(unsigned int kops) const;

    // Перегрузка оператора вывода
    friend std::ostream& operator<<(std::ostream& os, const Money& m);

    // Унарные операции (префиксные)
    Money& operator++();    // добавить одну копейку
    Money& operator--();    // вычесть одну копейку (защита от отрицания)

    // Постфиксные ++ и -- (для удобства)
    Money operator++(int);
    Money operator--(int);

    // Операции приведения типа
    operator unsigned int() const;          // неявное -> рубли (копейки отбрасываются)
    explicit operator double() const;       // явное -> копейки как дробная часть (<1)

    // Бинарные операции (вычитание)
    Money operator-(unsigned int kops) const;   // вычесть заданное число копеек
    Money operator-(const Money& other) const;  // вычесть другую сумму

    // Геттеры для отладки
    unsigned int getRubles() const { return rubles; }
    short int getKopeks() const { return kopeks; }
};

#endif