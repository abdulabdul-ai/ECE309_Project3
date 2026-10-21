// src/tools/calculator_eval.cpp — YOU IMPLEMENT (spec §4.6).
//
// calc_evaluate: evaluate a well-formed token list with two stacks
// (one for numbers, one for operators and '('). See spec §4.6 and the
// worked trace in Appendix C. std::stack or std::vector are both fine.

#include "tools/calculator.h"
#include <stack>
#include <vector>
#include <cmath>
#include <utility>
#include <string>


bool calc_evaluate(const std::vector<CalcToken>& tokens, double* out,
                   std::string* error) {

    std::stack<double> values;
    std::stack<CalcToken::Kind> ops;

    for(const CalcToken &t : tokens){
        if (t.kind == CalcToken::Kind::Number){
            values.push(t.value);
        }
        else if (t.kind == CalcToken::Kind::LParen){
            ops.push(t.kind);
        }
        else if (t.kind == CalcToken::Kind::RParen){
            while (!ops.empty() && ops.top() != CalcToken::Kind::LParen){
                const CalcToken::Kind op = ops.top();
                ops.pop();
                if (values.size() < 2) {
                    *error = "error";
                    return false;
                }
                double answer = 0.0;
                const double rightValue = values.top();   
                values.pop();
                const double leftValue = values.top();   
                values.pop();
                if (op == CalcToken::Kind::Plus) {
                    answer = leftValue + rightValue;
                } else if (op == CalcToken::Kind::Minus) {
                    answer = leftValue - rightValue;
                } else if (op == CalcToken::Kind::Times) {
                    answer = leftValue * rightValue;
                } else if (op == CalcToken::Kind::Divide) {
                    if (rightValue == 0.0) {
                        
                        *error = "division by zero";
                        return false;
                    }
                    answer = leftValue / rightValue;
                }
                values.push(answer);
            }

            if (ops.empty()){
                
                *error = "Error";
                return false;
            }

            ops.pop();
        }
        else {
            int precedence = 0;
            if ((t.kind == CalcToken::Kind::Times) || (t.kind == CalcToken::Kind::Divide)){
                precedence = 2;
            } else { precedence = 1;}

            while (!ops.empty() && (ops.top() != CalcToken::Kind::LParen)){
                int precedenceTop = 1;
                if ((ops.top() == CalcToken::Kind::Times) || (ops.top() == CalcToken::Kind::Divide)){
                precedenceTop = 2;
                }
                if (precedenceTop < precedence){
                    break;
                }

                const CalcToken::Kind op = ops.top();
                ops.pop();
                if (values.size() < 2) {
                    *error = "error";
                    return false;
                }
                double answer = 0.0;
                const double rightValue = values.top();   
                values.pop();
                const double leftValue = values.top();   
                values.pop();
                if (op == CalcToken::Kind::Plus) {
                    answer = leftValue + rightValue;
                } else if (op == CalcToken::Kind::Minus) {
                    answer = leftValue - rightValue;
                } else if (op == CalcToken::Kind::Times) {
                    answer = leftValue * rightValue;
                } else if (op == CalcToken::Kind::Divide) {
                    if (rightValue == 0.0) {
                        
                        *error = "division by zero";
                        return false;
                    }
                    answer = leftValue / rightValue;
                }
                values.push(answer);
                

            }
            ops.push(t.kind);
        }
    }

    while (!ops.empty()) {
        if (ops.top() == CalcToken::Kind::LParen){
            
            *error = ("error");
            return false;

        }
        const CalcToken::Kind op = ops.top();
        ops.pop();
        if (values.size() < 2) {
            *error = "error";
            return false;
        }
        double answer = 0.0;
        const double rightValue = values.top();   
        values.pop();
        const double leftValue = values.top();   
        values.pop();
        if (op == CalcToken::Kind::Plus) {
            answer = leftValue + rightValue;
        } else if (op == CalcToken::Kind::Minus) {
            answer = leftValue - rightValue;
        } else if (op == CalcToken::Kind::Times) {
            answer = leftValue * rightValue;
        } else if (op == CalcToken::Kind::Divide) {
            if (rightValue == 0.0) {
                
                *error = "division by zero";
                return false;
            }
            answer = leftValue / rightValue;

        }
        values.push(answer);
    }
    
    if (values.size() != 1){    
        *error= ("error");
        return false;
    }

    *out = values.top();
    return true;
}
