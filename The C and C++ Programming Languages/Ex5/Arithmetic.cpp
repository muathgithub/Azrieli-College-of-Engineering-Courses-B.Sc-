#include <iostream>
#include <string>
#include "Arithmetic.h"
using namespace std;

// initializing the constructor of the class Value that puts the received value in the private variable this->val
Value::Value(double val){
    this->val = val;
}

// initializing the method eval() for the class Value which will return the value of the private variable of the class(val)
double Value::eval() const{
    return this->val;
}

// initializing the method equals() that receives another expression and it compares the value of this with the value of other
// if the values are the same it returns true else false
bool Value::equals(const Expression& expOther){
    if(this->eval() == expOther.eval()){
        return true;
    }

    return false;
}

// initializing the method format() of the class Value that returns the value (double) as a string using the method to_string()
string Value::format(){
    return to_string(this->val);
}


// initializing the constructor of the class Addition which receives two pointers for two Expression
// and it puts there values in the two private extended variables
Addition::Addition(Expression* exOne, Expression* exTwo){
    this->exOne = exOne;
    this->exTwo = exTwo;
}

// initializing the method eval() for the class Addition which will return the sum of the two values of its two exprestions
double Addition::eval() const{
    return exOne->eval() + exTwo->eval();
}

// initializing the method format() of the class Addition that returns the Addition expression in a formated way with the () and +  as a string
string Addition::format(){
    string result = "(" + exOne->format() + " + " + exTwo->format() + ")";
    return result;
}

// initializing the method equals() that receives another expression and it compares the value of this with the value of other
// if the values are the same it returns true else false
bool Addition::equals(const Expression& expOther){
    if(this->eval() == expOther.eval()){
        return true;
    }
    return false;
}

// initializing the constructor of the class Subtract which receives two pointers for two Expression
// and it puts there values in the two private extended variables
Subtract::Subtract(Expression* exOne, Expression* exTwo){
    this->exOne = exOne;
    this->exTwo = exTwo;
}

// initializing the method eval() for the class Addition which will return the sum of the two values of its two exprestions
double Subtract::eval() const{
    return exOne->eval() - exTwo->eval();
}

// initializing the method format() of the class Addition that returns the Addition expression in a formated way with the () and +  as a string
string Subtract::format(){
    string result = "(" + exOne->format() + " - " + exTwo->format() + ")";
    return result;
}

// initializing the method equals() that receives another expression and it compares the value of this with the value of other
// if the values are the same it returns true else false
bool Subtract::equals(const Expression& expOther){
    if(this->eval() == expOther.eval()){
        return true;
    }
    return false;
}
