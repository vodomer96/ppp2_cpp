//------------------------------------------------------------------------------

#ifndef VECTOR_EASY_HPP
#define VECTOR_EASY_HPP

//------------------------------------------------------------------------------

#include "../../../bstroustrup_code/std_lib_facilities.h"

//------------------------------------------------------------------------------

template<typename T, typename A>
struct Vector_base {
    A alloc;    // Распредилитель памяти
    T* elem;    // Начало выделенной памяти
    int sz;     // Количество элементов
    int space;  // Размер выделенной памяти

    Vector_base() : sz{0}, elem{nullptr}, space{0} {}

    Vector_base(const A& a, int n)
        : alloc{a}, elem{alloc.allocate(n)}, sz{n}, space{n} {}

    Vector_base(const A& a, int size, int newaloc)
        : alloc{a},
          elem{alloc.allocate(newaloc)},
          sz{size}, space{newaloc} {}

    ~Vector_base() { alloc.deallocate(elem, space); }
};

//------------------------------------------------------------------------------

template<typename T, typename A = allocator<T>> // Для всех типов T
class Vector_easy : private Vector_base<T, A>
{
public:
    Vector_easy() : Vector_base<T, A>() {}
    explicit Vector_easy(int s);
    Vector_easy(initializer_list<T> lst);

    // Копирующие конструктор и присваивание
    //Vector_easy(const Vector_easy& a);
    //Vector_easy& operator=(const Vector_easy& a);

    // Перемещающие конструктор и присваивание
    //Vector_easy(Vector_easy&& a);
    //Vector_easy& operator=(Vector_easy&& a);

    ~Vector_easy(); // Деструктор

    T& at(int n);                                 // Доступ с проверкой
    const T& at(int n) const;

    T& operator[](int n) { return this->elem[n]; }      // Доступ
    const T& operator[](int n) const { return this->elem[n]; }

    int size() const { return this->sz; }               // Текущий размер
    int capacity() const { return this->space; }
                                                  // Увеличение
    void resize(int newsize, T def = T{});
    void push_back(const T& d);
    void reserve(int newalloc);
    void reserve1(int newalloc);
};

//------------------------------------------------------------------------------

template<typename T, typename A>
Vector_easy<T, A>::Vector_easy(int s) : Vector_base<T, A>(this->alloc, s)
{    
    for (int i=0; i < s; ++i)
        this->alloc.construct(this->elem[i], T{});
}

//------------------------------------------------------------------------------

template<typename T, typename A>
Vector_easy<T, A>::Vector_easy(initializer_list<T> lst)
    : Vector_base<T, A>(this->alloc, static_cast<int>(lst.size()))
{
    // Инициализация элементов
    uninitialized_copy(lst.begin(), lst.end(), this->elem);
}

//------------------------------------------------------------------------------

template<typename T, typename A>
Vector_easy<T, A>::~Vector_easy()
{
    for (int i=0; i < this->sz; ++i)            // Уничтожаем созданные
        this->alloc.destroy(&this->elem[i]);
}

//------------------------------------------------------------------------------

template<typename T, typename A>
T& Vector_easy<T, A>::at(int n)
{
    if (n<0 || this->sz<=n) throw Range_error(n);
    return operator[](n);
}

//------------------------------------------------------------------------------

template<typename T, typename A>
const T& Vector_easy<T, A>::at(int n) const
{
    if (n<0 || this->sz<=n) throw Range_error(n);
    return operator[](n);
}

//------------------------------------------------------------------------------

template<typename T, typename A>
void Vector_easy<T, A>::reserve(int newalloc)
{
    if (newalloc <= this->space) return;    // Размер не уменьшается
    Vector_base<T, A>
            b(this->alloc, this->sz, newalloc);       // Выделение новой памяти
    // Копируем старые элементы:
    uninitialized_copy(this->elem, &this->elem[this->sz], b.elem);
    // Уничтожаем:
    for (int i=0; i < this->sz; ++i)
        this->alloc.destroy(&this->elem[i]);
    std::swap<Vector_base<T, A>>(*this, b); // Обмен представлений
}

//------------------------------------------------------------------------------

template<typename T, typename A>
void Vector_easy<T, A>::reserve1(int newalloc)
{
    if (newalloc <= this->space) return;    // Размер не уменьшается
    unique_ptr<T> p {this->alloc.allocate(newalloc)};
    // Копируем старые элементы:
    for (int i=0; i < this->sz; ++i)
        this->alloc.construct(&p[i], &this->elem[i]);
    // Уничтожаем:
    for (int i=0; i < this->sz; ++i)
        this->alloc.destroy(&this->elem[i]);

    this->alloc.deallocate(this->elem, this->space);
    this->elem = p.release();
    this->space = newalloc;
}

//------------------------------------------------------------------------------

template<typename T, typename A>
void Vector_easy<T, A>::resize(int newsize, T def)
// Создаем вектор, содержащий newsize элементов
// Инициализируем каждый элемент значением def
// Размер никогда не уменьшается
{
    reserve(newsize);
    // Создаем
    for (int i=newsize; i < this->sz; ++i)
        this->alloc.construct(&this->elem[i], def);
        //uninitialized_copy(def, def + 1, &this->elem[i]);
    // Уничтожаем
    for (int i=newsize; i < this->sz; ++i)
        this->alloc.destroy(&this->elem[i]);
    this->sz = newsize;
}

//------------------------------------------------------------------------------

template<typename T, typename A>
void Vector_easy<T, A>::push_back(const T& val)
// Увеличивает размер вектора на единицу;
// инициализирует новый элемент значением d
{
    if (this->space == 0) reserve(8);     // Начинаем с 8 элементов
    else if (this->sz == this->space)
        reserve(2*this->space);           // Выделяем больше памяти
    this->alloc.construct(&this->elem[this->sz], val); // Добавляем в конец значение val
    //uninitialized_copy(val, val+1, &this->elem[this->sz]);
    ++this->sz;                           // Увеличиваем размер
}

//------------------------------------------------------------------------------

template<typename C> void printout(const C& c)
{
    for (size_t i{0}; i < c.size(); ++i) cout << c[i] << '\n';
}

//------------------------------------------------------------------------------

template<typename T, size_t N>
void fill(array<T, N>& b, const T& val)
{
    for (size_t i{0}; i < N; ++i) b[i] = val;
}

//------------------------------------------------------------------------------

#endif // VECTOR_EASY_HPP

//------------------------------------------------------------------------------
