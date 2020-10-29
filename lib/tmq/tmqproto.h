/// @file tmqproto.h

#ifndef TMQPROTO_H
#define TMQPROTO_H

#include <cstdint>


#include "tmq/tmq.pb.h"

//============================================================================
namespace tmq {
namespace proto {
namespace v1 {


//----------------------------------------------------------------------------
const int HeaderSize = 9; // bytes

template <typename T>
auto makeHeader(const T& msg, int size)
{
  std::int64_t t((size & 0x7fffffL) << 40);
  t += ((T::msgType() & 0x7fff) << 24);
  t += 0x01090a;

  tmq::Header h;
  h.set_magic(t);
  return h;
}

//----------------------------------------------------------------------------
int messageSize(const tmq::Header &h)
{
  auto magic = h.magic();
  return (magic >> 40);
}



//=========================================================
} // namespace v1
} // namespace proto
} // namespace tmq

#endif
