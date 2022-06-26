#include <type_traits>


/*begin(),end() detector for C++17*/
template <typename T, typename = int>
struct has_begin_end : std::false_type { };

template <typename T>
struct has_begin_end <T, decltype(
	std::declval<T>().begin(), std::declval<T>().end(), 0
)> : std::true_type { };


/*size() detector for C++03/14*/
template <typename T>
struct has_size {
private:
    template <typename U>
    static auto test(int) -> decltype(std::declval<U>().size(), std::true_type());

    template <typename U>
    static auto test(...) -> decltype(std::false_type());

public:
    using type = decltype(test<T>(0));
    static constexpr bool value = type::value;
};