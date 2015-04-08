/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _NETWORKPLAYER_TYPES_HPP_
#define _NETWORKPLAYER_TYPES_HPP_

#include <istream>
#include <boost/function.hpp>

namespace networkPlayer
{
    /**
     * @struct PlayerFunction
     *
     * @tparam Socket Socket type to use
     *
     * @brief only a struct define a logger function type with a unknown type(like type alias functionnality in c++11).
     *
     * @see http://en.cppreference.com/w/cpp/language/type_alias
     */
    template<typename Socket>
    struct PlayerFunction
    {
        typedef ::boost::function<void (std::istream&, Socket&)> FunctionType;
    };
}

#endif // _NETWORKPLAYER_TYPES_HPP_