#ifndef TO_POSTFIX_H
#define TO_POSTFIX_H

#include <iostream>
#include <sstream>
#include "stack.h"
using namespace std;

bool operand(const char& input){
    string x = "-+*/()";
    for (int i = 0; i < x.length(); i++){
        if (input == x[i])
            return false;
    }

    return true;
}


int precedence(const char& input){
    
    if (input == '(')
        return 0;
    else if (input == '-' || input == '+')
        return 1;
    else if (input == '*' || input == '/')
        return 2;
}

string to_postfix(const string& infix) {
    
    Stack<char> S;
    string s = "";
    string temp;
    string input = "";
    stringstream str;
    str << infix;

    while (str >> temp)
        input += temp;
    
    int length = input.length();

    
    for (int i = 0; i < length; i++){


        // we will check whether the operator '-' is unary or not
        if (i+1 < length && input[i] == '-'){
            
            if (i == 0){
                
                if (input[i+1] == '('){
                    s = s + '-' + '1' + ' ';
                    input[i] = '*';
                    i = i - 1;
                }
                else
                    s += '-'; 
                
                continue;
                
            }

            else if (!operand(input[i-1]) && operand(input[i+1]) && input[i-1] != ')'){
                s += '-';
                continue;
            }

            else if (!operand(input[i-1]) && input[i+1] == '(' && input[i-1] != ')'){
                s = s + '-' + '1' + ' ';
                input[i] = '*';
                i = i - 1;
                continue;
            }
        }
        
        if (operand(input[i])){
            
            int j = i;
            int num_oper = 0;
            
            while (operand(input[j])){
                s += input[j];
                j++;
                num_oper++;
                if (j == length)
                    break;
            }
            
            // corner case handling
            if (i == 0 && num_oper == length)
                ;
            // corner case handling
            else if ( i-1 >= 0 && num_oper == length - 1);
            // corner case handling
            else if (j == length-1 && input[j] == ')' && S.size() == 1);
            else
                s += ' ';

            i = j-1;
        }

       
        else if (input[i] == '('){
                S.push(input[i]);
            }
        
        else if (input[i] == ')'){
            while (S.top() != '('){    
                
                char temp = S.top();
                S.pop();

                if (S.size() != 1 || i != length - 1)
                    s = s + temp + ' ';
                
                else
                    s += temp; 
    
            }
            
            S.pop();
        }

        else if ((S.empty() || S.top() == '(')){
            S.push(input[i]);
        }
        
        
        else if (precedence(input[i]) > precedence(S.top())){
                S.push(input[i]);
            }
            
        else if (precedence(input[i]) <= precedence(S.top())){
            while (precedence(input[i]) <= precedence(S.top())){
                s = s + S.top() + ' ';
                S.pop();
                if (S.empty())
                    break;
            }

            S.push(input[i]);
        }
        
        }


    while (!S.empty()){
        char temp = S.top();
        S.pop();
        if (!S.empty())
            s = s + temp + ' ';
        else
            s += temp;
    }

    
    int len = s.length();
    if (len > 0 && s[len-1] == ' '){
        string s_new = "";
        int i = 0;
        while (i != len - 1){
            s_new += s[i];
            i++;
        }

        return s_new;
    }


    return s;

}

#endif //TO_POSTFIX_H
