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
                return false;  // Unbalanced parentheses
            }
            parentheses.pop();
        }
    }

    return parentheses.empty();  // If stack is empty, parentheses are balanced
}

int getPrecedence(const Operator& op) {
    return op.precedence;
}

class MathExpression {
private:
    std::vector<ExpressionItem> expressionList;
    std::vector<ExpressionItem> postfixExpressionList; 
    
public:
    MathExpression(const std::string& expression) {
       
    }
    
    int getLength() {
    	return postfixExpressionList.size();
    }
    
    std::vector<ExpressionItem> getList() {
    	return postfixExpressionList;
    }
    
    


    int getPrecedence(const OperatorList& opList, char op) {
        std::string opStr(1, op);
        Operator oper = opList.getOperatorByName(opStr);
        return oper.getPrecedence();
    }
    
    std::string infixToPostfix(const OperatorList& opList, const std::string& expression) {
        std::stack<char> stack;
        std::string postfix;

        std::unordered_map<char, int> precedenceMap;
        for (const Operator& op : opList.getOperators()) {
            precedenceMap[op.name[0]] = op.getPrecedence();
        }

        for (size_t i = 0; i < expression.length(); ++i) {
            char c = expression[i];

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
                    stack.pop(); // Discard the '('
                }
            } else if (isOperator(c)) {
                if ((c == '+' || c == '-') && (i == 0 || expression[i - 1] == '(')) {
                    // Handle unary operators at the beginning of the expression or after '('
                    stack.push((c == '+') ? '#' : '$');
                } else {
                    while (!stack.empty() && stack.top() != '(' && precedenceMap[c] <= precedenceMap[stack.top()]) {
                        postfix += stack.top();
                        postfix += ' ';
                        stack.pop();
                    }
                    stack.push(c);
                }
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
    int evaluateExpression(const OperatorList& opList, const std::string& postfixExpression) {
        std::stack<int> evaluationStack;

        std::cout << "Postfix Expression: " << postfixExpression << std::endl;

        size_t i = 0;
        while (i < postfixExpression.length()) {
            char c = postfixExpression[i];

            std::cout << "Processing character at index " << i << ": " << c << std::endl;

            if (c == ' ') {
                ++i;
                continue;
            }

            if (std::isdigit(c)) {
                int operand = 0;
                while (std::isdigit(c)) {
                    operand = operand * 10 + (c - '0');
                    c = postfixExpression[++i];
                }
                evaluationStack.push(operand);
                std::cout << "Pushed operand: " << operand << " onto the stack" << std::endl;
                continue;
            }

            if (isOperator(c)) {
                if (evaluationStack.size() < 2) {
                    throw std::logic_error("Invalid Expression for Evaluation (insufficient operands for binary operator)");
                }

                int operand2 = evaluationStack.top();
                evaluationStack.pop();
                int operand1 = evaluationStack.top();
                evaluationStack.pop();

                int result;
                if (c == '*') {
                    result = operand1 * operand2;
                    std::cout << "Applied operator: " << c << " on operands: " << operand1 << " and " << operand2 << std::endl;
                } else if (c == '/') {
                    if (operand2 == 0) {
                        throw std::logic_error("Division by zero error");
                    }
                    result = operand1 / operand2;
                    std::cout << "Applied operator: " << c << " on operands: " << operand1 << " and " << operand2 << std::endl;
                } else {
                    throw std::logic_error("Invalid Operator");
                }

                evaluationStack.push(result);
                std::cout << "Pushed result: " << result << " onto the stack" << std::endl;
                ++i;
                continue;
            }

            throw std::logic_error("Invalid Expression for Evaluation (unexpected character: " + std::string(1, c) + ")");
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
        std::string infixExpression =  "  10 * 2 / 5";
        MathExpression mathExpr = MathExpression(infixExpression);

        
        std::string postfixExpression = mathExpr.infixToPostfix(operators, infixExpression);

        try {
            int result = mathExpr.evaluateExpression( operators, postfixExpression);
            std::cout << "Result: " << result << std::endl;
        } catch (std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }

        return 0;
    }
