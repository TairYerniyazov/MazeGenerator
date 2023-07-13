// ZBIORY
//----------------------------------------------------------------------
// Autor: Tair Yerniyazov,                            Krakow, 29.05.2023
//----------------------------------------------------------------------

#ifndef SETS_H
#define SETS_H

// KONTENER
class Container {
protected:
    Container() {};
public:
    virtual int Count() const = 0;
    virtual bool IsEmpty() const {return Count() == 0;};
    virtual bool IsFull() const = 0;
    virtual void MakeNull() = 0;
};

// ZBIÓR UNIWERSALNY (UNIWERSUM)
template <typename T>
class Set : public virtual Container {
protected:
    int count = 0;
    int universeSize = 0;
public:
    Set() {}
    Set(int n) : universeSize{n} {}
    int UniverseSize() const {return universeSize;} 
    int Count() const {return count;}
    virtual void Insert(T element) = 0;
    virtual bool IsMember(T element) const = 0; 
    virtual void Withdraw(T element) = 0;
};

// ZBIÓR KONKRETNY (WEKTOR LOGICZNY)
class SetAsArray : public Set<int> {
    std::vector<bool> array;
public:
    SetAsArray(int n) {
        array.reserve(n);
        for (int i = 0; i < n; i++)
            array[i] = false;
        universeSize = n;
    }

    // Dodatkowy konstruktor potrzebny do łączenia zbiorów i alokacji wyniku
    SetAsArray(SetAsArray const& a, SetAsArray const& b) {
        array.reserve(a.universeSize);
        universeSize = a.universeSize;
        if (a.universeSize == b.universeSize) {
            for (int i = 0; i < a.universeSize; i++) {
                array[i] = a.array[i] || b.array[i];
            }
        }
        for (int i = 0; i < universeSize; i++)
            if (array[i])
                count++;
    }

    void MakeNull() {
        array.clear();
    }

    bool IsFull() const {
        return Count() == UniverseSize();
    };

    void Insert (int element) {
        if (!array[element] && element < universeSize) {
            array[element] = true;
            count++;
        }
    }

    bool IsMember (int element) const {
        return array[element];
    } 

    void Withdraw (int element) {
        if (array[element]) {
            array[element] = false;
            count--;
        }
    }
    
    friend SetAsArray operator+(SetAsArray const& a, SetAsArray const& b) {
        SetAsArray result(a.universeSize);
        if (a.universeSize == b.universeSize) {
            for (int i = 0; i < a.universeSize; i++) {
                result.array[i] = a.array[i] || b.array[i];
                if (result.array[i])
                    result.count++;
            }
        }
        return result;
    }
    
    friend SetAsArray operator-(SetAsArray const& a, SetAsArray const& b) {
        SetAsArray result(a.universeSize);
        if (a.universeSize == b.universeSize) {
            for (int i = 0; i < a.universeSize; i++) {
                result.array[i] = a.array[i] && !b.array[i];
                if (result.array[i])
                    result.count++;
            }
        }
        return result;
    }
    
    friend SetAsArray operator*(SetAsArray const& a, SetAsArray const& b) {
        SetAsArray result(a.universeSize);
        if (a.universeSize == b.universeSize) {
            for (int i = 0; i < a.universeSize; i++) {
                result.array[i] = a.array[i] && b.array[i];
                if (result.array[i])
                    result.count++;
            }
        }
        return result;
    }
    
    friend bool operator==(SetAsArray const& a, SetAsArray const& b) {
        if (a.universeSize == b.universeSize) {
            for (int i = 0; i < a.universeSize; i++)
                if (a.array[i] != b.array[i])
                    return false;
        }
        return true;
    }
    
    friend bool operator<=(SetAsArray const& a, SetAsArray const& b) {
        if (a.universeSize == b.universeSize) {
            for (int i = 0; i < a.universeSize; i++)
                if (a.array[i] && !b.array[i])
                    return false;
        }
        return true;
    }
    
    void Print() {
        for (int i = 0; i < universeSize; i++) {
            if (array[i])
                std::cout << i << " | ";
        }
        std::cout << "\n";
    }
};

#endif