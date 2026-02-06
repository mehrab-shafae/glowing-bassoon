
1- fail and not found

```
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
-- Looking for pthread_create in pthreads
-- Looking for pthread_create in pthreads - not found
-- Looking for pthread_create in pthread
-- Looking for pthread_create in pthread - found
```

2- std::string()

```
const auto timestamp = get_timestamp();
        std::cout << "[" << std::string(timestamp) << "] [" << level << "]: " << msg
                  << std::endl;
}

bool file(std::string_view level, std::string_view msg)
{
        const auto timestamp = get_timestamp();
        const auto log_entry = "[" + std::string(timestamp) + "] [" +
                                      std::string(level) + "] " +
                                      std::string(msg) + "\n";
```
