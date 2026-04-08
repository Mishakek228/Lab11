    #include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <string>
#include <iomanip>
#include <functional>
#include <clocale>

using namespace std;

// ПАРАМЕТРЫ
const size_t N = 15000;
const size_t M = 50000;
const int RAND_MIN = 0;
const int RAND_MAX_VAL = 100000;

// Глобальный генератор (инициализируется один раз)
static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

//  КЛАСС PERSON
class Person {
public:
    string name;
    int age;

    Person() : name(""), age(0) {}
    Person(const string& n, int a) : name(n), age(a) {}

    bool operator<(const Person& other) const {
        if (age != other.age) return age < other.age;
        return name < other.name;
    }

    bool operator==(const Person& other) const {
        return age == other.age && name == other.name;
    }

    friend wostream& operator<<(wostream& os, const Person& p) {
        os << L"{" << wstring(p.name.begin(), p.name.end()) << L", " << p.age << L"}";
        return os;
    }
};

//  ГЕНЕРАТОРЫ ДЛЯ INT
vector<int> generateRandomIntVector(size_t size, int minVal, int maxVal) {
    uniform_int_distribution<int> dist(minVal, maxVal);
    vector<int> vec(size);
    for (auto& x : vec) x = dist(rng);
    return vec;
}

vector<int> generateQueries(size_t count, int minVal, int maxVal) {
    uniform_int_distribution<int> dist(minVal, maxVal);
    vector<int> queries(count);
    for (auto& q : queries) q = dist(rng);
    return queries;
}

// ГЕНЕРАТОРЫ ДЛЯ PERSON (ИСПРАВЛЕНЫ)
vector<Person> generateRandomPersonVector(size_t size) {
    uniform_int_distribution<int> ageDist(1, 90);
    vector<string> names = {"Alice","Bob","Charlie","Diana","Eve","Frank","Grace","Henry"};
    uniform_int_distribution<size_t> nameDist(0, names.size() - 1);
    vector<Person> vec;
    vec.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        vec.emplace_back(names[nameDist(rng)], ageDist(rng));
    }
    return vec;
}

vector<Person> generatePersonQueries(size_t count) {
    uniform_int_distribution<int> ageDist(1, 90);
    vector<string> names = {"Alice","Bob","Charlie","Diana","Eve","Frank","Grace","Henry"};
    uniform_int_distribution<size_t> nameDist(0, names.size() - 1);
    vector<Person> queries;
    queries.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        queries.emplace_back(names[nameDist(rng)], ageDist(rng));
    }
    return queries;
}

// ЛИНЕЙНЫЙ ПОИСК
template<typename T>
bool linearSearch(const vector<T>& vec, const T& value) {
    for (const auto& item : vec)
        if (item == value) return true;
    return false;
}

//  ЗАМЕР ВРЕМЕНИ
long long measureTime(const function<void()>& func, const wstring& description) {
    auto start = chrono::high_resolution_clock::now();
    func();
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);
    wcout << left << setw(35) << description << L" -> " << duration.count() << L" ms" << endl;
    return duration.count();
}

// BUBBLE SORT
template<typename T>
void bubbleSort(vector<T>& arr) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j+1] < arr[j]) {
                swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

//  SELECTION SORT
template<typename T>
void selectionSort(vector<T>& arr) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        size_t min_idx = i;
        for (size_t j = i + 1; j < n; ++j) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        if (min_idx != i) swap(arr[i], arr[min_idx]);
    }
}

// ======================== ЗАГЛУШКИ ========================
template<typename T>
void insertionSort(vector<T>& arr) {
    size_t n = arr.size();
    for (size_t i = 1; i < n; ++i) {
        T key = arr[i];
        size_t j = i;
        while (j > 0 && key < arr[j-1]) {
            arr[j] = arr[j-1];
            --j;
        }
        arr[j] = key;
    }
}

// Вспомогательная функция для слияния двух половин
template<typename T>
void merge(vector<T>& arr, size_t left, size_t mid, size_t right) {
    vector<T> temp(right - left + 1);
    size_t i = left, j = mid + 1, k = 0;
    while (i <= mid && j <= right) {
        if (arr[i] < arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];
    for (size_t p = 0; p < temp.size(); ++p)
        arr[left + p] = temp[p];
}

// Рекурсивная реализация
template<typename T>
void mergeSortImpl(vector<T>& arr, size_t left, size_t right) {
    if (left >= right) return;
    size_t mid = left + (right - left) / 2;
    mergeSortImpl(arr, left, mid);
    mergeSortImpl(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

// Основная функция, которую вызывает main
template<typename T>
void mergeSort(vector<T>& arr) {
    if (!arr.empty())
        mergeSortImpl(arr, 0, arr.size() - 1);
}

template<typename T>
size_t partition(vector<T>& arr, size_t low, size_t high) {
    T pivot = arr[high];
    size_t i = low;
    for (size_t j = low; j < high; ++j) {
        if (arr[j] < pivot) {
            swap(arr[i], arr[j]);
            ++i;
        }
    }
    swap(arr[i], arr[high]);
    return i;
}

template<typename T>
void quickSortImpl(vector<T>& arr, size_t low, size_t high) {
    if (low >= high) return;
    size_t pi = partition(arr, low, high);
    if (pi > 0) quickSortImpl(arr, low, pi-1);
    quickSortImpl(arr, pi+1, high);
}

template<typename T>
void quickSort(vector<T>& arr) {
    if (!arr.empty()) quickSortImpl(arr, 0, arr.size() - 1);
}

template<typename T>
bool binarySearch(const vector<T>& vec, const T& value) {
    size_t left = 0, right = vec.size();
    while (left < right) {
        size_t mid = left + (right - left) / 2;
        if (vec[mid] < value)
            left = mid + 1;
        else if (value < vec[mid])
            right = mid;
        else
            return true;
    }
    return false;
}

// ======================== ОСНОВНАЯ ФУНКЦИЯ ========================
int main() {
    setlocale(LC_ALL, "Russian");
    wcout << L"========== Лабораторная работа №4 (все 6 алгоритмов) ==========" << endl << endl;

    vector<int> original = generateRandomIntVector(N, RAND_MIN, RAND_MAX_VAL);
    vector<int> queries = generateQueries(M, RAND_MIN, RAND_MAX_VAL);

    wcout << L"--- 2. Линейный поиск (без сортировки) ---" << endl;
    long long linearTime = measureTime([&]() {
        for (const auto& q : queries) linearSearch(original, q);
    }, L"Линейный поиск (M=" + to_wstring(M) + L")");

    wcout << endl << L"--- Сравнение алгоритмов сортировки (N=" << N << L") ---" << endl;
    vector<int> v_bubble    = original;
    vector<int> v_selection = original;
    vector<int> v_insertion = original;
    vector<int> v_merge     = original;
    vector<int> v_quick     = original;
    vector<int> v_std       = original;

    measureTime([&]() { bubbleSort(v_bubble);    }, L"Пузырьковая (участник 1)");
    measureTime([&]() { selectionSort(v_selection); }, L"Выбором (участник 1)");
    measureTime([&]() { insertionSort(v_insertion); }, L"Вставками (участник 2)");
    measureTime([&]() { mergeSort(v_merge);      }, L"Слиянием (участник 2)");
    measureTime([&]() { quickSort(v_quick);      }, L"Быстрая (участник 3)");
    measureTime([&]() { sort(v_std.begin(), v_std.end()); }, L"std::sort (эталон)");

    wcout << endl << L"--- Бинарный поиск ---" << endl;
    long long binaryTime = measureTime([&]() {
        for (const auto& q : queries) binarySearch(v_std, q);
    }, L"Бинарный поиск (участник 3 - заглушка)");

    // ----- Работа с классом Person -----
    wcout << endl << L"--- Работа с классом Person (N=5000, M=20000) ---" << endl;
    const size_t NP = 5000, MP = 20000;
    vector<Person> persons = generateRandomPersonVector(NP);
    vector<Person> personQueries = generatePersonQueries(MP);

    measureTime([&]() {
        for (const auto& q : personQueries) linearSearch(persons, q);
    }, L"Линейный поиск Person (несорт.)");

    measureTime([&]() { bubbleSort(persons); }, L"Пузырьковая Person (участник 1)");
    measureTime([&]() { selectionSort(persons); }, L"Выбором Person (участник 1)");

    wcout << endl << L"Первые 5 Person после сортировки (по возрасту, затем имени):" << endl;
    for (size_t i = 0; i < min(size_t(5), persons.size()); ++i) {
        wcout << L"  " << persons[i] << endl;
    }

    wcout << endl << L"========== Завершение ==========" << endl;
    return 0;
}
