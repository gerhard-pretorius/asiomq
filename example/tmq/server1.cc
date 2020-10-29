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
  asio::io_context aio;
  auto address = asio::ip::make_address("127.0.0.1");
  asio::ip::tcp::endpoint ep(address, 2020);

  Acceptor a(aio);
  a.listen(ep);
  auto c1 = a.accept();

  msg::Foo foo;
  c1->read(foo);

  cout << foo.value() << endl;

  return 0;
}
