#include "Counter.hpp"
namespace framework {
    Counter::Counter()
        : _i(0) {}

    Counter::Counter(index start)
        : _i(start) {}

    index Counter::get() const noexcept { return _i; }

    Counter &Counter::operator++(int) noexcept {
        _i += 1;
        return *this;
    }
} // namespace framework
