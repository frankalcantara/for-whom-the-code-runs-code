#include <coroutine>
#include <iostream>
#include <optional>
#include <utility>

template <typename T>
class Generator {
public:
    struct promise_type {
        std::optional<T> current;

        Generator get_return_object() {
            return Generator{
                std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) noexcept {
            current = value;
            return {};
        }
        void return_void() noexcept {}
        void unhandled_exception() { throw; }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit Generator(Handle handle) : handle_(handle) {}
    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;
    Generator(Generator&& other) noexcept
        : handle_(std::exchange(other.handle_, {})) {}
    ~Generator() {
        if (handle_) handle_.destroy();
    }

    std::optional<T> next() {
        if (!handle_ || handle_.done()) return std::nullopt;
        handle_.resume();
        if (handle_.done()) return std::nullopt;
        return handle_.promise().current;
    }

private:
    Handle handle_;
};

Generator<long long> progression(long long first, long long difference,
                                 int count) {
    long long value = first;
    for (int i = 0; i < count; ++i) {
        co_yield value;
        value += difference;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long first;
    long long difference;
    int count;
    std::cin >> first >> difference >> count;

    auto values = progression(first, difference, count);
    long long sum = 0;
    while (const auto value = values.next()) sum += *value;
    std::cout << sum << '\n';
}
