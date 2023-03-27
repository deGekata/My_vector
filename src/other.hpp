#define R_UP_DIV_(X, delim) (((X) + (delim) - 1) / (delim))
#define R_UP_DIV(X) R_UP_DIV_(X, sizeof(block_t) * 8)
#define R_UP_NO_DIV(X) (R_UP_DIV(X) * sizeof(block_t) * 8)
#define R_DOWN_DIV_(X, delim) ((X) / (delim))
#define R_DOWN_DIV(X) R_DOWN_DIV_(X, sizeof(block_t) * 8)
#define R_DOWN_NO_DIV(X) (R_DOWN_DIV(X) * sizeof(block_t) * 8)

#define SET_BIT(VAR, OFFSET, VAL) (VAR) = (((VAR) & ~(1ULL << (OFFSET))) | ((uint64_t)(VAL) << (OFFSET)))

template<>
class Vector<bool> {
public:
    using block_t=uint16_t;
    class BitRef;

    Vector();

    explicit Vector(size_t sz);

    Vector(const Vector<bool>& arg);

    Vector<bool>& operator=(const Vector<bool>& sample);

    ~Vector();

    BitRef begin();

    BitRef begin() const;

    BitRef end();
    
    BitRef end() const;

    BitRef cbegin() const;

    BitRef cend() const;


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

    void push_back(bool val);
    
    void push_back(BitRef val);

    void pop_back();

    BitRef at(int ind);

    const BitRef at(int ind) const;

    BitRef operator[](int ind);

    const BitRef operator[](int ind) const;

    BitRef front();

    const BitRef front() const;

    BitRef back();

    const BitRef back() const;

    char* data();

    const char* data() const;

    /*----------------------------*/

private:
    size_t       size_;    
    uint8_t*     data_;
    size_t       capacity_;   
    size_t       real_capacity_;
                        
};

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

class Vector<bool>::BitRef{
public:
    using iterator_category = std::random_access_iterator_tag ;
    using difference_type   = std::ptrdiff_t;
    using value_type        = bool;

    BitRef(BitRef&& other) : ptr(other.ptr), offset(other.offset) {}

    BitRef(const BitRef& other) : ptr(other.ptr), offset(other.offset) {}

    BitRef& operator=(const bool other) {
        (*ptr) = (((*ptr) & ~(1ULL << offset)) | ((uint64_t)other << offset));
        return *this;
    }

    BitRef& operator=(const BitRef& other) {
        return operator=((bool)( (*other.ptr >> offset) & ~1ULL ));
    }

    BitRef& operator=(BitRef&& other) {
        this->ptr = other.ptr;
        this->offset = other.offset;
        return *this;
    }

    BitRef(block_t* ptr, uint8_t offset) : ptr(ptr), offset(offset) {}

    bool operator*() const { return (((*ptr) >> offset) & 1ULL);}

    BitRef& operator++() {
        if (offset == (sizeof(block_t) * 8 - 1)) {
            ptr++;
            offset = 0;
        } else {
            offset++;
        }
        return *this;
    }

    BitRef operator++(int) {
        BitRef tmp = *this;
        ++(*this);
        return tmp;
    }

    BitRef& operator--() {
        if (offset == 0) {
            ptr--;
            offset = sizeof(block_t) * 8 - 1;
        } else {
            offset--;
        }
        return *this;
    }

    BitRef operator--(int) {
        BitRef tmp = *this;
        --(*this);
        return tmp;
    }

    friend ptrdiff_t operator-(const BitRef& a, const BitRef& b) {return (b.ptr - a.ptr) * (sizeof(block_t) * 8) + b.offset - a.offset;}

    friend bool operator==(const BitRef& a, const BitRef& b) {return (a.ptr == b.ptr) && (a.offset == b.offset);}
    friend bool operator!=(const BitRef& a, const BitRef& b) {return !(a == b);}

    friend bool operator<(const BitRef& a, const BitRef& b)  {return (a.ptr < b.ptr) || (a.ptr == b.ptr && a.offset < b.offset);}
    friend bool operator>(const BitRef& a, const BitRef& b)  {return !(a < b || a == b);}
    
    friend bool operator<=(const BitRef& a, const BitRef& b) {return !(a > b); }
    friend bool operator>=(const BitRef& a, const BitRef& b) {return !(a < b);}

    BitRef& operator+=(int64_t val) {
        this->ptr += (int64_t)((val) / (int64_t)(sizeof(block_t) * 8));
        this->ptr += (int64_t)(val % int64_t((sizeof(block_t) * 8)) + offset) / int64_t((sizeof(block_t) * 8));
        this->offset = (int64_t)(val % int64_t(sizeof(block_t) * 8) + (int64_t)offset) % int64_t(sizeof(block_t) * 8);
        return *this;
    }

    BitRef& operator-=(int64_t val) {
        return this->operator+=(-val);
    }

    BitRef operator+(int64_t val) {
        BitRef tmp = *this;
        tmp += val;
        return tmp;
    }

    BitRef operator-(int64_t val) {
        return this->operator+(-val);
    }

    friend std::ostream& operator<<(std::ostream&, BitRef);

private:
    block_t* ptr = nullptr;
    uint8_t offset = 0;

    friend class Vector<bool>;
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

    for (size_t index = size_, cur_bit = 0; index < R_UP_NO_DIV(size_); ++index, ++cur_bit) {
        SET_BIT(data_[R_DOWN_DIV(size_)], sizeof(block_t) * 8 - 1 - cur_bit, val);
    }

    size_ = newsize;
}

//TODO:
void Vector<bool>::initialize_elems(size_t& lft, size_t rht) {
    // size_t cur_idx = lft;
    // try {
    for (size_t index = R_UP_DIV(lft); index < R_DOWN_DIV(rht); ++index){
        data_[index] = (~(static_cast<block_t>(0)));
    }

    for (size_t index = lft, cur_bit = 0; index < R_UP_NO_DIV(lft); ++index, ++cur_bit) {
        SET_BIT(data_[R_DOWN_DIV(lft)], sizeof(block_t) * 8 - 1 - cur_bit, 0);
    }

    for (size_t index = R_DOWN_NO_DIV(rht), cur_bit = 0; index < rht; ++index, ++cur_bit) {
        SET_BIT(data_[R_DOWN_DIV(rht)], sizeof(block_t) * 8 - 1 - cur_bit, 0);
    }
}

// //TODO:
void Vector<bool>::deinitialize_elems(size_t& lft, size_t rht) {
    (void)rht;
    (void)lft;
    return;
}


void Vector<bool>::fill_poison() {
    size_     = 0;
    capacity_ = 0;
    data_     = nullptr;
    real_capacity_ = 0;
    return;
}

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
void Vector<bool>::push_back(bool val)
{
    if (this->capacity_ == 0)
        reserve(8);
    else if (size_ == capacity_)
        reserve(2 * capacity_);

    operator[](size_++) = val;
    // data_[size_] = val;

    // ++size_;
}

void Vector<bool>::push_back(Vector<bool>::BitRef val)
{
    push_back(((*val.ptr) >> val.offset) & 1ULL);
}



//-------------------INDEXATION SECTION---------------------
Vector<bool>::BitRef Vector<bool>::at(int ind)
{
    if (ind < 0 || size_ <= (size_t)ind) throw out_of_range();
    return (*this)[ind];
}

const Vector<bool>::BitRef Vector<bool>::at(int ind) const
{
    if (ind < 0 || size_ <= (size_t)ind) throw out_of_range();
    return (*this)[ind];
}

Vector<bool>::BitRef Vector<bool>::operator[](int ind)
{
    // std::cout << "creating bitref with offset " << ind - R_DOWN_NO_DIV(ind) << "\n";
    return BitRef((block_t*)data_ + R_DOWN_DIV(ind), ind - R_DOWN_NO_DIV(ind));
}

const Vector<bool>::BitRef Vector<bool>::operator[](int ind) const
{
    return operator[](ind);
}

Vector<bool>::~Vector() {
    delete[] data_;
}

Vector<bool>::BitRef Vector<bool>::front()
{
    return BitRef((block_t*)data_, 0);
}

const Vector<bool>::BitRef Vector<bool>::front() const
{
    return BitRef((block_t*)data_, 0);
}

Vector<bool>::BitRef Vector<bool>::back()
{
    
    return BitRef((block_t*)data_ + R_DOWN_DIV(size_ - 1), (size_ - 1) - R_DOWN_NO_DIV(size_ - 1));
}

const Vector<bool>::BitRef Vector<bool>::back() const
{
    return BitRef((block_t*)data_ + R_DOWN_DIV(size_ - 1), (size_ - 1) - R_DOWN_NO_DIV(size_ - 1));
}

Vector<bool>::BitRef Vector<bool>::begin()
{    
    return Vector<bool>::BitRef((block_t*)data_, 0);
}

Vector<bool>::BitRef Vector<bool>::begin() const
{
    return Vector<bool>::BitRef((block_t*)data_, 0);
}

Vector<bool>::BitRef Vector<bool>::end()
{
    return Vector<bool>::BitRef((block_t*)data_ + R_DOWN_DIV(size_), (size_) - R_DOWN_NO_DIV(size_));
    // return Vector<bool>::BitRef((block_t*)data_ + R_DOWN_DIV(size_ - 1), (size_ - 1) - R_DOWN_NO_DIV(size_ - 1));
}

Vector<bool>::BitRef Vector<bool>::end() const
{
    return Vector<bool>::BitRef((block_t*)data_ + R_DOWN_DIV(size_), (size_) - R_DOWN_NO_DIV(size_));
    // return Vector<bool>::BitRef((block_t*)data_ + R_DOWN_DIV(size_ - 1), (size_ - 1) - R_DOWN_NO_DIV(size_ - 1));
}

Vector<bool>::BitRef Vector<bool>::cbegin() const
{
    return Vector<bool>::BitRef((block_t*)data_, 0);
}

Vector<bool>::BitRef Vector<bool>::cend() const
{
    return Vector<bool>::BitRef((block_t*)data_ + R_DOWN_DIV(size_), size_ - R_DOWN_NO_DIV(size_));
}


std::ostream& operator<<(std::ostream& stream, Vector<bool>::BitRef ref) {
    // std::string s = std::format("{:x}", byte(ref.ptr));
    stream << "ptr: 0x" << std::hex << (uint64_t)ref.ptr << std::dec << "\n"; 
    stream << "offset:" << (uint64_t)ref.offset << std::dec << "\n"; 
    stream << (*ref);
    return stream;
}