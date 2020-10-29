/*

*/

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>

#include <asio.hpp>

#include "tmq/tmq.pb.h"
#include "tmq/tmqproto.h"
#include "tmq/connection.h"
#include "foo.pb.h"
#include "conn1.h"


//============================================================================
using namespace std;

int main(int argc, char *argv[])
{
  msg::Foo foo;
  foo.set_value(42);

  msg::Bar bar;
  bar.set_name("bar");

  asio::io_context aio;
  auto address = asio::ip::make_address("127.0.0.1");
  asio::ip::tcp::endpoint ep(address, 2020);

  Connection c(aio);
  c.connect(ep);
  c.write(foo);

  return 0;
}
