#ifndef ASYNC_H
#define ASYNC_H

//*----------------------------------------------------------------------------**
#include "core.h"
//*----------------------------------------------------------------------------**
// template <typename T>
// class async {
//     public:
//         template <typename Func, typename... Args, typename R = std::invoke_result_t<Func, Args...>, typename = std::enable_if_t<std::is_convertible_v<R, T>>>
//         explicit async(Func&& func, Args&&... args) : future_(std::async(std::launch::async, std::forward<Func>(func), std::forward<Args>(args)...)) {}

//         // Disable copy
//         async(const async&)            = delete;
//         async& operator=(const async&) = delete;

//         // Enable move
//         async(async&&) noexcept            = default;
//         async& operator=(async&&) noexcept = default;

//         void wait() const { future_.wait(); }

//         T get() { return future_.get(); }

//         bool valid() const noexcept { return future_.valid(); }

//         std::future<T>& getFuture() { return future_; }

//         // using value_type = T;

//         template <typename Func>
//         auto then(Func&& cont) && {
//             using U = std::invoke_result_t<Func, T>;
//             return async<U>([ fut = std::move(future_), cont = std::forward<Func>(cont) ]() mutable {
//                 T res = fut.get();
//                 if constexpr(std::is_void_v<U>) {
//                     cont(res);
//                 } else {
//                     return cont(res);
//                 }
//             });
//         }

//     private:
//         std::future<T> future_;
// };

// template <>
// class async<void> {
//     public:
//         template <typename Func, typename... Args, typename = std::enable_if_t<std::is_invocable_v<Func, Args...>>>
//         explicit async(Func&& func, Args&&... args) : future_(std::async(std::launch::async, std::forward<Func>(func), std::forward<Args>(args)...)) {}

//         async(const async&)                = delete;
//         async& operator=(const async&)     = delete;
//         async(async&&) noexcept            = default;
//         async& operator=(async&&) noexcept = default;

//         void wait() const { future_.wait(); }

//         void get() { future_.get(); }

//         bool valid() const noexcept { return future_.valid(); }

//         std::future<void>& getFuture() { return future_; }

//         // using value_type = void;

//         template <typename Func>
//         auto then(Func&& cont) && {
//             using U = std::invoke_result_t<Func>;
//             return async<U>([ fut = std::move(future_), cont = std::forward<Func>(cont) ]() mutable {
//                 fut.get();
//                 if constexpr(std::is_void_v<U>) {
//                     cont();
//                 } else {
//                     return cont();
//                 }
//             });
//         }

//     private:
//         std::future<void> future_;
// };

// template <typename Func, typename... Args>
// async(Func&&, Args&&...) -> async<std::invoke_result_t<Func, Args...>>;
//*----------------------------------------------------------------------------**

#endif  // ASYNC_H
