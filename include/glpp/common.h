//
// License: MIT
//

#ifndef GLPP_INCLUDE_GLPP_COMMON_H
#define GLPP_INCLUDE_GLPP_COMMON_H
namespace glpp {
class UnCopyable {
public:
  UnCopyable() = default;
  UnCopyable(const UnCopyable &) = delete;
  UnCopyable &operator=(const UnCopyable &) = delete;
  UnCopyable(UnCopyable &&) = default;
  UnCopyable &operator=(UnCopyable &&) = default;
};
} // namespace glpp
#endif // GLPP_INCLUDE_GLPP_COMMON_H
