
// template <typename T>
// class var
// {
// public:
//     std::unique_ptr<T> data;

//     var(T value)
//     {
//         data = std::make_unique<T>(value);
//     }

//     T &operator*()
//     {
//         return *data;
//     }

//     T *operator->()
//     {
//         return data.get();
//     }

//     var &operator=(T value)
//     {
//         *data = value;
//         return *this;
//     }
// };

// void var_test(){
//     var hi = 5;
//     var<int> hi = 5;
// }