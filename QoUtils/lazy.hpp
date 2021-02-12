#include <optional>
#include <functional>

namespace QoUtils 
{

template<class T>
class Lazy
{
    std::function<T()> f;
    mutable std::optional<T> v;

    T && get () const
    {
        if (!v) v = f();
        return std::move(*v);
    }

public:
    Lazy() {}
    template<class F> Lazy(F && _f) : f(std::move(_f)) {}
    template<class F> Lazy<T>& operator =(F && _f)
    { 
        f = std::move(_f); 
        v = std::nullopt; 
        return *this; 
    }

    operator T&&() { return get(); }
    operator T const&() const { return get(); }
};

} // namespace QoUtils.