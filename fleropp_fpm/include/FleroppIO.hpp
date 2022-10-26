#ifndef FLEROPP_IO_HPP
#define FLEROPP_IO_HPP

#include <ostream>

namespace fleropp_io {
    inline thread_local std::ostream fppout{nullptr};
}

#endif /* FLEROPP_IO_HPP */