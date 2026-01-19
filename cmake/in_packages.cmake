# by MRB 
#find package and link

## POSIX Library
find_package(Threads REQUIRED)
target_link_libraries(APP PRIVATE Threads::Threads)

