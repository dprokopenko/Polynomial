#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <vector>

template<typename T>
class Polynomial {
public:
    std::vector<T> arr;
    Polynomial(const std::vector<T> &arr)
        : arr(arr) {
    }
    Polynomial(const T& val = T()) {
        arr.push_back(val);
    }
    template<typename Iterator>
    Polynomial(Iterator first, Iterator last)
        : arr(first, last) {
    }
    Polynomial<T>& operator+=(const Polynomial<T>& other) {
        std::size_t sz = std::min(arr.size(), other.arr.size());
        for (std::size_t i = 0; i < sz; ++i) {
            arr[i] += other.arr[i];
        }
        for (std::size_t i = sz; i < other.arr.size(); ++i) {
            arr.push_back(other.arr[i]);
        }
        return *this;
    }
    Polynomial<T>& operator+=(const T& other) {
        return (*this) += Polynomial<T>(other);
    }
    Polynomial<T>& operator-=(const Polynomial<T>& other) {
        std::size_t sz = std::min(arr.size(), other.arr.size());
        for (std::size_t i = 0; i < sz; ++i) {
            arr[i] -= other.arr[i];
        }
        for (std::size_t i = sz; i < other.arr.size(); ++i) {
            arr.push_back(other.arr[i] * T(-1));
        }
        return *this;
    }
    Polynomial<T>& operator-=(const T& other) {
        return (*this) -= Polynomial<T>(other);
    }
    Polynomial<T>& operator*=(const Polynomial<T>& other) {
        std::vector<T> new_arr(arr.size() + other.arr.size());
        for (std::size_t i = 0; i < arr.size(); ++i) {
            for (std::size_t j = 0; j < other.arr.size(); ++j) {
                new_arr[i + j] += arr[i] * other.arr[j];
            }
        }
        arr = new_arr;
        return *this;
    }
    Polynomial<T>& operator*=(const T& other) {
        return (*this) *= Polynomial<T>(other);
    }
    T operator[](std::size_t i) const {
        if (i < arr.size()) {
            return arr[i];
        } else {
            return T(0);
        }
    }
    std::int64_t Degree() const {
        if (!arr.size()) {
            return -1;
        }
        for (std::int64_t i = arr.size() - 1; i >= 0; --i) {
            if (arr[i] != T(0)) {
                return i;
            }
        }
        return -1;
    }
    typename std::vector<T>::const_iterator begin() const {
        return arr.begin();
    }
    typename std::vector<T>::const_iterator end() const {
        std::int64_t d = this->Degree();
        if (d == -1) {
            return arr.begin();
        } else {
            return arr.begin() + d + 1;
        }
    }
    T operator()(const T& x) const {
        T res{}, cur{1};
        for (std::size_t i = 0; i < arr.size(); ++i) {
            res += cur * arr[i];
            cur *= x;
        }
        return res;
    }
};

template<typename T>
bool operator==(const Polynomial<T>& first, const Polynomial<T>& second) {
    std::size_t sz = std::min(first.arr.size(), second.arr.size());
    for (std::size_t i = 0; i < sz; ++i) {
        if (first.arr[i] != second.arr[i]) {
            return false;
        }
    }
    for (std::size_t i = sz; i < first.arr.size(); ++i) {
        if (first.arr[i] != T(0)) {
            return false;
        }
    }
    for (std::size_t i = sz; i < second.arr.size(); ++i) {
        if (second.arr[i] != T(0)) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool operator==(const Polynomial<T>& first, const T& second) {
    return first == Polynomial<T>(second);
}

template<typename T>
bool operator==(const T& first, const Polynomial<T>& second) {
    return Polynomial<T>(first) == second;
}

template<typename T>
bool operator!=(const Polynomial<T>& first, const Polynomial<T>& second) {
    return !(first == second);
}

template<typename T>
bool operator!=(const Polynomial<T>& first, const T& second) {
    return first != Polynomial<T>(second);
}

template<typename T>
bool operator!=(const T& first, const Polynomial<T>& second) {
    return Polynomial<T>(first) != second;
}

template<typename T>
Polynomial<T> operator+(const Polynomial<T>& first, const Polynomial<T>& second) {
    Polynomial<T> ans(first);
    return ans += second;
}

template<typename T>
Polynomial<T> operator+(const Polynomial<T>& first, const T& second) {
    return first + Polynomial<T>(second);
}

template<typename T>
Polynomial<T> operator+(const T& first, const Polynomial<T>& second) {
    return Polynomial<T>(first) + second;
}

template<typename T>
Polynomial<T> operator-(const Polynomial<T>& first, const Polynomial<T>& second) {
    Polynomial<T> ans(first);
    return ans -= second;
}

template<typename T>
Polynomial<T> operator-(const Polynomial<T>& first, const T& second) {
    return first - Polynomial<T>(second);
}

template<typename T>
Polynomial<T> operator-(const T& first, const Polynomial<T>& second) {
    return Polynomial<T>(first) - second;
}

template<typename T>
Polynomial<T> operator*(const Polynomial<T>& first, const Polynomial<T>& second) {
    Polynomial<T> ans(first);
    return ans *= second;
}

template<typename T>
Polynomial<T> operator*(const Polynomial<T>& first, const T& second) {
    return first * Polynomial<T>(second);
}

template<typename T>
Polynomial<T> operator*(const T& first, const Polynomial<T>& second) {
    return Polynomial<T>(first) * second;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const Polynomial<T>& p) {
    if (p.Degree() == -1) {
        out << 0;
        return out;
    }
    std::int64_t d = p.Degree();
    for (std::int64_t i = d; i >= 0; --i) {
        if (p.arr[i] == T(0)) {
            continue;
        }
        if (i) {
            if (p.arr[i] != T(1) && p.arr[i] != T(-1)) {
                if (p.arr[i] < T(0)) {
                    out << p.arr[i];
                } else if (i != d) {
                    out << '+' << p.arr[i];
                } else {
                    out << p.arr[i];
                }
                out << "*x";
                if (i > 1) {
                    out << '^' << i;
                }
            } else if (p.arr[i] == T(-1)) {
                out << "-x";
                if (i > 1) {
                    out << '^' << i;
                }
            } else if (i != d) {
                out << "+x";
                if (i > 1) {
                    out << '^' << i;
                }
            } else {
                out << 'x';
                if (i > 1) {
                    out << '^' << i;
                }
            }
        } else {
            if (p.arr[i] < T(0)) {
                out << p.arr[i];
            } else if (i != d) {
                out << '+' << p.arr[i];
            } else {
                out << p.arr[i];
            }
        }
    }
    return out;
}

template<typename T>
Polynomial<T> operator&(const Polynomial<T>& first, const Polynomial<T>& second) {
    Polynomial<T> cur(T(1)), res;
    for (std::size_t i = 0; i < first.arr.size(); ++i) {
        res += cur * first.arr[i];
        cur *= second;
    }
    return res;
}

template<typename T>
Polynomial<T> operator/(const Polynomial<T>& first, const Polynomial<T>& second) {
    Polynomial<T> a = first;
    Polynomial<T> b = second;
    Polynomial<T> res;
    for (int i = a.Degree(); i >= b.Degree(); i--) {
        if (a[i] == T(0))
            continue;
        std::vector<T> arr(i - b.Degree() + 1);
        arr.back() = T(1);
        Polynomial<T> cur(arr);
        cur = cur * a[i];
        res += cur;
        cur = b * cur;
        a -= cur;
    }
    return a;
}

template<typename T>
Polynomial<T> operator%(const Polynomial<T>& first, const Polynomial<T>& second) {
    return first - ((first / second) * second);
}

template<typename T>
Polynomial<T> operator,(const Polynomial<T>& first, const Polynomial<T>& second) {
    Polynomial<T> a = first;
    Polynomial<T> b = second;
    while (b.Degree() != -1) {
        a = a % b;
        std::swap(a, b);
    }
    return a;
}

void print_vector(const std::vector<int>& v) {
    for (auto el : v) {
        std::cout << el << ' ';
    }
    std::cout << std::endl;
}

int main() {
    return 0;
}
