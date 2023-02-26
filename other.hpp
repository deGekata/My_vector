namespace DataStructures {

    template<class T>
    Vector<T>::Vector():
        Vector(0)
    {}
    //TODO:
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
            data_[index] = arg.data_[index];
    }

    //TODO:
    template<class T>
    inline void Vector<T>::reserve(int newalloc) {
        if (newalloc <= capacity_) return;

        T* p = new T[newalloc];

        for (int i = 0; i < size_; ++i)
            p[i] = data_[i];

        delete[] data_;

        data_ = p;

        capacity_ = newalloc;
    }

    //FIX:
    template<class T>
    inline void Vector<T>::resize(size_t newsize, T val)
    {
        reserve(newsize);

        for (int index = size_; index < newsize; ++index)
            data_[index] = T();

        size_ = newsize;
    }

    //TODO:
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

    //TODO:
    template<class T>
    void Vector<T>::deinitialize_elems(size_t& lft, size_t rht) {
        try {
            for (uint64_t it = lft; it < rht; ++it) 
                (T*)(data_ + it*sizeof(T))->~T();
                this->~Vector<T>();
        } catch(...) {
            throw;
        }
    }

    //TODO:
    template<class T>
    void* Vector<T>::operator new(size_t sz) {
        std::cout << "Operator new for class DataStructures::Vector";

        char* newData = (char*) calloc(sz, 1);
        if (newData == nullptr) {
            throw std::bad_alloc();
        }

        return newData;
    }


    //TODO:
    template<class T>
    void* Vector<T>::operator new[](uint64_t size) {
        std::cout << "Operator new[] for class DataStructures::Vector" << std::endl;

        void* newData = calloc(size, 1);
        if (!newData) {
            throw std::bad_alloc();
        }

        return newData;
    }

    //TODO:
    template<class T>
    inline void Vector<T>::push_back(const T& d)
    {
        if (this->capacity_ == 0)
            reserve(8);
        else if (size_ == capacity_)
            reserve(2 * capacity_);

        data_[size_] = d;

        ++size_;
    }



    //-------------------INDEXATION SECTION---------------------
    template<class T>
    inline T & Vector<T>::at(int ind)
    {
        if (ind < 0 || size_ <= ind) throw out_of_range();
        return data_[ind];
    }

    template<class T>
    inline const T & Vector<T>::at(int ind) const
    {
        if (ind < 0 || size_ <= ind) throw out_of_range();
        return data_[ind];
    }

    template<class T>
    inline T & Vector<T>::operator[](int ind)
    {
        return data_[ind];
    }

    template<class T>
    inline const T & Vector<T>::operator[](int ind) const
    {
        return const_cast<const T&>operator[](ind);
    }


    template<class T>
    inline T& Vector<T>::front()
    {
        return data_[0];
    }

    template<class T>
    inline const T& Vector<T>::front() const
    {
        return data_[0];
    }

    template<class T>
    inline T& Vector<T>::back()
    {
        return data_[size_ - 1];
    }

    template<class T>
    inline const T& Vector<T>::back() const
    {
        return data_[size_ - 1];
    }

    template<class T>
    inline T* Vector<T>::data()
    {
        return data_;
    }



    template<class T>
    inline const T* Vector<T>::data() const
    {
        return data_;
    }

    //-----------------------ITERATOR-------------------------

    template<class T> class Vector<T>::iterator
    {
    public:
        iterator(T* ptr)
            :_curr(ptr)
        {}

        iterator& operator++()
        {
            _curr++;
            return *this;
        }

        iterator& operator--()
        {
            _curr--;
            return *this;
        }

        T& operator*()
        {
            return *_curr;
        }

        bool operator==(const iterator& ptr) const
        {
            return *_curr == *ptr._curr;
        }

        bool operator!=(const iterator& ptr) const
        {
            return *_curr != *ptr._curr;
        }

    private:
        T* _curr;
    };

    template<class T>
    inline typename Vector<T>::iterator Vector<T>::begin()
    {    
        return Vector<T>::iterator(&data_[0]);
    }

    template<class T>
    inline typename Vector<T>::iterator Vector<T>::begin() const
    {
        return Vector<T>::iterator(&data_[0]);
    }

    template<class T>
    inline typename Vector<T>::iterator Vector<T>::end()
    {
        return Vector<T>::iterator(&data_[size_]);
    }

    template<class T>
    inline typename Vector<T>::iterator Vector<T>::end() const
    {
        return Vector<T>::iterator(&data_[size_]);
    }

    template<class T>
    inline typename Vector<T>::iterator Vector<T>::cbegin() const
    {
        return Vector<T>::iterator(&data_[0]);
    }

    template<class T>
    inline typename Vector<T>::iterator Vector<T>::cend() const
    {
        return Vector<T>::iterator(&data_[size_]);
    }
}