/// @file conn1.h

#ifndef ASMQ_EXAMPLE_CONN1_H
#define ASMQ_EXAMPLE_CONN1_H

#include <asio.hpp>

#include "tmq/tmq.pb.h"
#include "tmq/tmqproto.h"
#include "tmq/connection.h"
#include "foo.pb.h"

//============================================================================
class Connection : public asmq::Connection<Connection> {
public:

  using Base = asmq::Connection<Connection>;
  using Socket = Base::Socket;
  using EndpointType = Base::EndpointType;
  using Context = Base::Context;

  Connection(Context &context, Socket &&socket)
    : Base(context, std::forward<Socket>(socket))
  {}

  Connection(Context &context)
    : Base(context)
  {}

//----------------------------------------------------------------------------
  template<typename M>
  static
  std::error_code serialize(const M &msg, std::string &hbuffer, std::string &mbuffer)
  {
    std::error_code error = std::error_code();

    if (!msg.SerializeToString(&mbuffer)) {
      return std::error_code(EBADMSG, std::generic_category());
    }
  
    auto header = tmq::proto::v1::makeHeader(msg, mbuffer.size());

    if (!header.SerializeToString(&hbuffer)) {
      return std::error_code(EBADMSG, std::generic_category());
    }

    return error;
  }


//----------------------------------------------------------------------------
  template<typename M, typename B>
  static
  std::error_code deserialize(M &msg, const B &buffer, int size)
  {
    std::error_code error = std::error_code();
    if (!msg.ParseFromArray(buffer, size)) {
      return std::error_code(EBADMSG, std::generic_category());
    }

    return error;
  }


//----------------------------------------------------------------------------
  template<typename M>
  std::error_code read(M &msg)
  {
    std::error_code error = std::error_code();

    tmq::Header h;
    error = Base::read(h, tmq::proto::v1::HeaderSize);

    if (!error) {
      error = Base::read(msg, tmq::proto::v1::messageSize(h));
    }

    return error;
  }

};

//============================================================================
using Acceptor = asmq::Acceptor<Connection>;

#endif
