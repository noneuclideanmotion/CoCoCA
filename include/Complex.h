#pragma once
#include <cmath>
#include <iostream>

class Complex
{
public:
    //Constructors
    Complex() : m_real(0), m_imaginary(0) {}
    Complex(double r, double i) : m_real(r), m_imaginary(i) {}

    //exp function
    static Complex exp(Complex c)
    {
        return (Complex(cos(c.m_imaginary), sin(c.m_imaginary)) * std::exp(c.m_real));
    }

    //Get Real/Imaginary/Abs/Angle
    double real() const
    {
        return m_real;
    }
    double imaginary() const
    {
        return m_imaginary;
    }
    double abs() const
    {
        return sqrt(m_real * m_real + m_imaginary * m_imaginary);
    }
    double angle() const
    {
        if (atan2(m_imaginary, m_real) < 0)
        {
            return atan2(m_imaginary, m_real) + 2*3.1416;
        }
        return atan2(m_imaginary, m_real);
    }

    //Conjugate
    Complex conjugate() const
    {
        return Complex(m_real, -m_imaginary);
    }

    //operators
    Complex operator+(const Complex& b)
    {
        return Complex(m_real + b.m_real, m_imaginary + b.m_imaginary);
    }
    Complex operator-(const Complex& b)
    {
        return Complex(m_real - b.m_real, m_imaginary - b.m_imaginary);
    }
    Complex operator*(const Complex& b)
    {
        return Complex(m_real * b.m_real - m_imaginary * b.m_imaginary, m_imaginary * b.m_real + b.m_imaginary * m_real);
    }
    Complex operator*(const double s)
    {
        return Complex(m_real * s, m_imaginary * s);
    }
    Complex operator/(const Complex& b)
    {
        double absp2 = b.abs();
        absp2 *= absp2;
        return (Complex(m_real, m_imaginary) * b.conjugate()) * (1 / absp2);
    }
    Complex& operator=(const Complex& b)
    {
        m_real = b.m_real;
        m_imaginary = b.m_imaginary;
        return *this;
    }

    //Friend, to print
    friend std::ostream& operator<<(std::ostream& out, const Complex& c);

    void set_real(double r)
    {
        m_real = r;
    }

    void set_imaginary(double i)
    {
        m_imaginary = i;
    }

protected:
    //Cartesian
    double m_real;
    double m_imaginary;
};

inline std::ostream& operator<<(std::ostream& out, const Complex& c)
{
    if (c.m_imaginary == 1)
    {
        out << c.m_real << " + i";
    }
    else if (c.m_imaginary == -1)
    {
        out << c.m_real << " - i";
    }
    else if (c.m_imaginary < 0)
    {
        out << c.m_real << " - " << -c.m_imaginary << "i";
    }
    else
    {
        out << c.m_real << " + " << c.m_imaginary << "i";
    }
    return out;
}
