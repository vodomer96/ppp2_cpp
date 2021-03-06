#include <Graph.h>

//-----------------------------------------------------------------------------

struct B {
    virtual void f() const { cout << "B::f ";}
    void g()         const { cout << "B::g ";}
};

//-----------------------------------------------------------------------------

struct D : B {
    void f() const override
    { cout << "D::f ";} // Перекрывает B::f
    void g()       { cout << "D::g ";}
};

//-----------------------------------------------------------------------------

struct DD : D {
    void f()       { cout << "DD::f ";} // Не перекрывает D::f (нет const)
    void g() const { cout << "DD::g ";}
};

//-----------------------------------------------------------------------------

void call(const B& b)
{
    b.f();
    b.g();
}

//-----------------------------------------------------------------------------

void example_book()
{
    B b;
    D d;
    DD dd;

    call(b);
    call(d);
    call(dd);

    b.f();
    b.g();

    d.f();
    d.g();

    dd.f();
    dd.g();
}

//-----------------------------------------------------------------------------

struct B1 {
    virtual void vf() const { cout << "B1::vf ";}
    void f()          const { cout << "B1::f ";}
    virtual void pvf() = 0;
};

//-----------------------------------------------------------------------------

struct D1 : B1 {
    void vf() const override
    { cout << "D1::vf ";}
    void f()  const { cout << "D1::f ";}
    void pvf() override
    { cout << "D1::pvf ";}
};

//-----------------------------------------------------------------------------

struct D2 : D1 {
    void pvf() override
    { cout << "D2::pvf ";}
};

//-----------------------------------------------------------------------------

struct B2 {
    virtual void pvf() const = 0;
};

//-----------------------------------------------------------------------------

struct D21 : B2 {
    void pvf() const override
    { cout << str << '\n'; }
    string str = "hi";
};

//-----------------------------------------------------------------------------

struct D22 : B2 {
    void pvf() const override
    { cout << num << '\n'; }
    int num = 8;
};

//-----------------------------------------------------------------------------

void f(const B2& b2)
{
    b2.pvf();
}

//-----------------------------------------------------------------------------

void task1()
{
    //B1 b1;
    //b1.vf();
    //b1.f();


}

//-----------------------------------------------------------------------------

void task2()
{
    D1 d1;
    d1.vf();
    d1.f();
}

//-----------------------------------------------------------------------------

void task3()
{
    D1 d1;
    B1& b1{d1};

    b1.vf();
    b1.f();
}

//-----------------------------------------------------------------------------

void task6()
{
    D2 d2;
    d2.vf();
    d2.f();
    d2.pvf();
}

//-----------------------------------------------------------------------------

void task7()
{
    D21 d21;
    D22 d22;
    f(d21);
    f(d22);
}

//-----------------------------------------------------------------------------

int main()
{
    cout << "-----------------------------"
         << "example_book()\n";
    example_book();
    cout << "\n-----------------------------"
         << "task1()\n";
    task1();
    cout << "\n-----------------------------"
         << "task2()\n";
    task2();
    cout << "\n-----------------------------"
         << "task3()\n";
    task3();
    cout << "\n-----------------------------"
         << "task6()\n";
    task6();
    cout << "\n-----------------------------"
         << "task7()\n";
    task7();

    return 0;
}

//-----------------------------------------------------------------------------
