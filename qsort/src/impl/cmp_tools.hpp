#ifndef IMPL_CMP_TOOL_HPP
#define IMPL_CMP_TOOL_HPP

namespace impl {

template <class T> struct LessThan {
  bool operator()(const T &compared) const { return compared < value; }

  const T &value;
};

template <class T> struct NotLessThan {
  bool operator()(const T &compared) const { return compared >= value; }

  const T &value;
};

} // namespace impl

#endif  // IMPL_CMP_TOOL_HPP
