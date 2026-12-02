
#include <iostream>
#include <string>
#include <math.h>
#include <memory>
#include <any>
#include <future>
#include <thread>
#include <utility>
#include <vector>
#include <map>
#include <set>
#include <type_traits>
#include <functional>
#include <chrono>
#include <exception>

#define echo(x) std::cout << x << std::endl // printing to console

template <typename T>
class var
{
public:
    std::unique_ptr<T> data;

    var(T value)
    {
        data = std::make_unique<T>(value);
    }

    T &operator*()
    {
        return *data;
    }

    T *operator->()
    {
        return data.get();
    }

    var &operator=(T value)
    {
        *data = value;
        return *this;
    }
};

int main(){
    {
        var hi = "Hello World";
        // var<int> hi = 5;

        echo("Function a is running");
        echo("var: " << *hi);
    }

    // echo("var: " << *hi);

    return 0;
}