/// @file connection.h

#ifndef ASMQ_CONNECTION_H
#define ASMQ_CONNECTION_H


#include <iostream>
#include <list>
#include <memory>
#include <initializer_list>
#include <functional>

#include <asio.hpp>


//////////////////////////////////////////////////////////////////////////////
namespace asmq {


//============================================================================
template <typename T>
class Connection : public std::enable_shared_from_this<T> {
public:
  using Socket = asio::generic::stream_protocol::socket;
  using EndpointType = Socket::endpoint_type;
  using Context = asio::io_context;

  Connection(Context &context, Socket &&socket)
    : m_context(context), m_socket(std::move(socket))
  {}

  Connection(Context &context)
    : m_context(context), m_socket(context)
  {}


  virtual ~Connection() {}

//----------------------------------------------------------------------------
  template<typename ...Args>
  static auto
  create(Args&& ...params)
  {
    return std::make_unique<T>(std::forward<Args>(params)...);
  }

//----------------------------------------------------------------------------
  template<typename M>
  std::error_code write(const M &msg)
  {
    std::error_code error = std::error_code();
    std::string hbuffer, mbuffer;

    error = T::serialize(msg, hbuffer, mbuffer);

    if (error) {
      return error;
    }

    std::vector<asio::const_buffer> buffer;
    buffer.emplace_back(asio::buffer(hbuffer));
    buffer.emplace_back(asio::buffer(mbuffer));

    auto len = hbuffer.size() + mbuffer.size();
    auto bytes = asio::write(m_socket, buffer, error);

    if (error) {
      return error;
    }

    if (bytes != len) {
      error = std::error_code(ENOSPC, std::system_category());
    }

    return error;
  }



//----------------------------------------------------------------------------
  template<typename M>
  std::error_code read(M &msg, int size)
  {
    std::error_code error = std::error_code();

    BufferType buffer(size);
    auto bytes = asio::read(m_socket, asio::buffer(buffer), error);

    if (error) {
      return error;
    }

    if (bytes < size) {
      return std::error_code(EMSGSIZE, std::generic_category());
    }

    error = T::deserialize(msg, buffer.data(), size);

    if (error) {
      return error;
    }

    return error;
  }

//----------------------------------------------------------------------------
  void connect(const EndpointType &ep) { m_socket.connect(ep); }
  void cancel() { m_socket.cancel(); }
  void close() { m_socket.close(); }


//------------------------------------------------------------------------
protected:
  using BufferType = std::vector<uint8_t>;
  using SharedBufferType = std::shared_ptr<BufferType>;


//----------------------------------------------------------------------------
protected:
  Context &m_context;
  Socket m_socket;
  std::error_code m_error_code;

}; // Connection


//============================================================================
template <typename C, typename T = asio::ip::tcp>
class Acceptor {
public:
  using Context = asio::io_context;
  using Connection = C;
  using AcceptorT = typename T::acceptor;
  using Socket = typename T::socket;
  using Endpoint = typename T::endpoint;


  Acceptor(Context &c)
    : m_context(c), m_acceptor(c), m_socket(c)
  {}


  void listen(Endpoint &ep)
  {
    m_acceptor.open(ep.protocol());
    asio::ip::tcp::acceptor::reuse_address option(true);
    m_acceptor.set_option(option);
    m_acceptor.bind(ep);
    m_acceptor.listen();
  }


  auto accept()
  {
    m_acceptor.accept(m_socket);
    return C::create(m_context, std::move(m_socket));
  }


private:
  Context &m_context;
  AcceptorT m_acceptor;
  Socket m_socket;
};  // Acceptor


//=========================================================
} // namespace asmq

#endif

