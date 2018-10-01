
#ifndef LIJST_H
#define LIJST_H

#include <memory>
#include <vector>

template <class T>
class Knoop;

template <class T>
class Lijst
{
public:
    Lijst() = default;
    Lijst(const T& t);
    Lijst(const Lijst<T>& andere); // Dit moeten we zelf in elkaar steken
    Lijst(Lijst<T>&& andere) = default;
    Lijst<T>& operator=(const Lijst<T>& andere); // Dit moeten we zelf in elkaar steken
    Lijst<T>& operator=(Lijst<T>&& andere) = default;
    virtual ~Lijst() = default;

    void append(Lijst<T>&& andere);
    std::vector<T> get_all_elements() const;
    bool empty() const;
    int size() const;

private:
    struct Knoop
    {
        Knoop(const T& sl);

        T sleutel;
        std::unique_ptr<Knoop> volgende = nullptr;
    };

    std::unique_ptr<Knoop> eerste = nullptr;
};

////////////////////////////////////////////////////////////////////////////////

template <class T>
Lijst<T>::Lijst(const T& t) : eerste{std::make_unique<Knoop>(t)}
{
}

template <class T>
Lijst<T>::Lijst(const Lijst<T>& andere)
{
    eerste = nullptr;

    const std::unique_ptr<Knoop>* iter_ander = &(andere.eerste);
    std::unique_ptr<Knoop>* iter_eigen = &eerste;

    while (*iter_ander)
    {
        (*iter_eigen) = std::make_unique<Knoop>((*iter_ander)->sleutel);

        iter_ander = &((*iter_ander)->volgende);
        iter_eigen = &((*iter_eigen)->volgende);
    }
}

template <class T>
Lijst<T>& Lijst<T>::operator=(const Lijst<T>& andere)
{
    // Copy-swap trick
    Lijst<T> temp{andere};
    std::swap((*this), temp);
    return (*this);
}

template <class T>
void Lijst<T>::append(Lijst<T>&& andere)
{
    std::unique_ptr<Knoop>* iter_eigen = &eerste;

    while (*iter_eigen)
    {
        iter_eigen = &((*iter_eigen)->volgende);
    }

    (*iter_eigen) = std::move(andere.eerste);
}

template <class T>
std::vector<T> Lijst<T>::get_all_elements() const
{
    std::vector<int> all_elements;
    const std::unique_ptr<Knoop>* iter_eigen = &eerste;

    while (*iter_eigen)
    {
        all_elements.push_back((*iter_eigen)->sleutel);
        iter_eigen = &((*iter_eigen)->volgende);
    }

    return all_elements;
}

template <class T>
bool Lijst<T>::empty() const
{
    return (eerste == nullptr);
}

template <class T>
int Lijst<T>::size() const
{
    int aantal_knopen = 0;

    const std::unique_ptr<Knoop>* iter_eigen = &eerste;

    while (*iter_eigen)
    {
        aantal_knopen++;

        iter_eigen = &((*iter_eigen)->volgende);
    }

    return aantal_knopen;
}

template <class T>
Lijst<T>::Knoop::Knoop(const T& sleutel) : sleutel(sleutel)
{
}

#endif
