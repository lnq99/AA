#include <functional>
#include <chrono>

namespace utils
{
    // using time_t = std::chrono::duration<double>;
    using time_t = std::chrono::nanoseconds;

    template<typename Function, typename... Args, typename ti_t = time_t>
    auto measure(Function&& f, Args&&... a)
    {
        auto start { std::chrono::steady_clock::now() };
        std::invoke(std::forward<Function>(f), std::forward<Args>(a)...);
        auto stop { std::chrono::steady_clock::now() };
        return (std::chrono::duration_cast<ti_t>(stop - start)).count();
    }
}
