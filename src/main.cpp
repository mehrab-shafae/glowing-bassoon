#include "core.hpp"
#include "console.hh"
#include "db.hh"
// --------------------------------------------------------------------
int main(const int argc, const char* argv[]) {
    console::log("Hi");

    db::unit0::join("mehrab");

    // std::string valu;
    // console::in::console(valu, "Hi");
    // console::log(valu);

    return 0;
}
// --------------------------------------------------------------------