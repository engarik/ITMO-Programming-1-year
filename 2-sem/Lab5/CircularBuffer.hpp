#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <iterator>

#ifndef LAB5_CIRCULARBUFFER_HPP
#define LAB5_CIRCULARBUFFER_HPP

template <class T>
class CircularBuffer {
public:
    // Default constructor
    explicit CircularBuffer(size_t capacity = 1) {
        if (capacity == 0) throw std::invalid_argument("Capacity of buffer must be positive number");
        this->capacity = capacity;
        this->currentCapacity = std::min(capacity, (size_t) 2);
        this->n = 0;
        this->array = new T[currentCapacity];
        this->startPos = 0;
        this->endPos = 0;
    }

    // Copy constructor
    CircularBuffer(const CircularBuffer& other) :
    capacity(other.capacity),
    currentCapacity(other.currentCapacity),
    n(other.n),
    array(new T[currentCapacity]),
    startPos(other.startPos),
    endPos(other.endPos)
    {
        for (size_t i = 0; i < n; ++i) {
            array[(startPos + i) % currentCapacity] = other.array[(startPos + i) % currentCapacity];
        }
    }

    // Destructor
    ~CircularBuffer() {
        delete [] array;
    }

    // Copy-assign operator
    CircularBuffer& operator=(const CircularBuffer& other) {
        if (this != &other) {
            delete [] array;
            capacity = other.capacity;
            currentCapacity = other.currentCapacity;
            n = other.n;
            startPos = other.startPos;
            endPos = other.endPos;
            array = new T[currentCapacity];
            for (size_t i = 0; i < n; ++i) {
                array[(startPos + i) % currentCapacity] = other.array[(startPos + i) % currentCapacity];
            }
        }

        return *this;
    }

    void addFirst(const T& value) {
        if (currentCapacity != capacity) {
            resize(capacity);
        }
        if (n != 0) {
            dec(startPos);
            if (startPos == endPos) dec(endPos);
        }
        array[startPos] = value;
        n = std::min(n + 1, capacity);
    }

    void addLast(const T& value) {
        if (n + 1 > currentCapacity) {
            resize((currentCapacity * 3) / 2 + 1);
        }
        if (n != 0) {
            inc(endPos);
            if (startPos == endPos) inc(startPos);
        }
        array[endPos] = value;
        n = std::min(n + 1, capacity);
    }

    T removeFirst() {
        checkEmpty();
        size_t pos = startPos;
        if (n > 1) inc(startPos);
        --n;
        return array[pos];
    }

    T removeLast() {
        checkEmpty();
        size_t pos = endPos;
        if (n > 1) dec(endPos);
        --n;
        return array[pos];
    }

    const T& getFirst() const {
        checkEmpty();
        return array[startPos];
    }

    const T& getLast() const {
        checkEmpty();
        return array[endPos];
    }

    const T& get(size_t pos) const {
        checkEmpty();
        if (pos >= n) throw std::out_of_range("No such element");
        else return array[(startPos + pos) % currentCapacity];
    }

    void changeCapacity(size_t newCapacity) {
        if (newCapacity == 0) throw std::out_of_range("New capacity must be a positive value");
        T * newArray = new T[newCapacity];
        for (size_t i = 0; i < std::min(n, newCapacity); ++i) {
            newArray[i] = array[(startPos + i) % currentCapacity];
        }
        delete [] array;
        array = newArray;
        capacity = newCapacity;
        currentCapacity = newCapacity;
        n = std::min(n, newCapacity);
        startPos = 0;
        endPos = n - 1;
    }

    struct iterator : std::iterator<std::random_access_iterator_tag, T> {
    public:
        iterator(T * cur) : cur(cur) {}

        iterator() = default;

        iterator(const iterator &) = default;

        iterator &operator=(const iterator &) = default;

        ~iterator() = default;

        T &operator* () {
            return *cur;
        }
        iterator& operator++() {
            ++cur;
            return *this;
        }

        iterator operator++(T) {
            iterator old(*this);
            ++cur;
            return old;
        }

        iterator& operator--() {
            --cur;
            return *this;
        }

        iterator operator--(T) {
            iterator old(*this);
            --cur;
            return old;
        }

        iterator& operator+=(ptrdiff_t pos) {
            cur += pos;
            return *this;
        }

        iterator& operator-=(ptrdiff_t pos) {
            cur -= pos;
            return *this;
        }

        iterator friend operator+(iterator iter, ptrdiff_t pos) {
            iter += pos;
            return iter;
        }

        iterator friend operator+(ptrdiff_t pos, iterator iter) {
            return iter.cur + pos;
        }

        iterator friend operator-(iterator iter, ptrdiff_t pos) {
            iter -= pos;
            return iter;
        }

        ptrdiff_t operator-(const iterator& iter) {
            return cur - iter.cur;
        }

        friend bool operator==(const iterator& lhs, const iterator& rhs) {
            return lhs.cur == rhs.cur;
        }

        friend bool operator!=(const iterator& lhs, const iterator& rhs) {
            return lhs.cur != rhs.cur;
        }

        friend bool operator<(const iterator& lhs, const iterator& rhs) {
            return lhs.cur < rhs.cur;
        }

        friend bool operator>(const iterator& lhs, const iterator& rhs) {
            return lhs.cur > rhs.cur;
        }

        friend bool operator<=(const iterator& lhs, const iterator& rhs) {
            return lhs.cur <= rhs.cur;
        }

        friend bool operator>=(const iterator& lhs, const iterator& rhs) {
            return lhs.cur >= rhs.cur;
        }
    private:
        T * cur;
    };

    iterator begin() {
        return iterator(array + startPos);
    }

    iterator end() {
        return iterator(array + endPos + 1);
    }

    template <class M>
    friend std::ostream& operator<<(std::ostream& stream, const CircularBuffer<M>& buffer);

private:
    size_t capacity, currentCapacity, n;
    T * array;
    size_t startPos, endPos;

    void inc(size_t & pos) {
        pos = (pos + 1) % currentCapacity;
    }

    void dec(size_t & pos) {
        pos = ((pos - 1) + capacity) % currentCapacity;
    }

    void checkEmpty() const {
        if (n == 0) throw std::out_of_range("Buffer is empty");
    }

    void resize(size_t toEnlarge) {
        size_t newCapacity = std::min(toEnlarge, capacity);
        T * newArray = new T[newCapacity];
        for (size_t i = 0; i < n; ++i) {
            newArray[(startPos + i) % newCapacity] = array[(startPos + i) % currentCapacity];
        }
        delete [] array;
        array = newArray;
        currentCapacity = newCapacity;
    }
};

template <class M>
std::ostream& operator<<(std::ostream& stream, const CircularBuffer<M>& buffer) {
    stream << "s:" << buffer.startPos << " e:" << buffer.endPos << " n:" << buffer.n << " cc:" << buffer.currentCapacity << " mc:" << buffer.capacity <<std::endl;
    for (size_t i = 0; i < buffer.capacity; ++i) {
        stream << buffer.array[i] << " ";
    }

    return stream;
}
#endif
