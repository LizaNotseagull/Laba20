#include "Money.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <string>

using namespace std;

// Очистка потока ввода после ошибки
void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ ВВОДА С ВЫБОРОМ СПОСОБА

// Ввод unsigned int с клавиатуры (с проверкой) 
unsigned int inputUnsignedFromKeyboard(const string& prompt) {
    unsigned int value;
    cout << prompt;
    while (!(cin >> value)) {
        cout << "Ошибка! Введите целое неотрицательное число: ";
        clearInput();
    }
    return value;
}

// Ввод unsigned int из файла (одно число) 
unsigned int inputUnsignedFromFile(const string& prompt) {
    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;
    ifstream file(filename);
    if (!file) {
        cerr << "Не удалось открыть файл. Используем значение 0.\n";
        return 0;
    }
    unsigned int value;
    if (!(file >> value)) {
        cerr << "Ошибка чтения числа из файла. Используем 0.\n";
        return 0;
    }
    cout << prompt << value << endl;
    return value;
}

// Случайный unsigned int (0..10000) 
unsigned int inputUnsignedRandom(const string& prompt) {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }
    unsigned int value = rand() % 10001; // 0..10000
    cout << prompt << value << endl;
    return value;
}

// Универсальное меню для ввода unsigned int
unsigned int inputUnsignedWithMenu(const string& prompt) {
    cout << "\nВыберите способ ввода для: " << prompt << "\n";
    cout << "1 - Ввод с клавиатуры\n";
    cout << "2 - Ввод из файла\n";
    cout << "3 - Случайная генерация\n";
    cout << "Ваш выбор: ";
    int choice;
    cin >> choice;
    if (cin.fail()) {
        clearInput();
        choice = 0;
    }
    switch (choice) {
    case 1: return inputUnsignedFromKeyboard(prompt);
    case 2: return inputUnsignedFromFile(prompt);
    case 3: return inputUnsignedRandom(prompt);
    default:
        cout << "Неверный выбор. Используем ввод с клавиатуры.\n";
        return inputUnsignedFromKeyboard(prompt);
    }
}

// Ввод short int (копейки) с клавиатуры (0..99) 
short int inputShortFromKeyboard(const string& prompt) {
    short int value;
    cout << prompt;
    while (!(cin >> value) || value < 0 || value >= 100) {
        cout << "Ошибка! Введите число от 0 до 99: ";
        clearInput();
    }
    return value;
}

// Ввод short int из файла 
short int inputShortFromFile(const string& prompt) {
    string filename;
    cout << "Введите имя файла: ";
    cin >> filename;
    ifstream file(filename);
    if (!file) {
        cerr << "Не удалось открыть файл. Используем 0.\n";
        return 0;
    }
    short int value;
    if (!(file >> value)) {
        cerr << "Ошибка чтения числа из файла. Используем 0.\n";
        return 0;
    }
    if (value < 0 || value >= 100) {
        cerr << "Некорректное значение копеек (должно быть 0..99). Исправлено на 0.\n";
        return 0;
    }
    cout << prompt << value << endl;
    return value;
}

// Случайный short int (0..99)
short int inputShortRandom(const string& prompt) {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }
    short int value = rand() % 100;
    cout << prompt << value << endl;
    return value;
}

// Универсальное меню для ввода short int (копейки)
short int inputShortWithMenu(const string& prompt) {
    cout << "\nВыберите способ ввода для: " << prompt << "\n";
    cout << "1 - Ввод с клавиатуры\n";
    cout << "2 - Ввод из файла\n";
    cout << "3 - Случайная генерация\n";
    cout << "Ваш выбор: ";
    int choice;
    cin >> choice;
    if (cin.fail()) {
        clearInput();
        choice = 0;
    }
    switch (choice) {
    case 1: return inputShortFromKeyboard(prompt);
    case 2: return inputShortFromFile(prompt);
    case 3: return inputShortRandom(prompt);
    default:
        cout << "Неверный выбор. Используем ввод с клавиатуры.\n";
        return inputShortFromKeyboard(prompt);
    }
}

// Ввод объекта Money с выбором способа (для второй суммы при вычитании)
Money inputMoneyWithMenu(const string& context) {
    cout << "\n=== Ввод денежной суммы для " << context << " ===\n";
    cout << "Выберите способ ввода:\n";
    cout << "1 - Ввод с клавиатуры\n";
    cout << "2 - Ввод из файла\n";
    cout << "3 - Случайная генерация\n";
    cout << "Ваш выбор: ";
    int choice;
    cin >> choice;
    if (cin.fail()) {
        clearInput();
        choice = 0;
    }

    switch (choice) {
    case 1: {
        unsigned int rub = inputUnsignedFromKeyboard("Введите рубли: ");
        short int kop = inputShortFromKeyboard("Введите копейки (0..99): ");
        return Money(rub, kop);
    }
    case 2: {
        string filename;
        cout << "Введите имя файла: ";
        cin >> filename;
        ifstream file(filename);
        if (!file) {
            cerr << "Не удалось открыть файл. Создана сумма 0 руб.\n";
            return Money();
        }
        unsigned int rub;
        short int kop;
        if (!(file >> rub >> kop)) {
            cerr << "Ошибка чтения данных. Используется 0 руб.\n";
            return Money();
        }
        if (kop < 0 || kop >= 100) {
            cerr << "Некорректные копейки в файле – исправлено нормализацией.\n";
        }
        return Money(rub, kop);
    }
    case 3: {
        unsigned int rub = rand() % 10001;
        short int kop = rand() % 100;
        cout << "Сгенерировано: " << rub << " руб. " << kop << " коп.\n";
        return Money(rub, kop);
    }
    default:
        cout << "Неверный выбор. Используем ввод с клавиатуры.\n";
        unsigned int rub = inputUnsignedFromKeyboard("Введите рубли: ");
        short int kop = inputShortFromKeyboard("Введите копейки (0..99): ");
        return Money(rub, kop);
    }
}

// ФУНКЦИИ ТЕСТИРОВАНИЯ (с меню выбора ввода)

void showCurrent(const Money& m) {
    cout << "Текущая сумма: " << m << endl;
}

// Тест addKopeks: теперь выбор способа ввода для количества копеек
void testAddKopeks(Money& m) {
    cout << "\n--- Тест: addKopeks ---\n";
    showCurrent(m);
    unsigned int k = inputUnsignedWithMenu("количество добавляемых копеек");
    Money res = m.addKopeks(k);
    cout << "Результат: " << res << endl;
}

void testUnaryPlusPlus(Money& m) {
    cout << "\n--- Тест: унарный ++ (префиксный и постфиксный) ---\n";
    showCurrent(m);
    cout << "Префиксный ++ : " << ++m << endl;
    cout << "Постфиксный ++ : " << m++ << " (после операции: " << m << ")" << endl;
}

void testUnaryMinusMinus(Money& m) {
    cout << "\n--- Тест: унарный -- (префиксный и постфиксный) ---\n";
    showCurrent(m);
    cout << "Префиксный -- : " << --m << endl;
    cout << "Постфиксный -- : " << m-- << " (после операции: " << m << ")" << endl;
}

void testCastToUnsignedInt(const Money& m) {
    cout << "\n--- Тест: неявное приведение к unsigned int (рубли) ---\n";
    showCurrent(m);
    unsigned int rub = m;   // неявное преобразование
    cout << "Рубли (копейки отброшены): " << rub << endl;
}

void testCastToDouble(const Money& m) {
    cout << "\n--- Тест: явное приведение к double (копейки как дробная часть) ---\n";
    showCurrent(m);
    double kopPart = static_cast<double>(m);
    cout << "Копейки в рублях (<1): " << kopPart << endl;
}

// Тест вычитания unsigned int (копеек) с выбором способа ввода
void testSubtractUnsigned(Money& m) {
    cout << "\n--- Тест: вычитание беззнакового целого (копеек) ---\n";
    showCurrent(m);
    unsigned int k = inputUnsignedWithMenu("количество вычитаемых копеек");
    Money res = m - k;
    cout << "Результат: " << res << endl;
}

// Тест вычитания Money с выбором способа ввода для второй суммы
void testSubtractMoney(Money& m) {
    cout << "\n--- Тест: вычитание другой денежной суммы ---\n";
    showCurrent(m);
    Money other = inputMoneyWithMenu("вычитания");
    Money res = m - other;
    cout << "Результат: " << res << endl;
}

void demonstrateConstructors() {
    cout << "\n=== Демонстрация конструкторов ===\n";
    Money defaultMoney;
    cout << "1. Конструктор по умолчанию: " << defaultMoney << endl;
    Money paramMoney(123, 45);
    cout << "2. Конструктор с параметрами (123,45): " << paramMoney << endl;
    Money copyMoney(paramMoney);
    cout << "3. Конструктор копирования: " << copyMoney << endl;
   
}

// ГЛАВНОЕ МЕНЮ ТЕСТИРОВАНИЯ
void testingMenu(Money& m) {
    int choice;
    do {
        cout << "\n========== МЕНЮ ТЕСТИРОВАНИЯ ==========\n";
        cout << "1.  addKopeks (добавить копейки)\n";
        cout << "2.  ++ (унарный, добавить 1 копейку)\n";
        cout << "3.  -- (унарный, вычесть 1 копейку)\n";
        cout << "4.  Приведение к unsigned int (рубли)\n";
        cout << "5.  Приведение к double (копейки/100)\n";
        cout << "6.  Вычитание unsigned int (копеек)\n";
        cout << "7.  Вычитание Money (другой суммы)\n";
        cout << "8.  Показать текущую сумму\n";
        cout << "9.  Повторная демонстрация конструкторов\n";
        cout << "0.  Выход из программы\n";
        cout << "Ваш выбор: ";
        cin >> choice;

        if (cin.fail()) {
            clearInput();
            cout << "Некорректный ввод. Повторите.\n";
            continue;
        }

        switch (choice) {
        case 1: testAddKopeks(m); break;
        case 2: testUnaryPlusPlus(m); break;
        case 3: testUnaryMinusMinus(m); break;
        case 4: testCastToUnsignedInt(m); break;
        case 5: testCastToDouble(m); break;
        case 6: testSubtractUnsigned(m); break;
        case 7: testSubtractMoney(m); break;
        case 8: showCurrent(m); break;
        case 9: demonstrateConstructors(); break;
        case 0: cout << "До свидания!\n"; break;
        default: cout << "Неверный пункт. Попробуйте снова.\n";
        }
    } while (choice != 0);
}


int main() {
    setlocale(LC_ALL, "Russian");
    cout << "Лабораторная работа 20. Вариант 8 (Класс Money)\n";

    // Выбор способа инициализации основного объекта
    cout << "Выберите способ инициализации объекта:\n";
    cout << "1 - Ввод с клавиатуры\n";
    cout << "2 - Ввод из файла\n";
    cout << "3 - Случайная генерация\n";
    cout << "Ваш выбор: ";
    int initChoice;
    cin >> initChoice;

    Money money;
    switch (initChoice) {
    case 1: {
        unsigned int rub = inputUnsignedFromKeyboard("Введите рубли: ");
        short int kop = inputShortFromKeyboard("Введите копейки (0..99): ");
        money = Money(rub, kop);
        break;
    }
    case 2: {
        string filename;
        cout << "Введите имя файла: ";
        cin >> filename;
        ifstream file(filename);
        if (!file) {
            cerr << "Не удалось открыть файл. Создана сумма 0 руб.\n";
            money = Money();
        }
        else {
            unsigned int rub;
            short int kop;
            if (!(file >> rub >> kop)) {
                cerr << "Ошибка чтения данных. Используется 0 руб.\n";
                money = Money();
            }
            else {
                if (kop < 0 || kop >= 100) {
                    cerr << "Некорректные копейки в файле – исправлено нормализацией.\n";
                }
                money = Money(rub, kop);
            }
        }
        break;
    }
    case 3: {
        unsigned int rub = rand() % 10001;
        short int kop = rand() % 100;
        cout << "Сгенерировано: " << rub << " руб. " << kop << " коп.\n";
        money = Money(rub, kop);
        break;
    }
    default:
        cout << "Неверный выбор. Используем сумму по умолчанию (0 руб).\n";
        money = Money();
    }

    cout << "\nИсходный объект: " << money << endl;
    demonstrateConstructors(); 

    testingMenu(money);        

    return 0;
}