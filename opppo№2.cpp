#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

struct Dish {
    string name;
    double price;
    string time;
};

// Функция для извлечения названия блюда
string extractName(const string& line) {
    regex name_regex(R"([a-zA-Z]+(?: [a-zA-Z]+)*)");  // Название блюда: последовательность букв (возможны пробелы)
    smatch match;
    if (regex_search(line, match, name_regex)) {
        return match.str();
    }
    return "";
}

// Функция для извлечения цены
double extractPrice(const string& line) {
    regex price_regex(R"(\d+\.\d{2})");  // Цена: число с плавающей точкой
    smatch match;
    if (regex_search(line, match, price_regex)) {
        return stod(match.str());
    }
    return 0.0;
}

// Функция для извлечения времени
string extractTime(const string& line) {
    regex time_regex(R"(\d{2}:\d{2})");  // Время: формат "00:00"
    smatch match;
    if (regex_search(line, match, time_regex)) {
        return match.str();
    }
    return "";
}

// Функция для извлечения данных блюда с использованием регулярных выражений
Dish extractDish(const string& line) {
    string name = extractName(line);
    double price = extractPrice(line);
    string time = extractTime(line);

    // Если время или цена не были найдены, выбрасываем исключение
    if (time.empty() || price == 0.0) {
        throw invalid_argument("Invalid data format: Missing time or price");
    }

    Dish dish;
    dish.name = name;
    dish.price = price;
    dish.time = time;

    return dish;
}

// Функция для чтения данных из файла
vector<Dish> readDishesFromFile(const string& filename) {
    ifstream file(filename);
    vector<Dish> dishes;

    if (!file.is_open()) {
        throw runtime_error("Could not open the file");
    }

    string line;
    while (getline(file, line)) {
        try {
            dishes.push_back(extractDish(line)); // Извлекаем данные блюда
        }
        catch (const invalid_argument& e) {
            cerr << "Error: " << e.what() << " in line: " << line << endl;
        }
    }

    file.close();
    return dishes;
}

// Функция для вывода всех блюд
void printDishes(const vector<Dish>& dishes) {
    for (const auto& dish : dishes) {
        cout << "Dish: " << dish.name
            << ", Price: " << dish.price
            << ", Time: " << dish.time << endl;
    }
}

// Функция для сортировки блюд по цене
void sortDishesByPrice(vector<Dish>& dishes) {
    sort(dishes.begin(), dishes.end(), [](const Dish& a, const Dish& b) {
        return a.price < b.price;
        });
}

// Функция для подсчета общей стоимости для каждого блюда
map<string, double> calculateTotalPrice(const vector<Dish>& dishes) {
    map<string, double> totalPrice;

    for (const auto& dish : dishes) {
        totalPrice[dish.name] += dish.price;
    }

    return totalPrice;
}

int main() {
    try {
        string filename = "oppo.txt";  // Замените на путь к вашему файлу
        vector<Dish> dishes = readDishesFromFile(filename);

        // Сортировка блюд по цене
        sortDishesByPrice(dishes);
        cout << "Dishes sorted by price:" << endl;
        printDishes(dishes);  // Выводим отсортированные блюда

        // Подсчет общей стоимости для каждого блюда
        map<string, double> totalPrice = calculateTotalPrice(dishes);
        cout << "\nTotal price for each dish:" << endl;
        for (const auto& entry : totalPrice) {
            cout << entry.first << ": " << entry.second << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
