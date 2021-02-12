#ifndef STL_FORWARD_DECLARATION
#define STL_FORWARD_DECLARATION

namespace std
{
    template<typename, std::size_t> class array;
    template<typename...> class tuple;

    template<typename, typename> class vector;
    template<typename, typename> class deque;

    template<typename, typename> class forward_list;
    template<typename, typename> class list;

    template<typename> class optional;
}

#endif