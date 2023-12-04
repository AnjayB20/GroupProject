#include <list>
#include <string>
#include <functional>
#include <cmath>
#include <variant>

// Operator class definition remains the same as before
class Operator {
public:
    std::string name;
    double precedence;
    std::function<double(double, double)> operation;
    
    Operator(const std::string& opName, double opPrecedence, std::function<double(double, double)> opFunction)
        : name(opName), precedence(opPrecedence), operation(opFunction) {}
    
    Operator(const std::string& opName, double opPrecedence) : name(opName) , precedence(opPrecedence) {}

   
    std::string getName() const {
        return name;
    }

    double getPrecedence() const {
        return precedence;
    }
    
  
    double execute(double a, double b) const {
        return operation(a, b);
    }
};

class OperatorList {
public:
    std::vector<Operator> operators;

    OperatorList() {
        operators.emplace_back("+", 1, [](double a, double b) { return a + b; });
        operators.emplace_back("-", 1, [](double a, double b) { return a - b; });
        operators.emplace_back("*", 2, [](double a, double b) { return a * b; });
        operators.emplace_back("/", 2, [](double a, double b) {
                   if (b != 0) {
                       return a / b;
                   } else {
                       throw std::runtime_error("Division by zero");
                   }
               });
        operators.emplace_back("%", 2, [](double a, double b) { return fmod(a, b); });
        operators.emplace_back("^", 3, [](double a, double b) { return pow(a, b); });
        operators.emplace_back("(", 4 );
        operators.emplace_back(")", 4 );
       

        // Add more operators as needed
    }

    const std::vector<Operator>& getOperators() const {
        return operators;
    }

    Operator getOperatorByName(std::string opName) const {
        for (const Operator& op : operators) {
            if (op.name == opName) {
                return op;
            }
        }
        throw std::invalid_argument("Invalid Operator");
    }
};

class ExpressionItem {
public:
    enum class Type { Number, Operator };

private:
    Type type;
    std::variant<double, Operator> item;

public:
    ExpressionItem(double num) : type(Type::Number), item(num) {}
    ExpressionItem(Operator oper) : type(Type::Operator), item(oper) {}

    bool isNumber() const {
        return type == Type::Number;
    }

    bool isOperator() const {
        return type == Type::Operator;
    }

    double getNumber() const {
        if (type == Type::Number) {
            return std::get<double>(item);
        }
        throw std::logic_error("Not a number");
    }

    Operator getOperator() const {
        if (type == Type::Operator) {
            return std::get<Operator>(item);
        }
        throw std::logic_error("Not an operator");
    }
};
