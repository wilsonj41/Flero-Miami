#ifndef FLEROPP_IO_HPP
#define FLEROPP_IO_HPP

#include <ostream>

// fleropp IO namespace
namespace fleropp_io {
    /**
     * \brief A \code thread_local output stream with external linkage to
     * afford to enable writing to an arbitrary output stream within a
     * 0-ary or signature-restricted function (such as UDLs).
     * 
     * See \link ScopedRedirect ScopedRedirect \endlink
     */
    inline thread_local std::ostream fppout{nullptr};
}

#endif /* FLEROPP_IO_HPP */