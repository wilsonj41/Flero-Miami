#ifndef FLEROPP_IO_HPP
#define FLEROPP_IO_HPP

#include <istream>
#include <ostream>

// fleropp IO namespace
namespace fleropp::io {
    /**
     * \brief A `thread_local` output stream with external linkage
     * to enable writing to an arbitrary output stream within a
     * 0-ary or signature-restricted function (such as UDLs).
     * 
     * See \link ScopedRedirect ScopedRedirect \endlink
     */
    inline thread_local std::ostream fppout{nullptr};

    /**
     * \brief A `thread_local` input stream with external linkage to
     * enable reading from an arbitrary input stream.
     * 
     * See \link ScopedRedirect ScopedRedirect \endlink
     */
    inline thread_local std::istream fppin{nullptr};
}

#endif /* FLEROPP_IO_HPP */