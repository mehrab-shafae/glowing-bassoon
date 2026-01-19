#ifndef LET_H
#define LET_H

//*----------------------------------------------------------------------------**
#include "core.h"
//*----------------------------------------------------------------------------**
template <typename T>
class let {
    public:
        T data;

        let(const T& value) : data(value) {}

        template <typename U, typename = enable_if_t<!is_same_v<decay_t<U>, let>>>
        let(U&& value) : data(forward<U>(value)) {}

        T&       operator*() { return data; }
        const T& operator*() const { return data; }

        T*       operator->() { return &data; }
        const T* operator->() const { return &data; }

        let& operator=(const T& value) {
            data = value;
            return *this;
        }
};
//*----------------------------------------------------------------------------**
template <typename U>
let(U) -> let<decay_t<U>>;
//*----------------------------------------------------------------------------**

#endif  // LET_H
