// stl

// boost

// third-party

// other
#include "StringUtils.h"

extern "C"
{
#include <string.h>
#include <stdio.h>
}

#include <stack>
#include <cmath>
#include <algorithm>
#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include <errno.h>

using namespace std;

void StringUtils::split(const std::string& src, std::vector<std::string>& ret)
{
	 split(src, ",;\t /|", ret);
}

void StringUtils::split(const std::string& src, const std::string splitStr,
			std::vector<std::string>& ret)
{
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(splitStr.c_str());
	tokenizer toks(src, sep);
	for(tokenizer::iterator it = toks.begin(); it != toks.end(); ++it)
	{
		ret.push_back(*it);
	}
}

std::string StringUtils::replace(std::string& instr, const std::string& src, const std::string& dest)
{
	string outstr = instr;
	string::size_type pos = instr.find(src);
	if (pos != string::npos){
		outstr = instr.replace(pos, src.length(), dest);
	}

	return outstr;
}

std::string StringUtils::replaceAll(std::string& instr, const std::string& src, const std::string& dest)
{
	string outstr = instr;
	while(1){
		string::size_type pos = instr.find(src);
		if (pos == string::npos){
			break;
		}
		outstr = instr.replace(pos, src.length(), dest);
	}

	return outstr;
}

//std::string	StringUtils::toString(uint32_t i)
//{
//	try{
//		return boost::lexical_cast<std::string>(i);
//	} catch(boost::bad_lexical_cast& e)
//	{
//		return "";
//	}
//}

//std::string StringUtils::toString(const boost::posix_time::ptime& time)
//{
//	std::string str = boost::posix_time::to_iso_extended_string(time);
//	boost::replace_first(str, "T" ," ");
//	return str;
//}

int StringUtils::toInt32(const std::string& str, int32_t & value)
{
	errno = 0;
	char* endptr;
	value = strtol(str.c_str(), &endptr, 10);
	if ((errno == ERANGE && (value == LONG_MAX || value == LONG_MIN))
	                   || (errno != 0 && value == 0))
	{
		value = 0;
		return 1;
	}

	if (endptr == str) {
		value = 0;
		return 1;
	}

	return 0;
}

int StringUtils::toUInt32(const std::string& str, uint32_t & value)
{
	errno = 0;
	char* endptr;
	value = strtoul(str.c_str(), &endptr, 10);
	if ((errno == ERANGE && (value ==UINT_MAX || value == INT_MIN))
					   || (errno != 0 && value == 0))
	{
		value = 0;
		return 1;
	}

	if (endptr == str) {
		value = 0;
		return 1;
	}

	return 0;
}

int StringUtils::toInt64(const std::string& str, int64_t & value)
{
	errno = 0;
	char* endptr;
	value = strtoll(str.c_str(), &endptr, 10);
	if ((errno == ERANGE && (value == LLONG_MAX  || value == LLONG_MIN))
					   || (errno != 0 && value == 0))
	{
		value = 0;
		return 1;
	}

	if (endptr == str) {
		value = 0;
		return 1;
	}

	return 0;
}

int StringUtils::toUInt64(const std::string& str, uint64_t & value)
{
	errno = 0;
	char* endptr;
	value = strtoull(str.c_str(), &endptr, 10);
	if ((errno == ERANGE && (value == ULLONG_MAX  || value == LLONG_MIN))
					   || (errno != 0 && value == 0))
	{
		value = 0;
		return 1;
	}

	if (endptr == str) {
		value = 0;
		return 1;
	}

	return 0;
}


int getOperatorPriority(const std::string& op)
{
    int priority = -1;
    switch (op[0])
    {
        case '(':
            priority = 4;
            break;
        case '+':
        case '-':
            priority = 1;
            break;
        case '*':
        case '/':
        case '%':
            priority = 2;
            break;
        case '^':
            priority = 3;
            break;
    }
    return priority;
}

bool isLetter(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return true;
    return false;
}

bool isOperator(char c)
{
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^')
        return true ;
    return false ;
}

bool isBracket(char c)
{
    if (c == '(' || c == ')')
        return true;
    return false;
}

int getToken(const string& expr, string& token, size_t& idx)
{
    char curChar = 0;
    bool inTok = false ;
    int res = 0;
    token.clear();
    size_t len = expr.length();
    while (idx < len) {
        curChar = expr[idx++];
        if (isspace(curChar) || isOperator(curChar) || isBracket(curChar)) {
            if (!inTok && (isOperator(curChar) || isBracket(curChar))) {
                token += curChar;
                res = 1; // token is a operatro or bracket, return token immediately.
                break;
            }
            if (inTok) {
                if (isOperator(curChar) || isBracket(curChar))
                    --idx; // back input pointer and get the operator or bracket when we call getToken() next time.
                res = 1;
                break;
            }
            inTok = false;
        }  else if (isLetter(curChar)) { // letter is illegal.
            res = 0;
            break;
        }  else if (!inTok) { // begin a number.
            inTok = true;
            token += curChar;
        } else if (inTok) {
            token += curChar;
        }
    }
    if (!token.empty())
        res = 1;
    return res;
}

int infix_to_postfix(const string& infix_expr, string& postfix_expr)
{
	std::string token;
	size_t idx = 0;
	std::stack < string > stk;
	int balance = 0; // use to check the bracket's balance.
	postfix_expr.clear();
	while (getToken(infix_expr, token, idx)) {
		switch (token[0]) {
		/* If we see '+','-','*','/','%','^','(' , then we pop entries from
		 * the stack until we find an entry of lower priority
		 * One exception is that we never remove a '(' from the stack except when processing a ')'.*/
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '(':
			if (token[0] == '(')
				++balance;
			while (!stk.empty() && getOperatorPriority(stk.top())
					>= getOperatorPriority(token) && stk.top() != "(") {
				postfix_expr += stk.top();
				stk.pop();
				postfix_expr += " ";
			}
			stk.push(token);
			break;
			/* right association, handle it specifically! */
		case '^':
			while (!stk.empty() && getOperatorPriority(stk.top())
					> getOperatorPriority(token) && stk.top() != "(") {
				postfix_expr += stk.top();
				stk.pop();
				postfix_expr += " ";
			}
			stk.push(token); // later come, early out.(right association)
			break;
			/* If we see a ')', then we pop the stack, writing symbols until we encounter
			 * a (corresponding) left parenthesis, which is poped but not output. */
		case ')':
			--balance;
			while (!stk.empty() && stk.top() != "(") {
				postfix_expr += stk.top();
				stk.pop();
				postfix_expr += " ";
			}
			stk.pop();
			break;
		default:
			postfix_expr += token;
			postfix_expr += " ";
			break;
		}
	}
	while (!stk.empty()) {
		postfix_expr += " ";
		postfix_expr += stk.top();
		stk.pop();
	}
	if (balance != 0) {
		return 0;
	}
	return 1;
}

int compute_postfix(const std::string& postfix_expr, double& result)
{
	std::stack<std::string> stk;
	std::string token;
    size_t idx = 0;
    double operand1 = 0.0, operand2 = 0.0;
    char buf[128] = {0};
    int resStatus = 0;
    while (getToken(postfix_expr, token, idx)) {
        memset(buf, 0, 128 * sizeof(char));
        switch (token[0]) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^':
                // firstly, get two operands to compute.
                operand1 = atof(stk.top().c_str());
                stk.pop();
                operand2 = atof(stk.top().c_str());
                stk.pop();
                switch (token[0]) {
                    case '+':
                        sprintf(buf, "%f", operand1 + operand2);
                        break;
                    case '-':
                        sprintf(buf, "%f", operand2 - operand1);
                        break;
                    case '*':
                        sprintf(buf, "%f", operand1 * operand2);
                        break;
                    case '/':
                        if (!operand1) {
                            resStatus = 0;
                            goto Exit;
                        }
                        sprintf(buf, "%f", operand2 / operand1);
                        break;
                    case '%':
                        // if operand is a float number, then error!
                        if ((int)operand1 != operand1 || (int)operand2 != operand2 || !operand1) {
                            resStatus = 0;
                            goto Exit;
                        }
                        // care: the format should be "%d".
                        sprintf(buf, "%d", (int)operand2 % (int)operand1);
                        break;
                    case '^':
                        if (operand2 <= 0) {
                            resStatus = 0;
                            goto Exit;
                        }
                        sprintf(buf, "%f", pow(operand2, operand1));
                        break;
                }
                stk.push(string(buf));
                break;
            default:
                stk.push(token); // numbers push into the stack directly.
                break;
        }
    }
    // now the number in the stack is the result.
    result = atof(stk.top().c_str());
    resStatus = 1;
Exit:
    return resStatus;
}

//void StringUtils::toPtime(const std::string& str,  boost::posix_time::ptime& time)
//{
//	try{
//		time = boost::posix_time::time_from_string(str);
//	}catch(std::out_of_range& e)
//	{
//		LOG4CXX_INFO(fogs::log::gCommonLogger, "Error: covert string "<<str<<" to time,"<<e.what());
//		time =  boost::posix_time::second_clock::local_time();
//	}
//	return;
//}



double StringUtils::eval(const std::string& formula)
{
	if(formula == "")
	{
		return 0;
	}

	string infix_expr, postfix_expr;
	infix_expr = formula;
	double result = 0.0;
	if (infix_to_postfix(infix_expr, postfix_expr)) {
		compute_postfix(postfix_expr, result);
	}
	return result;
}

std::string StringUtils::toHexString(int i)
{
	std::ostringstream oss;
	oss<<"0x"<<std::hex<<i;
	return oss.str();
}
