#ifndef __DEMO_H__
#define __DEMO_H__

#include <iostream>
#include <vector>
#include <string>
#include <typeinfo>

using namespace std;

template <typename F, typename... Args>
auto call(F&& f, Args&&... args) -> decltype(f(args...)) {
    return f(args...);
}

template <typename T>
size_t print(const std::vector<T>& v) {
    for (const auto& x : v) cout << x << ' '<< '\n';
    return v.size();
}

template <typename T>
string type_name() {
    return typeid(T).name();//
}


void DemoBasics();
void DemoInvoke();

#endif