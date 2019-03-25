#include <iostream>
#include "vec3.hpp"

using namespace std;

int main() {
    vec3 a(3.f, 2.f, 1.f), b(1.f, 2.f, 3.f);

    cout << a << " " << b << endl;
    cout << cross({1, 0, 0}, {0, 1, 0}) << endl;
    cout << dot(a, b) << endl;
    cout << a * 2.f << endl;
    cout << b / 2.f << endl;
    cout << a + b << endl;
    cout << a - b << endl;
    cout << -a << endl;
}
