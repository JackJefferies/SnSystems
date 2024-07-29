#include <iostream>
#include <string>
#include <stack>

// Declare the functions before they are used
int precedence(char op);
bool evaluateTop(std::stack<int>& values, std::stack<char>& ops);


// Function to evaluate a simple arithmetic expression
bool evaluate(const char* expression, int& result) {
    std::stack<int> values;    // Stack to hold numbers
    std::stack<char> ops;      // Stack to hold operators
    int nStringLength = strlen(expression);
    int i = 0;

    while (i < nStringLength) {
        // Ignore whitespace
        if (isspace(expression[i])) {
            i++;
            continue;
        }

        // Handle numbers & negatives
        if (isdigit(expression[i]) || (expression[i] == '-' && (i == 0 || expression[i - 1] == '('))) {
            int sign = 1;
            if (expression[i] == '-') {
                sign = -1;
                i++;
            }

            int val = 0;
            while (i < nStringLength && isdigit(expression[i])) {
                // subtracting '0' is a quick trick for converting ASCII to int.
                val = val * 10 + (expression[i] - '0');
                i++;
            }
            values.push(sign * val);
            continue;
        }

        // Handle opening parenthesis
        if (expression[i] == '(') {
            ops.push(expression[i]);
            i++;
            continue;
        }

        // Handle closing parenthesis
        if (expression[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                if (!evaluateTop(values, ops)) {
                    return false;
                }
            }
            if (ops.empty() || ops.top() != '(') {
                return false; // Unmatched parenthesis
            }
            ops.pop(); // Pop '('
            i++;
            continue;
        }

        // Handle operators +, -, *, /
        if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                if (!evaluateTop(values, ops)) {
                    return false;
                }
            }
            ops.push(expression[i]);
            i++;
            continue;
        }

        // Invalid character
        return false;
    }

    // Evaluate remaining operations in stack
    while (!ops.empty()) {
        if (!evaluateTop(values, ops)) {
            return false;
        }
    }

    if (values.size() != 1) {
        return false; // Invalid expression
    }

    result = values.top();
    return true;
}

// Helper function to determine precedence of operators
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Helper function to evaluate the top of the stacks
bool evaluateTop(std::stack<int>& values, std::stack<char>& ops) {
    if (values.size() < 2 || ops.empty()) {
        return false; // Error: not enough values or operators
    }
    int right = values.top(); values.pop();
    int left = values.top(); values.pop();
    char op = ops.top(); ops.pop();

    int result;
    switch (op) {
    case '+': result = left + right; break;
    case '-': result = left - right; break;
    case '*': result = left * right; break;
    case '/':
        if (right == 0) return false; // Division by zero error
        result = left / right;
        break;
    default: return false; // Invalid operator
    }

    values.push(result);
    return true;
}


// Define a few test cases & call the evaluate() function
int main() {
    const char* expressions[] = {
        "1 + 3",
        "(1 + 3) * 2",
        "(4 / 2) + 6",
        "4 + (12 / (1 * 2))",
        "(1 + (12 * 2)",
        "-1 + 3",
        "-4 * (-1 - 4) + 3"
    };
    int results[7];
    bool evaluations[7];

    for (int i = 0; i < 7; ++i) {
        evaluations[i] = evaluate(expressions[i], results[i]);
        if (evaluations[i]) {
            std::cout << "Expression: " << expressions[i] << "\nResult: " << results[i] << "\nReturn code: true\n\n";
        }
        else {
            std::cout << "Expression: " << expressions[i] << "\nResult: N/A\nReturn code: false\n\n";
        }
    }

    return 0;
}
