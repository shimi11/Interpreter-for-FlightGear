#include "calculate.h"

CalculateValue* CalculateValue::instance = 0;

CalculateValue* CalculateValue::getInstance()
{
    if (instance == 0)
    {
        instance = new CalculateValue();
    }

    return instance;
}

CalculateValue::CalculateValue()
{

}

float CalculateValue::shuntingYard(list<string> *line)
{
    stack< string> stack_;
    stack< string> stack_operators;
    list <string>::iterator it;
    for (it = line->begin(); it != line->end(); it++)
    {
        if (*it == "{")
            break;
        if(MATHEMATICAL_OP(*it) || BOOLEAN_OP(*it))
        {
            while (stack_operators.size())
            {
                if (!priorityOp(*it, stack_operators.top()))
                    break;
                stack_.push(stack_operators.top());
                stack_operators.pop();
            }
            stack_operators.push(*it);
            continue;
        }

        if (*it == "(" )
        {
            stack_operators.push(*it);
            continue;
        }
        if (*it == ")")
        {
            while (stack_operators.size())
            {
                if (stack_operators.top() == "(")
                {
                    stack_operators.pop();
                    break;
                }
                stack_.push(stack_operators.top());
                stack_operators.pop();
            }
            continue;
        }
        else
            stack_.push(*it);
    }
    while (stack_operators.size())
    {
        stack_.push(stack_operators.top());
        stack_operators.pop();
    }
   // I used a cartridge instead of a queue so I wouldn't have to overreverse it
    queue <string>queue_;
    while (stack_.size())
    {
        queue_.push(stack_.top());
        stack_.pop();
    }
    return Calculate(queue_);
}

float CalculateValue::Calculate(queue< string> queue_)
{
    string::size_type size_;
    if (queue_.size() == 1)
    {
        string str = queue_.front();
        queue_.pop();
        bool minus = 0;
        if (str[0] == '-')
        {
            minus = 1;
            str.replace(0, 1, "");
        }
        if (is_digits(str))
        {
            //float f = stof(str, &size_);
            return minus? -stof(str, &size_) : stof(str, &size_);
        }

        
        Variables* allVar = Variables::getInstance();
        string path = allVar->getValueBind(str);
        if (path.size() == 0)//if is from var
        {
            //float f = allVar->getValue(str);
            return minus ? -allVar->getValue(str) : allVar->getValue(str);
        }
        //if is from bind
        path.erase(remove(path.begin(), path.end(), '\"'), path.end());

        Data* Data = Data::getInstance();
        float result = Data->getVelue(path);
        return minus ? -result : result;
    }

    string operator_ = queue_.front();
    queue_.pop();
    queue<string>queueRight;
    queue<string>queueLeft;
    if (BOOLEAN_OP(operator_) || MATHEMATICAL_OP(operator_))
    {
        int i;
        for ( i = 0; i < 1; i++)
        {
            string char_ = queue_.front();
            queue_.pop();
            queueRight.push(char_);
            if (BOOLEAN_OP(char_) || MATHEMATICAL_OP(char_))
            {
                i -= 2;
            }
        }
        for (i = 0; i < 1; i++)
        {
            string char_ = queue_.front();
            queue_.pop();
            queueLeft.push(char_);
            if (BOOLEAN_OP(char_) || MATHEMATICAL_OP(char_))
            {
                i -= 2;
            }
        }
       
        
        if (operator_ == "==")
        {
            return (Calculate(queueLeft) == Calculate(queueRight));
        }
        if (operator_ == "<=")
        {
            return (Calculate(queueLeft) <= Calculate(queueRight));
        }
        if (operator_ == ">=")
        {
            return (Calculate(queueLeft) >= Calculate(queueRight));
        }
        if (operator_ == "!=")
        {
            return (Calculate(queueLeft) != Calculate(queueRight));
        }
        if (operator_ == ">")
        {
            return (Calculate(queueLeft) > Calculate(queueRight));
        }
        if (operator_ == "<")
        {
            return (Calculate(queueLeft) < Calculate(queueRight));
        }
        if (operator_ == "+")
            return  Calculate(queueLeft) + Calculate(queueRight);
        if (operator_ == "-")
            return  Calculate(queueLeft) - Calculate(queueRight);
        if (operator_ == "*")
            return  Calculate(queueLeft) * Calculate(queueRight);
        if (operator_ == "/")
        {
            float denominator = Calculate(queueRight);
            if (denominator == 0)
            {
                cout <<"denominator == 0" ; 
                return 0;
            }
            return Calculate(queueLeft) / denominator;
        }
        else
        {
            cout << "Calculator error, the character "<< operator_ <<" is unknown\n";
            return 0;
        }
    }
    else
    {
        cout << "Calculator error, unknown character\n";
        return 0;
    }
}

bool CalculateValue::is_digits(string str)
{
    return all_of(str.begin(), str.end(), [](char i)
        { return isdigit(i) || i == '-' || i == '.'; }); // C++11
}

bool CalculateValue::priorityOp(string op1, string op2)
{
    if (op1 == "*" || op1 == "/")
    {
        return (op2 == "*" || op2 =="/");
    }
    if (op1 == "+" || op1 == "-")
    {
        return ((MATHEMATICAL_OP(op2)) );

    }
    if (BOOLEAN_OP(op1))
    {
        return (op2 != "(");
    }
    return 0;
}
