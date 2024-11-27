#include <iostream>
#include <cassert>
#include <sstream>
#include <stdexcept>

using namespace std;

class Vector {
private:
    int* arr;
    int capacity;
    int current;

public:
    friend class Stack;

    Vector() : arr(new int[1]), capacity(1), current(0) {}

    ~Vector() {
        delete[] arr;
    }

    explicit Vector(size_t n) : arr(new int[n]), capacity(n), current(0) {
        for (size_t i = 0; i < n; ++i) {
            arr[i] = 0;
        }
    }


    Vector(const Vector& other) : arr(new int[other.capacity]), capacity(other.capacity), current(other.current) {
        for (int i = 0; i < current; ++i) {
            arr[i] = other.arr[i];
        }
    }


    Vector& operator=(const Vector& other) {
        if (this == &other) {
            return *this;
        }

        delete[] arr;

        capacity = other.capacity;
        current = other.current;
        arr = new int[capacity];

        for (int i = 0; i < current; ++i) {
            arr[i] = other.arr[i];
        }

        return *this;
    }


    Vector(Vector&& other) noexcept : arr(other.arr), capacity(other.capacity), current(other.current) {
        other.arr = nullptr;
        other.capacity = 0;
        other.current = 0;
    }


    Vector& operator=(Vector&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        delete[] arr;

        arr = other.arr;
        capacity = other.capacity;
        current = other.current;

        other.arr = nullptr;
        other.capacity = 0;
        other.current = 0;

        return *this;
    }

    void push(int data) {
        if (current == capacity) {
            int* temp = new int[2 * capacity];
            for (int i = 0; i < capacity; ++i) {
                temp[i] = arr[i];
            }
            delete[] arr;
            arr = temp;
            capacity *= 2;
        }
        arr[current++] = data;
    }

    int get(int index) const {
        if (index < current) {
            return arr[index];
        }
        return -1;
    }

    void clear() {
        delete[] arr;
        arr = nullptr;
        current = 0;
        capacity = 0;
    }

    int size() const { return current; }
    int getCapacity() const { return capacity; }

    void resize(int n) {
        if (n == size()) return;

        Vector temp;
        if(n > capacity) {
            capacity = n *2;
            for (int i = 0; i < current; ++i) {
                temp.push(arr[i]);
            }
            for (int i = current; i < n; ++i) {
                temp.push(0);
            }
        }else {
            for (int i = 0; i < n; ++i) {
                temp.push(arr[i]);
            }
        }
        *this = temp;
    }

    void assign(size_t n, int value) {
        clear();
        for (size_t i = 0; i < n; ++i) {
            push(value);  //
        }
    }

    int at(int index) const {
        if (index < 0) {
            index += current;
        }

        if (index < 0) {
            return arr[0];
        }

        if (index >= current) {
            return arr[current - 1];
        }

        return arr[index];
    }
};


class Stack {
private:
     Vector* vec;
public:
    ~Stack() { delete vec; }
    Stack() : vec(new Vector()) {}
    explicit Stack(const size_t n) { vec = new Vector(n); }
    Stack(const int a[], const size_t n) {
        vec = new Vector();
        for (int i = 0; i < n; i++)
            vec->push(a[i]);
    }
    Stack(const Stack& a) : vec(new Vector(*a.vec)) {}

    void clear() {
        vec->clear();
        vec = new Vector();
    }

    int at(int i) const {
        return vec->at(i);
    }

    void resize(size_t n) {
        vec->resize(n);
    }
    void assign(size_t n, int value) {
        vec->assign(n, value);
    }

    size_t length() const { return vec->size(); }

    [[nodiscard]] bool empty() const { return vec->size() == 0; }

    void swap(Stack& a) noexcept { std::swap(vec, a.vec); }

    Stack &operator=(const Stack& s) {
        if (this == &s) return *this;

        delete vec;
        vec = new Vector(*s.vec);
        return *this;
    }

    Stack &operator=( Stack&& s) noexcept {
        if (this == &s) return *this;
        delete vec;
        vec = s.vec;
        s.vec = nullptr;
        return *this;

    }

    friend bool operator==(const Stack& a, const Stack& b) {
        if (a.length() != b.length()) return false;

        for (size_t i = 0; i < a.length(); ++i) {
            if (a.at(i) != b.at(i)) return false;
        }
        return true;
    }

    friend bool operator!=(const Stack&a, const Stack&b) {
        return !(a == b);
    }

    friend bool operator<(const Stack& a, const Stack& b) {
        size_t minLength = std::min(a.length(), b.length());
        for (size_t i = 0; i < minLength; ++i) {
            if (a.at(i) != b.at(i)) return a.at(i) < b.at(i);
        }
        return a.length() < b.length();
    }

    friend bool operator>(const Stack&a , const Stack& b) {
        return b < a;
    }

    friend bool operator<=(const Stack& b , const Stack& a) {
        return !(b < a);
    }

    friend bool operator>=(const Stack& a, const Stack& b) {
        return !(a < b);
    }

    int& operator[](size_t index) {
        return vec->arr[index];
    }

    friend std::ostream &operator<<(std::ostream& os, const Stack& s) {
        os << "[";
        for (size_t i = 0; i < s.length(); ++i) {
            os << s.at(i);
            if (i + 1 < s.length()) os << ", ";
        }
        os << "]";
        return os;
    }

    friend std::istream &operator>>(std::istream& is, Stack& s) {
        int value;
        while (is >> value) {
            s.vec->push(value);
        }
        return is;
    }

    [[nodiscard]] bool contains(int value) const {
        for (size_t i = 0; i < length(); ++i) {
            if (at(i) == value) return true;
        }
        return false;
    }

    [[nodiscard]] size_t count(int value) const {
        size_t count = 0;
        for (size_t i = 0; i < length(); ++i) {
            if (at(i) == value) ++count;
        }
        return count;
    }
};

void print(Stack& a) {
    for (int i = 0; i < a.length(); ++i) {
        cout << a.at(i) << endl;
    }
}

int main(int argc, char const *argv[]) {
    std::stringstream ss{"1 3 5 7 9"};
    Stack a(5);
    ss >> a;
    assert(5 == a.length());
    assert(1 == a[0]);
    assert(9 == a[4]);
    std::cout << a;
    Stack b{a};
    assert(a == b);
    assert(3 == b[1]);
    assert(7 == b[3]);
    b[4] = 0;
    assert(0 == b[4]);
    assert(!b.contains(9));
    assert(b < a);
    assert(a > b);
    std::cout << b;
    Stack c;
    assert(0 == c.length());
    c = b;
    assert(b == c);
    c[1] = c[2] = 7;
    assert(7 == c[1]);
    assert(7 == c[2]);
    assert(3 == c.count(7));
    std::cout << c;
}
