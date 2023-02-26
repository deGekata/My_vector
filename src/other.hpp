#define R_UP_DIV_(X, delim) (((X) + (delim) - 1) / (delim))
#define R_UP_DIV(X) R_UP_DIV_(X, sizeof(block_t) * 8)
#define R_UP_NO_DIV(X) (R_UP_DIV(X) * sizeof(block_t) * 8)
#define SET_BIT(VAR, OFFSET, VAL) (VAR) = (((VAR) & ~(1ULL << (OFFSET))) | ((uint64_t)(VAL) << (OFFSET)))
template<>
class Vector<bool> {
public:
    using block_t=uint16_t;
    class reference {
    public:
        reference& operator=(const bool other) {
            (*ptr) = (*ptr) & ~(1ULL << offset) | ((uint64_t)other << offset);
            return *this;
        }

        reference& operator=(const reference& other) {
            return operator=((bool)( (*other.ptr >> offset) & ~1ULL ));
        }
    private:
        uint8_t* ptr = nullptr;
        uint8_t offset = 0;

    };

    Vector();

    explicit Vector(size_t sz);

    Vector(const Vector<bool>& arg);

    Vector<bool>& operator=(const Vector<bool>& sample);

    ~Vector();

    class iterator;

    iterator begin();

    iterator begin() const;

    iterator end();
    
    iterator end() const;

    iterator cbegin() const;

    iterator cend() const;


    bool empty() const;

    size_t capacity() const;

    void reserve(size_t newmalloc);

    void resize(size_t newsize, bool val = bool());

    void shrink_to_fit();

    void swap(Vector<bool>& other);

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

    void push_back(const reference elem);

    void pop_back();

    reference at(int ind);

    const reference at(int ind) const;

    reference operator[](int ind);

    const reference operator[](int ind) const;

    reference front();

    const reference front() const;

    reference back();

    const reference back() const;

    char* data();

    const char* data() const;

    /*----------------------------*/

private:
    size_t       size_;    
    uint8_t*     data_;
    size_t       capacity_;   
    size_t       real_capacity_;
                        
};




Vector<bool>::Vector():
    Vector(0)
{}

//TODO:


//sz passed as bits count
inline Vector<bool>::Vector(size_t sz) :
    size_    ((sz + sizeof(char) - 1) / 8), 
    data_    (nullptr), 
    capacity_((sz + sizeof(char) - 1) / 8),
    real_capacity_((sz + sizeof(char) - 1) / 8)
{
    
    try {
        data_ = new uint8_t[real_capacity_ * sizeof(uint8_t)];
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

Vector<bool>::Vector(const Vector<bool> & arg):
    size_(arg.size_), 
    data_(new uint8_t[arg.real_capacity_ * sizeof(uint8_t)]),
    capacity_(arg.capacity_)
{
    for (size_t index = 0; index < R_UP_DIV(arg.size_); ++index) {
        data_[index] = arg.data_[index];
    }
}

// //TODO:
void Vector<bool>::reserve(size_t newalloc) {
    if (newalloc <= capacity_) return;

    uint8_t* p = new uint8_t[R_UP_DIV(newalloc)];

    for (size_t index = 0; index < R_UP_DIV(size_); ++index) {
        p[index] = data_[index];
    }

    delete[] data_;

    data_ = p;

    capacity_ = newalloc;
    real_capacity_ = R_UP_DIV(newalloc);
}

// //FIX:
void Vector<bool>::resize(size_t newsize, bool val)
{
    reserve(newsize);


    for (size_t index = R_UP_DIV(size_); index < R_UP_DIV(newsize); ++index){
        data_[index] = (~(static_cast<block_t>(val)));
    }

    for (size_t index = size_, cur_bit = 0; index <= R_UP_NO_DIV(size_); ++index, ++cur_bit) {
        SET_BIT(data_[R_UP_DIV(size_)], sizeof(block_t) * 8 - 1 - cur_bit, val);
    }


    // for (int index = size_; index < newsize; ++index)
    //     data_[index] = T();

    size_ = newsize;
}

// //TODO:
// template<>
// void Vector<bool>::initialize_elems(size_t& lft, size_t rht) {
//     size_t cur_idx = lft;
//     try {
//         for (; cur_idx < size_; cur_idx++)
//             new (data_ + cur_idx * sizeof(T)) T();
//     } catch(...) {
//         deinitialize_elems(lft, cur_idx);
//         throw;
//     }
// }

// //TODO:
// template<>
// void Vector<bool>::deinitialize_elems(size_t& lft, size_t rht) {
//     try {
//         for (uint64_t it = lft; it < rht; ++it) 
//             (T*)(data_ + it*sizeof(T))->~T();
//             this->~Vector<bool>();
//     } catch(...) {
//         throw;
//     }
// }

// //TODO:
// template<>
// void* Vector<bool>::operator new(size_t sz) {
//     std::cout << "Operator new for class DataStructures::Vector";

//     char* newData = (char*) calloc(sz, 1);
//     if (newData == nullptr) {
//         throw std::bad_alloc();
//     }

//     return newData;
// }


// //TODO:
// template<>
// void* Vector<bool>::operator new[](uint64_t size) {
//     std::cout << "Operator new[] for class DataStructures::Vector" << std::endl;

//     void* newData = calloc(size, 1);
//     if (!newData) {
//         throw std::bad_alloc();
//     }

//     return newData;
// }

// //TODO:
// template<>
// inline void Vector<bool>::push_back(const reference d)
// {
//     if (this->capacity_ == 0)
//         reserve(8);
//     else if (size_ == capacity_)
//         reserve(2 * capacity_);

//     data_[size_] = d;

//     ++size_;
// }



// //-------------------INDEXATION SECTION---------------------
// template<>
// inline bool & Vector<bool>::at(int ind)
// {
//     if (ind < 0 || size_ <= ind) throw out_of_range();
//     return data_[ind];
// }

// template<>
// inline const bool & Vector<bool>::at(int ind) const
// {
//     if (ind < 0 || size_ <= ind) throw out_of_range();
//     return data_[ind];
// }

// template<>
// inline bool & Vector<bool>::operator[](int ind)
// {
//     return data_[ind];
// }

// template<>
// inline const bool & Vector<bool>::operator[](int ind) const
// {
//     return const_cast<const reference>operator[](ind);
// }


// template<>
// inline reference Vector<bool>::front()
// {
//     return data_[0];
// }

// template<>
// inline const reference Vector<bool>::front() const
// {
//     return data_[0];
// }

// template<>
// inline reference Vector<bool>::back()
// {
//     return data_[size_ - 1];
// }

// template<>
// inline const reference Vector<bool>::back() const
// {
//     return data_[size_ - 1];
// }

// template<>
// inline T* Vector<bool>::data()
// {
//     return data_;
// }



// template<>
// inline const T* Vector<bool>::data() const
// {
//     return data_;
// }

// //-----------------------ITERATOR-------------------------

// template<> class Vector<bool>::iterator
// {
// public:
//     iterator(T* ptr)
//         :_curr(ptr)
//     {}

//     iterator& operator++()
//     {
//         _curr++;
//         return *this;
//     }

//     iterator& operator--()
//     {
//         _curr--;
//         return *this;
//     }

//     reference operator*()
//     {
//         return *_curr;
//     }

//     bool operator==(const iterator& ptr) const
//     {
//         return *_curr == *ptr._curr;
//     }

//     bool operator!=(const iterator& ptr) const
//     {
//         return *_curr != *ptr._curr;
//     }

// private:
//     T* _curr;
// };

// template<>
// inline typename Vector<bool>::iterator Vector<bool>::begin()
// {    
//     return Vector<bool>::iterator(&data_[0]);
// }

// template<>
// inline typename Vector<bool>::iterator Vector<bool>::begin() const
// {
//     return Vector<bool>::iterator(&data_[0]);
// }

// template<>
// inline typename Vector<bool>::iterator Vector<bool>::end()
// {
//     return Vector<bool>::iterator(&data_[size_]);
// }

// template<>
// inline typename Vector<bool>::iterator Vector<bool>::end() const
// {
//     return Vector<bool>::iterator(&data_[size_]);
// }

// template<>
// inline typename Vector<bool>::iterator Vector<bool>::cbegin() const
// {
//     return Vector<bool>::iterator(&data_[0]);
// }

// template<>
// inline typename Vector<bool>::iterator Vector<bool>::cend() const
// {
//     return Vector<bool>::iterator(&data_[size_]);
// }
