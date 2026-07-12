#include <iostream>

using namespace std;


int sumar(int a, int b)
{
    return a + b;
}


int restar(int a, int b)
{
    return a - b;
}

int main()
{
    cout << "Suma: " << sumar(5, 3) << endl;
    cout << "Resta: " << restar(5, 3) << endl;

    return 0;
}