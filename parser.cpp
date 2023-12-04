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
                    stack.pop(); // Discard the '('
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

        // Trim any extra space at the end
        if (!postfix.empty() && postfix.back() == ' ') {
            postfix.pop_back();
        }

        return postfix;
    }


    int evaluateExpression(const OperatorList& opList, const std::string& postfixExpression) {
        std::stack<int> evaluationStack;
        
        std::cout << postfixExpression << std::endl;

        for (char c : postfixExpression) {
            if (std::isdigit(c)) {
                evaluationStack.push(c - '0');
            } else if (isOperator(c)) {
                int operand2 = evaluationStack.top();
                evaluationStack.pop();

                int operand1 = evaluationStack.top();
                evaluationStack.pop();

                std::string opStr(1, c);
                Operator oper = opList.getOperatorByName(opStr);
                int result = oper.execute(operand1, operand2);
                evaluationStack.push(result);
            }
            
            std::cout << "Evaluation Stack: ";
                   std::stack<int> temp = evaluationStack;
                   while (!temp.empty()) {
                       std::cout << temp.top() << " ";
                       temp.pop();
                   }
                   std::cout << std::endl;
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
        std::string infixExpression =  "(((2 + 3))) + (((1 + 2)))";
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