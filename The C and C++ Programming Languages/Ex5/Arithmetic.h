#ifndef  ARITHMETIC_H
#define  ARITHMETIC_H
#include "Expression.h"
#include <string>
using namespace std;

//declaring the value class
class Value: public Expression {
    // declaring the needed variables which is val that holds the double numbers for the expressions
    private:
        double val;

    // declaring the extended methods and the constructor of value
    public:
        Value(double);
        double eval() const;
        bool equals(const Expression&);
        string format();
};


//declaring the Compund class
class Compund: public Expression {

    // declaring the needed variables of compound which will be extended from from the two classes
    // Addition and Subtract
    // These are two pointer from the type Expression I think that there is need for destructor but when I
    // Tried to add it i got 2 warnings
    protected:
        Expression* exOne;
        Expression* exTwo;
};

//declaring the Compund class
class Addition: public Compund {
    // declaring the extended methods and the constructor of Addition
    public:
        Addition(Expression*, Expression*);
        double eval() const;
        bool equals(const Expression&);
        string format();
};

class Subtract: public Compund {
    // declaring the extended methods and the constructor of Subtract
    public:
        Subtract(Expression*, Expression*);
        double eval() const;
        bool equals(const Expression&);
        string format();
};

#endif
