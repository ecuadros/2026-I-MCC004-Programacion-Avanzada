#include <sstream>    
#include <typeinfo> 

#include "demo.h"     
#include "Type.h"     

using namespace std;

void DemoBasics()
{
    // boolalpha output
    cout << boolalpha 
              << "boolalpha true: " << true << '\n'
              << "boolalpha false: " << false << '\n';
    cout << noboolalpha 
              << "noboolalpha true: " << true << '\n'
              << "noboolalpha false: " << false << '\n';
    // boolalpha parse
    bool b1, b2;
    istringstream is("true false");
    is >> boolalpha >> b1 >> b2;
    cout << '\"' << is.str() << "\" parsed as " << b1 << ' ' << b2 << '\n';
}

void DemoInvoke()
{
 vector<T1> vals{0, 8, 15, 42, 13, -1, 0};

    call([&vals]() -> void {
        cout << "size: " << vals.size() << '\n';
    });

    call([&vals](T1 a) {
        cout << "size: " << vals.size() << ", a=" << a << '\n';
    }, 5);

    print(vals); 
    auto w0 = call(print<T1>, vals);
    cout << "w0=" << w0 << endl;

    vals.pop_back();

    auto w1 = call(print<T1>, vals);
    cout << "w1=" << w1 << endl;

    vals.clear();

    auto w2 = call(print<T1>, vals);
    cout << "w2=" << w2 << endl;

    cout << "Type of vals: " << type_name<decltype(vals)>() << endl;
    cout << "Type of w2: " << type_name<decltype(w2)>() << endl;

    auto pf = &print<T1>;
    cout << "Type of pf: " << type_name<decltype(pf)>() << endl;
}