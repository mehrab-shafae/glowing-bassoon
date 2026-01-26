// ===========================================================
#pragma once
#include "../core/cc.hpp"
// ===========================================================

class jthreadTaskClass
{
public:
    template <typename Func>
    jthreadTaskClass &operator<<(Func &&func)
    {
        std::jthread(std::forward<Func>(func)).detach();
        return *this;
    }
};

jthreadTaskClass Go;
