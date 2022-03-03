#include <iostream>
#include <string>
#include <algorithm>
#include <vector>


std::vector<int> toVec(std::string a)
{
    std::vector<int> res;
    for (int i = 0; i < a.length(); i++)
    {
        res.push_back(((int)a[i] - '0'));
    }

    return res;
}

std::string toString(std::vector<int> vec)
{
    std::string res = "";
    while (vec.front() == 0 && vec.size() != 1)
    {
        vec.erase(vec.begin());
    }
    for (int i = 0; i < vec.size(); i++)
    {
        vec[i] += (int)'0';
        res += (char)vec[i];
    }

    return res;
}

int comp(std::string a, std::string b)
{
    if (a.size() == b.size())
    {
        for (int i = 0; i < a.size(); i++)
        {
            if ((int)a[i]- '0' > (int)b[i]-'0')
            {
                return 1;
            }
            else if((int)a[i]- '0' < (int)b[i]-'0')
            {
                return -1;
            }
        }

        return 0;
    }
    else if (a.size() > b.size())
        return 1;
    else 
        return -1;
}

void logarr(std::vector<int> vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        std::cout << vec[i];
    }
}
std::string add(std::string a, std::string b) // assume a > b
{
    std::vector<int> vecA = toVec(a);
    std::vector<int> vecB = toVec(b);
    std::vector<int> resVec;

    reverse(vecA.begin(), vecA.end());
    reverse(vecB.begin(), vecB.end());

    int len = std::min(vecA.size(), vecB.size());
    int carry = 0;
    for (int i = 0; i < len; i++)
    {
        int digit = vecA[i] + vecB[i] + carry;
        carry = digit / 10;
        resVec.push_back(digit % 10);
    }

    if (vecA.size() > len)
    {
        for (int i = len; i < vecA.size(); i++)
        {
            int digit = vecA[i] + carry;
            carry = digit / 10;
            resVec.push_back(digit % 10);
        }
    }

    if (vecB.size() > len)
    {
        for (int i = len; i < vecB.size(); i++)
        {
            int digit = vecB[i] + carry;
            carry = digit / 10;
            resVec.push_back(digit % 10);
        }
    }

    while (carry)
    {
        resVec.push_back(carry % 10);
        carry /= 10;
    }

    reverse(resVec.begin(), resVec.end());
    return toString(resVec);
}

std::string sub(std::string a, std::string b) // assume a - b
{
    if (comp(a,b) < 0)
    {
        return sub(b, a);
    }
    std::vector<int> vecA = toVec(a);
    std::vector<int> vecB = toVec(b);
    std::vector<int> resVec;

    reverse(vecA.begin(), vecA.end());
    reverse(vecB.begin(), vecB.end());

    int len = std::min(vecA.size(), vecB.size());
    int carry = 0;
    for (int i = 0; i < len; i++)
    {
        int digit = vecA[i] - (vecB[i] + carry);
        if (digit < 0)
        {
            digit += 10;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        resVec.push_back(digit);
    }

    for (int i = len; i < vecA.size(); i++)
    {
        int digit = vecA[i] - carry;
        if (digit < 0)
        {
            digit += 10;
            carry = 1;
        }
        else
        {
            carry = 0;
        }
        resVec.push_back(digit);
    }

    reverse(resVec.begin(), resVec.end());
    return toString(resVec);
}

std::string modulo(std::string a, std::string m) // a mod m ( a >= m && m != 0 )
{
    if (a.compare(m) > 0)
    {
        return a;
    }

    if (m.compare("0") == 0)
    {
        return a;
    }

    while (comp(a,m) > 0)
    {
        int diff = a.size() - m.size();
        if (diff!=0)
        {
            m.append(diff-1,'0');
            a = sub(a, m);
            m.erase(m.begin()+(m.size()-diff+1),m.end());
        }
        else
        {
            a = sub(a, m);
        }

    }

    return a;
}

int main()
{
    std::string a = "214523625142351472549725416384423521";
    std::string b = "215423985471295321458";
    std::cout << modulo(a, b);
}