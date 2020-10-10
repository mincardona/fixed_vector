# fixed_vector
`mji::fixed_vector` is a C++17 allocator-aware array-like container with a fixed
size set at run time. It has no dependencies besides the standard library and
occupies a niche between a few existing alternatives:

- unlike `std::unique_ptr<T[]>`, it knows its own size and can perform bounds
  checking
- unlike `std::vector`, it does not perform overallocation or extra bookkeeping
  and cannot accidentally be shrunk or expanded
- unlike `std::array`, its size may be set at run time and is not part of the
  type

As with `std::array`, `mji::fixed_vector` does not support all the requirements
of SequenceContainer but tries to integrate as well as possible with the STL.
