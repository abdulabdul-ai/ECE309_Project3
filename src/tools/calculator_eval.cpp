// Mudassir Mohammed
// 10/26/2026
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

// This function returns true and stores the calculated value in *out if the computation is valid
// Otherwise, returns false and sets the *error to be the explanation.
bool calc_evaluate(const std::vector<CalcToken>& tokens, double* out,
                   std::string* error) {

    // Create 2 stacks (LIFO)
    std::stack<double> values;  //stack for values
    std::stack<CalcToken::Kind> ops; //stack for operators

    for(const CalcToken &t : tokens){

        //If the token is categorized as a number, it is pushed onto the value stack
        if (t.kind == CalcToken::Kind::Number){
            values.push(t.value);
        }

        // If the token is "(", it is pushed onto the ops stack
        else if (t.kind == CalcToken::Kind::LParen){
            ops.push(t.kind);
        }

        // If the token is ")", it triggers operations to be applied and popped until the top
        // of the ops stack is (, then continues to accept tokens
        else if (t.kind == CalcToken::Kind::RParen){
            while (!ops.empty() && ops.top() != CalcToken::Kind::LParen){
                //take and pop the top operator              
                const CalcToken::Kind op = ops.top();

                ops.pop();
                // Invalid computation: needs 2 values to operate anything!
                // trigger error message and return false
                if (values.size() < 2) {
                    *error = "need another value";
                    return false;
                }
                
                double answer = 0.0; //initialize the answer to be 0 (as a double)
                const double rightValue = values.top();   //pop right operand
                values.pop();
                const double leftValue = values.top();    //pop left operand
                values.pop();
                if (op == CalcToken::Kind::Plus) {        //If operator is "+",
                    answer = leftValue + rightValue;      //add the right and the left operands
                } else if (op == CalcToken::Kind::Minus) {//If operator is "-",
                    answer = leftValue - rightValue;      //subtract the right from the left operand
                } else if (op == CalcToken::Kind::Times) {//If operator is "*",
                    answer = leftValue * rightValue;      //multiply the right and the left operands
                } else if (op == CalcToken::Kind::Divide) {//If operator is "/",
                  //check if the right (denominator) is 0
                  if (rightValue == 0.0) {               

                        //Invalid Computation: 
                        // Return false and give error explanation
                        *error = "division by zero";      
                        return false;
                    }
                    answer = leftValue / rightValue; //divide the left by the right operands
                }
                values.push(answer); //push the computed value onto the values stack
            }

            if (ops.empty()){
                
               *error = "Error";
               return false;
            }

            ops.pop(); //remove the "(" parenthesis operator
        }
        // The token is "+ - * /"
        else {
            //initialize precedence of the token to be 0
            int precedence = 0;
            if ((t.kind == CalcToken::Kind::Times) || (t.kind == CalcToken::Kind::Divide)){
                precedence = 2; //if the operator is "*" or "/", set their precedence to be 2
            } else { precedence = 1;} // otherwise, precidence is 1 for + or -

            // while there's operator tokens in the ops stack and the top of the stack is not the "("
            // i.e. we're inside a parenthesis block
            while (!ops.empty() && (ops.top() != CalcToken::Kind::LParen)){
                int precedenceTop = 1; //set the precedence of the operator already at the top of the stack to be 1 as default
                if ((ops.top() == CalcToken::Kind::Times) || (ops.top() == CalcToken::Kind::Divide)){
                  precedenceTop = 2; //if the top operator is "*" or "/", set its precedence to be 2
                }

                //apply only if greater than or equal to the new
                if (precedenceTop < precedence){ //
                    break;
                }

                const CalcToken::Kind op = ops.top(); //pop the top operator
                ops.pop();

                // Invalid computation: needs 2 values to operate anything!
                // trigger error message and return false
                if (values.size() < 2) {
                    *error = "error";
                    return false;
                }
                
                double answer = 0.0; //initialize the answer to be 0 (as a double)
                const double rightValue = values.top();   //pop right operand
                values.pop();
                const double leftValue = values.top();    //pop left operand
                values.pop();
                if (op == CalcToken::Kind::Plus) {        //If operator is "+",
                    answer = leftValue + rightValue;      //add the right and the left operands
                } else if (op == CalcToken::Kind::Minus) {//If operator is "-",
                    answer = leftValue - rightValue;      //subtract the right from the left operand
                } else if (op == CalcToken::Kind::Times) {//If operator is "*",
                    answer = leftValue * rightValue;      //multiply the right and the left operands
                } else if (op == CalcToken::Kind::Divide) {//If operator is "/",
                  //check if the right (denominator) is 0
                  if (rightValue == 0.0) {               

                        //Invalid Computation: 
                        // Return false and give error explanation
                        *error = "division by zero";      
                        return false;
                    }
                    answer = leftValue / rightValue; //divide the left by the right operands
                }
                values.push(answer); //push the computed value onto the values stack
            

            }
            ops.push(t.kind); //push the new operator after the computation
        }
    }

    //tokens ran out, apply computations until the ops stack is empty and there's no operator tokens
    while (!ops.empty()) {
        // Invalid Computation
        // Return false and give error message
        if (ops.top() == CalcToken::Kind::LParen){
            
            *error = ("missing a parenthesis");
            return false;

        }

        //take and pop the top operator
        const CalcToken::Kind op = ops.top();
        ops.pop();

        // Invalid computation: needs 2 values to operate anything!
        // trigger error message and return false
        if (values.size() < 2) {
            *error = "need another value";
            return false;
        }
                double answer = 0.0; //initialize the answer to be 0 (as a double)
                const double rightValue = values.top();   //pop right operand
                values.pop();
                const double leftValue = values.top();    //pop left operand
                values.pop();
                if (op == CalcToken::Kind::Plus) {        //If operator is "+",
                    answer = leftValue + rightValue;      //add the right and the left operands
                } else if (op == CalcToken::Kind::Minus) {//If operator is "-",
                    answer = leftValue - rightValue;      //subtract the right from the left operand
                } else if (op == CalcToken::Kind::Times) {//If operator is "*",
                    answer = leftValue * rightValue;      //multiply the right and the left operands
                } else if (op == CalcToken::Kind::Divide) {//If operator is "/",
                  //check if the right (denominator) is 0
                  if (rightValue == 0.0) {               

                        //Invalid Computation: 
                        // Return false and give error explanation
                        *error = "division by zero";      
                        return false;
                    }
                    answer = leftValue / rightValue; //divide the left by the right operands
                }
                values.push(answer); //push the computed value onto the values stack
    }

    //There can only be 1 final answer
    if (values.size() != 1){    
        *error= ("final answer not computed, 2 or more values left on the stack");
        return false;
    }
    //Computation complete, return true and set the *out as the the final answer value
    *out = values.top();
    return true;
}
