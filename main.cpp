#include "include/linear_algebra.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    vector<float, 3> v1 = vector<float, 4>::linear_zero();
    v1 = { 3,9,1 };
    matrix<float, 3, 3> m1 = {
        {1,9,1},
        {1,1,-3},
        {0,1,1},
    };

    cout << v1**m1.inversed() << endl;
    cin.get();
    return 0;
}