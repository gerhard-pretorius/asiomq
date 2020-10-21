/*

*/

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdint>

#include "tmq/tmq.pb.h"
#include "tmq/tmqproto.h"
#include "foo.pb.h"


using namespace std;
int main(int argc, char *argv[])
{
  msg::Foo foo;
  foo.set_value(42);

  msg::Bar bar;
  bar.set_name("bar");

  auto fs = foo.SerializeAsString();
  auto h1 = tmq::proto::v1::makeHeader(foo, fs.size());

  cout << h1.SerializeAsString();

  return 0;
}
