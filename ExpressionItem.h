#include <list>
#include <string>
#include <functional>
#include <cmath>
#include <variant>

// Operator class definition remains the same as before
class Operator {
public:
    std::string name;
    int precedence;
    std::function<int(int, int)> operation;
    std::function<int(int)> unaryOperation; // Function for unary operations


    Operator(const std::string& opName, int opPrecedence, std::function<int(int, int)> opFunction)
        : name(opName), precedence(opPrecedence), operation(opFunction) {}
    
    Operator(const std::string& opName, int opPrecedence) : name(opName) , precedence(opPrecedence) {}

       Operator(const std::string& opName, int opPrecedence, std::function<int(int)> unaryOpFunction)
           : name(opName), precedence(opPrecedence), unaryOperation(unaryOpFunction) {}

    std::string getName() const {
        return name;
    }

    int getPrecedence() const {
        return precedence;
    }
    
    int executeUnary(int a) const {
           if (unaryOperation) {
               return unaryOperation(a);
           }
           throw std::runtime_error("Unary operation not supported");
       }

    int execute(int a, int b) const {
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
        operators.emplace_back("/", 2, [](int a, int b) {
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
        operators.emplace_back("-", 3, [](int a) { return -a; });

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
    std::variant<int, Operator> item;

public:
    ExpressionItem(int num) : type(Type::Number), item(num) {}
    ExpressionItem(Operator oper) : type(Type::Operator), item(oper) {}

    bool isNumber() const {
        return type == Type::Number;
    }

    bool isOperator() const {
        return type == Type::Operator;
    }

    int getNumber() const {
        if (type == Type::Number) {
            return std::get<int>(item);
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
