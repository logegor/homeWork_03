#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <list>

//мой аллокатор
template <class T>
struct MyAllocator
{
    using value_type = T;

    std::shared_ptr<void> pool;
    static constexpr std::size_t poolSize = 10;
    std::size_t currentSize{};
    
    MyAllocator() noexcept
    : pool(new T[poolSize])
    {
    }
    
    template<class U> MyAllocator(const MyAllocator<U>& other) noexcept
    {
        pool = other.pool;
    }
    
    template<class U> bool operator==(const MyAllocator<U>&) const noexcept
    {
        return true;
    }
    template<class U> bool operator!=(const MyAllocator<U>&) const noexcept
    {
        return false;
    }

    template< class U >
    struct rebind
    {
        typedef MyAllocator<U> other;
    };

    T* allocate(const size_t n);
    void deallocate(T* const p, size_t);
};

template <class T>
T* MyAllocator<T>::allocate(const size_t n)
{
    if (n == 0)
    {
        return nullptr;
    }
    if ((n + currentSize) > poolSize)
    {
        throw std::bad_alloc();
    }
    auto val = static_cast<T*>(pool.get()) + currentSize;
    currentSize += n;
    return val;
}

template<class T>
void MyAllocator<T>::deallocate(T * const ptr, size_t n)
{
    currentSize -= n;
}

// мой список

template <class T>
struct Node 
{
  T value;
  Node* next;
  Node(){};
  Node(T _value) : value(_value), next(nullptr)
  {
  }
};

template<typename T, typename Alloc = std::allocator<T>>
struct my_list 
{
    Node<T>* first;
    Node<T>* last;
    typename Alloc::template rebind<Node<T>>::other nodeAlloc;

    my_list() : first(nullptr), last(nullptr) 
    {

    }

    bool is_empty() 
    {
        return first == nullptr;
    }

    void push_back(T _value) 
    {
        Node<T>* p = nodeAlloc.allocate(1);
        p->value = _value;
        p->next = nullptr;
        if (is_empty()) {
            first = p;
            last = p;
            return;
        }
        last->next = p;
        last = p;
    }

    T front()
    {
        return first->value;
    }

    T back()
    {
        return *last->value;
    }

    void pop_front()
    {
        first = first->next;
    }
};

int main()
{
    try
    {
        auto factorial = [](int num, auto& func)-> int{
            if(num > 1)
                return num * func(num - 1, func);
            return num;
        };
        std::map<int, int> map1{};
        std::map<int, int, std::less<int>, MyAllocator<std::pair<const int, int>>> map2{};
        std::list<int> list1{};
        my_list<int, MyAllocator<int>> list2{};
        
        for (int i{}; i < 10; i++)
        {
            map1.insert(std::make_pair(i, factorial(i, factorial)));
            map2.insert(std::make_pair(i, factorial(i, factorial)));
            list1.push_back(i);
            list2.push_back(i);
        }


        std::cout << "map1 : " << std::endl;
        for (int i{}; i < 10; i++)
        {
            std::cout << "key= " << i << " value= " << map1.at(i) << std::endl;
        }
        std::cout << "map2 : " << std::endl;
        for (int i{}; i < 10; i++)
        {
            std::cout << "key= " << i << " value= " << map2.at(i) << std::endl;
        }
        std::cout << "list1 : " << std::endl;
        for (int i{}; i < 10; i++)
        {
            std::cout << "value= " << list1.front() << std::endl;
            list1.pop_front();
        }
        std::cout << "list2 : " << std::endl;
        for (int i{}; i < 10; i++)
        {
            std::cout << "value= " << list2.front() << std::endl;
            list2.pop_front();
        }

    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
