#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>

void fill(const int n, std::vector<int>& l, std::vector<int>& r, int& empty) {
    int cnt = 0;

    for (int i = 0; i < n; i++) {
        l.push_back(cnt++);
    }
    std::reverse(l.begin(), l.end());

    empty = cnt++;

    for (int i = 0; i < n; i++) {
        r.push_back(cnt++);
    }
}

void print(const std::vector<int>& l, const std::vector<int>& r) {
    std::string s(l.size() * 2 + 1, '_');

    for (const int index : l) {
        s[index] = '>';
    }

    for (const int index : r) {
        s[index] = '<';
    }

    std::cout << s << std::endl;
}

int main() {
    int n;

    std::cin >> n;

    std::vector<int> l;
    std::vector<int> r;

    int empty;

    fill(n, l, r, empty);

    print(l, r);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            if (i % 2 == 0) {
                std::swap(l[j], empty);
                print(l, r);
            }
            else {
                std::swap(r[j], empty);
                print(l, r);
            }
        }
    }

    if (n % 2 == 0) {
        for (int i = 0; i < n; i++) {
            std::swap(l[i], empty);
            print(l, r);
        }
    }
    else {
        for (int i = 0; i < n; i++) {
            std::swap(r[i], empty);
            print(l, r);
        }
    }

    for (int i = n; i > 0; i--) {
        for (int j = n - i; j < n; j++) {
            if (i % 2 == 0) {
                std::swap(r[j], empty);
                print(l, r);
            }
            else {
                std::swap(l[j], empty);
                print(l, r);
            }
        }
    }

    return 0;
}
