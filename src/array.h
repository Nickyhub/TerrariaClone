#pragma once

template <typename T, size_t Size>
class Array
{
public:
    Array()
    {
        currentElem = 0;
    };

    void add(T elem)
    {
        if (currentElem >= Size)
        {
            std::cout << "Array is full." << std::endl;
            return;
        }
        data[currentElem++] = elem;
    }

    T &operator[](size_t index)
    {
        if (index > currentElem)
        {
            std::cout << "Index out of range.";
            return data[0];
        }
        return data[index];
    }

    const T &operator[](size_t index) const
    {
        if (index > currentElem)
        {
            std::cout << "Index out of range.";
            return data[0];
        }
        return data[index];
    }

    size_t size() const
    {
        return currentElem;
    }

    void reset()
    {
        currentElem = 0;
    }

    ~Array() {}

private:
    T data[Size];
    size_t currentElem;
};