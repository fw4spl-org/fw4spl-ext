/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2014.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef _NETWORKPLAYER_ZEROMQLOGGER_HPP_
#define _NETWORKPLAYER_ZEROMQLOGGER_HPP_

#include "NetworkLogger/INetworkLogger.hpp"
#include "NetworkLogger/types.hpp"

#include <zmqNetwork/Socket.hpp>

#include <boost/type.hpp>

namespace networkLogger
{
/**
 * @class ZeroMQLogger
 *
 * @brief Logger class use zeromq socket type
 */
class ZeroMQLogger : public INetworkLogger
{
public:

    /**
     * @typedef SocketType
     *
     * @brief define the OpenIGTLink::Client socket to be the socket type to use
     */
    typedef ::zmqNetwork::Socket SocketType;

    /**
     * @typedef FunctionType
     *
     * @brief logger function type to use
     */
    typedef LoggerFunction<SocketType>::FunctionType FunctionType;

    /// constructor
    ZeroMQLogger();

    /// destructor
    ~ZeroMQLogger();

    /**
     * @brief log received object into the output stream
     * @throw std::runtime_error
     */
    void log(std::ostream &outputStream) throw (std::runtime_error);

    /**
     * @brief configure the logger(zmq connect str)
     *
     * @param[in] argc number of arguments
     * @param[in] argv arguments
     */
    void configure(int argc, char **argv) throw (std::runtime_error);

    /**
     * @brief create a new instance of ZeroMQLogger
     *
     * @param[in] logFunction function who log the date in a output stream
     * @return a new instance of ZeroMQLogger
     */
    static INetworkLogger::sptr create(FunctionType const &logFunction);
private:

    /// zeromq socket
    ::zmqNetwork::Socket m_socket;

    /// zmq connect str (see zmq format to connect like tcp://127.0.0.1:4245)
    std::string m_connectStr;

    /// log user-defined function
    FunctionType m_logFunction;
};
}

#endif // _NETWORKPLAYER_ZEROMQLOGGER_HPP_
