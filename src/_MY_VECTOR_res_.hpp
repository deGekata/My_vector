#ifndef _MY_VECTOR_
#define _MY_VECTOR_


#include <inttypes.h>
#include <iostream>
#include <string>
#include <new>
#include <iterator>



namespace DataStructures {
    struct out_of_range
    {

    };

    const uint64_t VectorMultiplier = 2;
    const uint64_t VectorBaseSize   = 10;


    template<class T> 
    class Vector {
    public:
        Vector();

        explicit Vector(size_t sz);

        Vector(const Vector<T>& arg);

        Vector<T>& operator=(const Vector<T>& sample);

        ~Vector();

        class iterator;
        class const_iterator;
        class reverse_iterator;

        iterator begin();

        iterator begin() const;

        iterator end();
        
        iterator end() const;

        const_iterator cbegin() const;

        const_iterator cend() const;

        reverse_iterator rbegin();

        reverse_iterator rbegin() const;

        reverse_iterator rend();

        reverse_iterator rend() const;


        bool empty() const;

        size_t capacity() const;

        void reserve(size_t newmalloc);

        void resize(size_t newsize, T val = T());

        void shrink_to_fit();

        void swap(Vector<T>& other);

        void* operator new(size_t sz);

        void* operator new[](uint64_t size);

        void operator delete(void* ptr);

        void operator delete[](void* ptr);

        void initialize_elems(size_t& lft, size_t rht);

        void deinitialize_elems(size_t& lft, size_t rht);

        void fill_poison();

        size_t size() const;

        size_t max_size() const;


        void clear();

        void push_back(const T& elem);

        void pop_back();

        T& at(int ind);

        const T& at(int ind) const;

        T& operator[](int ind);

        const T& operator[](int ind) const;

        T& front();

        const T& front() const;

        T& back();

        const T& back() const;

        T* data();

        const T* data() const;

        /*----------------------------*/

    private:
        size_t    size_;    
        char*     data_;
        size_t    capacity_;   
                            
    };

    
    // Constructors/Destructor
    template<class T>
    Vector<T>::Vector():
        Vector(0)
    {}

    template<class T>
    inline Vector<T>::Vector(size_t sz) :
        size_    (sz), 
        data_    (nullptr), 
        capacity_(sz)
    {
        
        try {
            data_ = new char[capacity_ * sizeof(T)];
        }
        //tho catches
        catch (std::bad_alloc& ex) {
            std::cerr << "Allocation exception," << std::endl;

            fill_poison();

            throw;
        } catch (...) {
            std::cerr << "Unexpected exception" << std::endl;
            
            fill_poison();
            
            throw;
        }

        uint64_t cur_idx = 0;
        //pasha naidi hren a to poluchish po jope
        initialize_elems(cur_idx, size_);
        
    }

    template<class T>
    inline Vector<T>::Vector(const Vector<T> & arg):
        size_(arg.size_), 
        data_(new T  [arg.size_]),
        capacity_(arg.capacity_)
    {
        for (int index = 0; index < arg.size_; ++index)
            data()[index] = arg.data()[index];
    }

    template<class T>
    Vector<T>::~Vector()
    {
        delete[] data_;
    }

    template<class T>
    inline Vector<T>& Vector<T>::operator=(const Vector<T>& sample) {
        if (this == &sample) {
            return *this;
        }

        Vector<T> a_copy(sample);
        //copy and swap operator=
        this->swap(a_copy);

        //placement new operator=
        // this->~Vector();
        // new (this) Vector(a);
        return *this;
    }

    template<class T>
    inline void Vector<T>::reserve(size_t newalloc) {
        if (newalloc <= capacity_) return;

        char* p = new char[newalloc];

        for (int i = 0; i < size_; ++i)
            p[i] = data()[i];

        delete[] data_;

        data_ = p;

        capacity_ = newalloc;
    }

    template<class T>
    inline void Vector<T>::resize(size_t newsize, T val)
    {
        reserve(newsize);

        for (int index = size_; index < newsize; ++index)
            data()[index] = T();

        size_ = newsize;
    }

    template<class T>
    void Vector<T>::swap(Vector<T>& other) {
        std::swap(this->size_, other.size_);
        std::swap(this->capacity_, other.capacity_);
        std::swap(this->data_, other.data_);
        return;
    }

    template<class T>
    void Vector<T>::initialize_elems(size_t& lft, size_t rht) {
        size_t cur_idx = lft;
        try {
            for (; cur_idx < size_; cur_idx++)
                new (data_ + cur_idx * sizeof(T)) T();
        } catch(...) {
            deinitialize_elems(lft, cur_idx);
            throw;
        }
    }

    template<class T>
    void Vector<T>::deinitialize_elems(size_t& lft, size_t rht) {
        try {
            for (uint64_t it = lft; it < rht; ++it) {
                (data() + it)->~T();
            }
                this->~Vector<T>();
        } catch(...) {
            throw;
        }
    }

    template<class T>
    void Vector<T>::fill_poison() {
        size_     = 0;
        capacity_ = 0;
        data_     = nullptr;
        return;
    }

    template<class T>
    void* Vector<T>::operator new(size_t sz) {
        std::cout << "Operator new for class DataStructures::Vector";

        char* newData = (char*) calloc(sz, 1);
        if (newData == nullptr) {
            throw std::bad_alloc();
        }

        return newData;
    }

    template<class T>
    void* Vector<T>::operator new[](uint64_t size) {
        std::cout << "Operator new[] for class DataStructures::Vector" << std::endl;

        void* newData = calloc(size, 1);
        if (!newData) {
            throw std::bad_alloc();
        }

        return newData;
    }

    template<class T>
    void Vector<T>::operator delete(void* ptr) {
        std::cout << "Operator delete for class DataStructures::Vector" << std::endl;
        free(ptr);
    }

    template<class T>
    void Vector<T>::operator delete[](void* ptr) {
        std::cout << "Operator delete[] for class DataStructures::Vector" << std::endl;
        free(ptr);
    }


    


    template<class T>
    inline bool Vector<T>::empty() const
    {
        return (size_ == 0);
    }

    template<class T>
    inline size_t Vector<T>::size() const
    {
        return size_;
    }

    template<class T>
    inline size_t Vector<T>::capacity() const
    {
        return capacity_;
    }

    


    template<class T>
    inline void Vector<T>::push_back(const T& d)
    {
        if (this->capacity_ == 0)
            reserve(8ULL);
        else if (size_ == capacity_)
            reserve(2 * capacity_);

        data()[size_] = d;

        ++size_;
    }



    //-------------------INDEXATION SECTION---------------------
    template<class T>
    inline T & Vector<T>::at(int ind)
    {
        if (ind < 0 || size_ <= ind) throw out_of_range();
        return data()[ind];
    }

    template<class T>
    inline const T & Vector<T>::at(int ind) const
    {
        if (ind < 0 || size_ <= ind) throw out_of_range();
        return data()[ind];
    }

    template<class T>
    inline T & Vector<T>::operator[](int ind)
    {
        return data()[ind];
    }

    template<class T>
    inline const T & Vector<T>::operator[](int ind) const
    {
        return const_cast<const T&>(operator[](ind));
    }


    template<class T>
    inline T& Vector<T>::front()
    {
        return data()[0];
    }

    template<class T>
    inline const T& Vector<T>::front() const
    {
        return data()[0];
    }

    template<class T>
    inline T& Vector<T>::back()
    {
        return data()[size_ - 1];
    }

    template<class T>
    inline const T& Vector<T>::back() const
    {
        return data()[size_ - 1];
    }

    template<class T>
    inline T* Vector<T>::data()
    {
        return (T*)data_;
    }



    template<class T>
    inline const T* Vector<T>::data() const
    {
        return data_;
    }

    //-----------------------ITERATOR-------------------------

    template<class T> 
    class Vector<T>::iterator : public std::iterator<std::random_access_iterator_tag, T>
    {
    public:
        iterator(T* ptr)
            :_curr(ptr)
        {}

        iterator(iterator&& it) = default;
        iterator(const iterator& it) = default;

        iterator& operator=(const iterator& it) = default;
        iterator& operator=(iterator&& it) = default;

        ~iterator() = default;

        iterator& operator++() {
            _curr++;
            return *this;
        }

        iterator& operator--()  {
            _curr--;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        iterator& operator+=(const std::ptrdiff_t diff) {
            _curr += diff;
            return *this;
        }

        iterator& operator-=(const std::ptrdiff_t diff) {
            _curr -= diff;
            return *this;
        }

        iterator operator+(const std::ptrdiff_t diff) const {
            iterator tmp = *this;
            tmp += diff;
            return tmp;
        }

        iterator operator-(const std::ptrdiff_t diff) const {
            iterator tmp = *this;
            tmp -= diff;
            return tmp;
        }



        T& operator*() {
            return *_curr;
        }

        T& operator[](const std::ptrdiff_t diff) const {return *(_curr + diff);}

        bool operator==(const iterator& ptr) const { return (*this->_curr) == (*ptr._curr);}
        bool operator!=(const iterator& ptr) const { return (*this->_curr) != (*ptr._curr);}
        bool operator< (const iterator& ptr) const { return (*this->_curr) <  (*ptr._curr);}
        bool operator<=(const iterator& ptr) const { return (*this->_curr) <= (*ptr._curr);}
        bool operator> (const iterator& ptr) const { return (*this->_curr) >  (*ptr._curr);}
        bool operator>=(const iterator& ptr) const { return (*this->_curr) >= (*ptr._curr);}

        friend operator-(const iterator& a, const iterator& b) { return a._curr - b._curr;}


    private:
        T* _curr;
    };

    template<class T> 
    class Vector<T>::const_iterator
        : public std::iterator<std::random_access_iterator_tag, T>
    {
    public:
        const_iterator(const T* ptr)
            :_curr(ptr)
        {}

        const_iterator(const_iterator&& it) = default;
        const_iterator(const const_iterator& it) = default;

        const_iterator& operator=(const const_iterator& it) = default;
        const_iterator& operator=(const_iterator&& it) = default;

        ~const_iterator() = default;

        const_iterator& operator++() {
            _curr++;
            return *this;
        }

        const_iterator& operator--()  {
            _curr--;
            return *this;
        }

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++(*this);
            return *this;
        }

        const_iterator& operator--(int) {
            const_iterator tmp = *this;
            --(*this);
            return *this;
        }

        const_iterator& operator+=(const std::ptrdiff_t diff) {
            _curr += diff;
            return *this;
        }

        const_iterator& operator-=(const std::ptrdiff_t diff) {
            _curr -= diff;
            return *this;
        }

        const_iterator operator+(const std::ptrdiff_t diff) const {
            const_iterator tmp = *this;
            tmp += diff;
            return tmp;
        }

        const_iterator operator-(const std::ptrdiff_t diff) const {
            const_iterator tmp = *this;
            tmp -= diff;
            return tmp;
        }



        const T& operator*() {
            return *_curr;
        }

        const T& operator[](const std::ptrdiff_t diff) const {return *(_curr + diff);}

        bool operator==(const const_iterator& ptr) const { return *_curr == *ptr._curr;}
        bool operator!=(const const_iterator& ptr) const { return *_curr != *ptr._curr;}
        bool operator< (const const_iterator& ptr) const { return *_curr <  *ptr._curr;}
        bool operator<=(const const_iterator& ptr) const { return *_curr <= *ptr._curr;}
        bool operator> (const const_iterator& ptr) const { return *_curr >  *ptr._curr;}
        bool operator>=(const const_iterator& ptr) const { return *_curr >= *ptr._curr;}

        friend operator-(const const_iterator& a, const const_iterator& b) { return b - a;}


    private:
        const T* _curr;
    };

        template<class T>
    class Vector<T>::reverse_iterator
        : public std::iterator<std::random_access_iterator_tag, T>
    {
    protected:
        Vector<T>::iterator _curr = Vector<T>::iterator();
    public:
        reverse_iterator() = default;
        explicit reverse_iterator(Vector<T>::iterator itr) : _curr(itr) {}
        // explicit reverse_iterator(Vector<T>::iterator&& itr) : _curr(itr) {}
        
        template<class U>
            requires (!std::is_same_v<U, Vector<T>::iterator> && std::convertible_to<const U&, Vector<T>::iterator>)
        constexpr explicit reverse_iterator(const U& other) : _curr(other.base()) {}
    
        constexpr decltype(auto) operator*() const
        {
            return *std::prev(_curr);
        }
    
        reverse_iterator& operator++() { 
            --_curr; 
            return *this; 
        }
    
        reverse_iterator& operator--() { 
            ++_curr; 
            return *this; 
        }

        reverse_iterator operator++(int) { 
            auto tmp = *this; 
            ++(*this); 
            return tmp; 
        }

        reverse_iterator operator--(int) { 
            auto tmp = *this; 
            --(*this); 
            return tmp; 
        }
    
        Vector<T>::iterator base() const { return _curr; }

        reverse_iterator& operator+=(const std::ptrdiff_t diff) {
            _curr -= diff;
            return *this;
        }

        reverse_iterator& operator-=(const std::ptrdiff_t diff) {
            _curr += diff;
            return *this;
        }

        reverse_iterator operator+(const std::ptrdiff_t diff) const {
            reverse_iterator tmp = *this;
            tmp += diff;
            return tmp;
        }

        reverse_iterator operator-(const std::ptrdiff_t diff) const {
            reverse_iterator tmp = *this;
            tmp -= diff;
            return tmp;
        }
        //spaceshiiip
        
        T& operator[](const std::ptrdiff_t diff) const {return _curr[-diff - 1];}

        bool operator==(const reverse_iterator& ptr) const { return (this->_curr) == (ptr._curr);}
        bool operator!=(const reverse_iterator& ptr) const { return (this->_curr) != (ptr._curr);}
        bool operator< (const reverse_iterator& ptr) const { return (this->_curr) >  (ptr._curr);}
        bool operator<=(const reverse_iterator& ptr) const { return (this->_curr) >= (ptr._curr);}
        bool operator> (const reverse_iterator& ptr) const { return (this->_curr) <  (ptr._curr);}
        bool operator>=(const reverse_iterator& ptr) const { return (this->_curr) <= (ptr._curr);}

        friend operator-(const reverse_iterator& a, const reverse_iterator& b) { return b._curr - a._curr; }
    };

    template<class T>
    inline typename Vector<T>::iterator Vector<T>::begin()
    {    
        return Vector<T>::iterator(&data()[0]);
    }

    template<class T>
    inline typename Vector<T>::iterator Vector<T>::begin() const
    {
        return Vector<T>::iterator(&data()[0]);
    }

    template<class T>
    inline typename Vector<T>::iterator Vector<T>::end()
    {
        return Vector<T>::iterator(&data()[size_]);
    }

    template<class T>
    inline typename Vector<T>::iterator Vector<T>::end() const
    {
        return Vector<T>::iterator(&data()[size_]);
    }

    template<class T>
    inline typename Vector<T>::const_iterator Vector<T>::cbegin() const
    {
        return Vector<T>::const_iterator(&data()[0]);
    }

    template<class T>
    inline typename Vector<T>::const_iterator Vector<T>::cend() const
    {
        return Vector<T>::const_iterator(&data()[size_]);
    }

    template<class T>
    inline typename Vector<T>::reverse_iterator Vector<T>::rbegin()
    {
        return Vector<T>::reverse_iterator(end());
    }

    template<class T>
    inline typename Vector<T>::reverse_iterator Vector<T>::rbegin() const
    {
        return Vector<T>::reverse_iterator(end());
    }

    template<class T>
    inline typename Vector<T>::reverse_iterator Vector<T>::rend() const
    {
        return Vector<T>::reverse_iterator(begin());
    }

    template<class T>
    inline typename Vector<T>::reverse_iterator Vector<T>::rend()
    {
        return Vector<T>::reverse_iterator(begin());
    }

    //instantiation for Vector<bool> and iterator<bool>

    #include "other.hpp"

}
#endif
