#ifndef VAR_HEAP_H
#define VAR_HEAP_H

//*----------------------------------------------------------------------------**
#include "core.h"
//*----------------------------------------------------------------------------**
// template <typename T>
// class var {
//     public:
//         constexpr var() noexcept : data(nullptr) {}
//         std::unique_ptr<T> data;

//         // template <typename U = T, typename = std::void_t<typename U::value_type>>
//         // var(std::initializer_list<typename U::value_type> il)
//         //     : data(std::make_unique<U>(il)) {}

//         template <typename U = T, typename = std::enable_if_t<std::is_copy_constructible_v<U>>>
//         explicit var(const T& value) : data(std::make_unique<T>(value)) {}

//         template <typename U, typename = std::enable_if_t<!std::is_same_v<std::decay_t<U>, var>>>
//         explicit(false) var(U&& value) : data(std::make_unique<T>(std::forward<U>(value))) {}

//         template <typename U = T, typename = std::enable_if_t<std::is_copy_constructible_v<U>>>
//         var(const var& other) : data(std::make_unique<T>(*other.data)) {}

//         var(var&&) noexcept = default;

//         template <typename U = T, typename = std::enable_if_t<std::is_copy_assignable_v<U>>>
//         var& operator=(const var& other) {
//             if(this != &other) *data = *other.data;
//             return *this;
//         }

//         var& operator=(var&&) noexcept = default;

//         template <typename U = T, typename = std::enable_if_t<std::is_copy_assignable_v<U>>>
//         var& operator=(const T& value) {
//             *data = value;
//             return *this;
//         }

//         T* operator->() {
//             if(!data) return nullptr;
//             return data.get();
//         }
//         const T* operator->() const {
//             if(!data) return nullptr;
//             return data.get();
//         }

//         // T& operator*() {
//         //     if (!data) return nullptr;
//         //     return *data;
//         // }
//         // const T& operator*() const {
//         //     if (!data) return nullptr;
//         //     return *data;
//         // }

//         // T* get() noexcept { return data.get(); }
//         // const T* get() const noexcept { return data.get(); }

//         template <typename U = T, typename = std::enable_if_t<std::is_copy_constructible_v<U>>>
//         operator T&() {
//             if(!data) return nullptr;
//             return *data;
//         }
//         template <typename U = T, typename = std::enable_if_t<std::is_copy_constructible_v<U>>>
//         operator const T&() const {
//             if(!data) return nullptr;
//             return *data;
//         }

//         explicit operator bool() const noexcept { return static_cast<bool>(data); }

//         // void reset() noexcept { data.reset(); }

//         // std::unique_ptr<T> release() noexcept { return std::move(data); }
// };
//*----------------------------------------------------------------------------**
// template <typename U>
// var(U) -> var<std::decay_t<U>>;
//*----------------------------------------------------------------------------**

#endif  // VAR_HEAP_H
