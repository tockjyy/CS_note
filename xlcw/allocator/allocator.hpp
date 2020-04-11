#include <new>
#include <iostream>
#include <cstdlib> // for exit()
#include <climits> // for UNIX_MAX
#include <cstddef>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <functional>
namespace test {

template<class T>
inline T* _allocate(ptrdiff_t size, T*) {
    std::cout << "_allocate called" << size << " " << sizeof(T) << std::endl;
    T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
    if (NULL == tmp) {
        std::cerr << "out of memory" << std::endl;
        exit(0);
    }
    return tmp;
}

template<class T>
inline void _deallocate(T* p) {
    std::cout << "_deallocate called" << sizeof(T) << std::endl;
    ::operator delete(p);
}

template<class T1, class T2>
inline void _construct(T1* p, const T2& value) {
    ::new(p) T1(value);
    std::cout << "_construct called" << sizeof(T2) << std::endl;
}

template<class T>
inline void _destroy(T* p) {
    p->~T();
}

template<class T>
class Allocator
{
public:
    typedef T               value_type;
    typedef T*              pointer;
    typedef const T*        const_pointer;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef size_t          size_type;
    typedef ptrdiff_t       difference_type;

    template<class U>
    struct rebind
    {
        typedef Allocator<U> other;
    };

    pointer allocate(size_type n, const void* hint=0) {
        return _allocate((difference_type)n, (pointer)0);
    }

    void deallocate(pointer p, size_type n) {
        return _deallocate(p);
    }

    void construct(pointer p, const T& value) {
        _construct(p, value);
    }

    void destroy(pointer p) {
        _destroy(p);
    }

    pointer address(reference x) {
        return (pointer)&x;
    }

    const_pointer address(const_reference x) {
        return (const_pointer)&x;
    }

    size_type max_size() const {
        return size_type(UINT_MAX/sizeof(T));
    }
};

} // end of namespace

using namespace std;

int main(int argc, char const *argv[])
{
    do
    {
    cout << "allocator test" << endl;
    vector<long, test::Allocator<long> > v;
    for(int i(0);i < 100; ++i)
    {
        v.push_back((long)i);
    }
    v.erase(v.begin());
    v.push_back(1);
    cout << sizeof(v) <<endl;
    for (int i = 0; i < v.size(); ++i)
    {
        cout << v[i] << endl;
    }
    }while(0);
    return 0;
}
