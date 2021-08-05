#ifndef COUNTER_HPP
#define COUNTER_HPP

#include <cstdlib>

namespace framework {
    typedef size_t index;
    class Counter {
    private:
        index _i;

    public:
        index get() const noexcept;
        Counter();
        Counter(index start);
        Counter &operator++(int) noexcept;
    };
} // namespace framework
#endif /* COUNTER_HPP */
