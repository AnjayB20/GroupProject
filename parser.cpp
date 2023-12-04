#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "ExpressionItem.h"
#include <variant>
#include <stack>


class InvalidExpressionException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Invalid character found in the expression.";
    }
};


bool isValidCharacter(char ch) {
    return std::isdigit(ch) || std::isspace(ch) || ch == '+' ||
    		ch == '-' || ch == '*' || ch == '/' || ch == '(' ||
			ch == ')' || ch == '%' || ch== '^';
}

bool isOperator( char token) {
	return token == '+' || token == '-' || token == '*' || token == '%' || token == '^' || token == '/';    }

bool isParenthesis(char token) {
	return token == '('  || token == ')';
    }

bool isDigit(char c) {
    return (c >= '0' && c <= '9');
}

bool areParenthesesBalanced(const std::string& expr) {
    std::stack<char> parentheses;

    for (char c : expr) {
        if (c == '(') {
            parentheses.push(c);
        } else if (c == ')') {
            if (parentheses.empty() || parentheses.top() != '(') {
                return false; 
            }
            parentheses.pop();
        }
    }

    return parentheses.empty(); 
}

double getPrecedence(const Operator& op) {
    return op.precedence;
}

class MathExpression {
private:
    std::vector<ExpressionItem> expressionList;
    std::vector<ExpressionItem> postfixExpressionList; 
    
public:
    MathExpression(const std::string& expression) {
       
    }
    
    double getLength() {
    	return postfixExpressionList.size();
    }
    
    std::vector<ExpressionItem> getList() {
    	return postfixExpressionList;
    }
    
    


    double getPrecedence(const OperatorList& opList, char op) {
        std::string opStr(1, op);
        Operator oper = opList.getOperatorByName(opStr);
        return oper.getPrecedence();
    }

    std::string infixToPostfix(const OperatorList& opList, const std::string& expression) {
        std::stack<char> stack;
        std::string postfix;

        std::unordered_map<char, double> precedenceMap;
        for (const Operator& op : opList.getOperators()) {
            precedenceMap[op.name[0]] = op.getPrecedence();
        }

        for (char c : expression) {
            if (std::isdigit(c)) {
                postfix += c;
                postfix += ' ';
            } else if (c == '(') {
                stack.push(c);
            } else if (c == ')') {
                while (!stack.empty() && stack.top() != '(') {
                    postfix += stack.top();
                    postfix += ' ';
                    stack.pop();
                }
                if (!stack.empty() && stack.top() == '(') {
                    stack.pop();
                }
            } else if (isOperator(c)) {
                while (!stack.empty() && stack.top() != '(' && precedenceMap[c] <= precedenceMap[stack.top()]) {
                    postfix += stack.top();
                    postfix += ' ';
                    stack.pop();
                }
                stack.push(c);
            }
        }

        while (!stack.empty()) {
            postfix += stack.top();
            postfix += ' ';
            stack.pop();
        }


        if (!postfix.empty() && postfix.back() == ' ') {
            postfix.pop_back();
        }

        return postfix;
    }


    double evaluateExpression(const OperatorList& opList, const std::string& postfixExpression) {
        std::stack<double> evaluationStack;

        for (char c : postfixExpression) {
            if (std::isdigit(c)) {
                evaluationStack.push(c - '0');
            } else if (isOperator(c)) {
                double operand2 = evaluationStack.top();
                evaluationStack.pop();

                double operand1 = evaluationStack.top();
                evaluationStack.pop();

                std::string opStr(1, c);
                Operator oper = opList.getOperatorByName(opStr);
                double result = oper.execute(operand1, operand2);
                evaluationStack.push(result);
            }
            
        
               }
        

        if (evaluationStack.size() == 1) {
            return evaluationStack.top();
        } else {
            throw std::logic_error("Invalid Expression for Evaluation (more than one item on the stack)");
        }
    }

};

    int main() {
        OperatorList operators;
        
        std::cout << "Enter a Mathematical Expression: " << std::endl;
        std::string infixExpression;
        std::getline(std::cin, infixExpression);
        MathExpression mathExpr = MathExpression(infixExpression);

        
        std::string postfixExpression = mathExpr.infixToPostfix(operators, infixExpression);

        try {
            double result = mathExpr.evaluateExpression( operators, postfixExpression);
            std::cout << "Result: " << result << std::endl;
        } catch (std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }

        return 0;
    }