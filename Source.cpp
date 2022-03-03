#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

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
        if (a < b)
        {
            return -1;
        }

        else if (a > b)
        {
            return 1;
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
    if (comp(a, b) < 0)
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
    if (comp(a, m) == -1) {
        return a;
    }

    if (m == "0") {
        return "NaN";
    }

    while (comp(a, m) > 0)
    {
        int diff = a.size() - m.size();
        if (diff != 0)
        {
            m.append(diff - 1, '0');
            a = sub(a, m);
            m.erase(m.begin() + (m.size() - diff + 1), m.end());
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
    string mStr = "", aStr = "", bStr = "";
    bool check = true;

    do {
        check = true;
        cout << "- Nhap m: ";
        cin >> mStr;
        cin.ignore();

        if (mStr.size() == 0) {
            cout << " * Vui long nhap m." << endl;
            check = false;
            continue;
        }

        if (mStr == "0") {
            cout << " * Vui long nhap m > 0." << endl;
            check = false;
            continue;
        }

        int mLen = mStr.length();
        for (int i = 0; i < mLen; i++) {
            if ((mStr[i] < '0') || (mStr[i] > '9')) {
                cout << " * Vui long nhap m la chuoi so nguyen khong am." << endl;
                check = false;
                break;
            }
        }

        while (mLen > 1) {
            if (mStr[0] == '0') {
                mStr.erase(mStr.begin());
            }
            mLen--;
        }

    } while (!check);

    do {
        check = true;
        cout << "- Nhap a < m: ";
        cin >> aStr;
        cin.ignore();

        if (aStr.size() == 0) {
            cout << " * Vui long nhap a." << endl;
            check = false;
            continue;
        }

        int aLen = aStr.length();
        for (int i = 0; i < aLen; i++) {
            if ((aStr[i] < '0') || (aStr[i] > '9')) {
                cout << " * Vui long nhap a la chuoi so nguyen khong am." << endl;
                check = false;
                break;
            }
        }

        if (!check) continue;

        while (aLen > 1) {
            if (aStr[0] == '0') {
                aStr.erase(aStr.begin());
            }
            aLen--;
        }

        if (comp(aStr, mStr) == 1) {
            cout << " * Vui long nhap a < m." << endl;
            check = false;
        }

    } while (!check);

    do {
        check = true;
        cout << "- Nhap b < m: ";
        cin >> bStr;
        cin.ignore();

        if (bStr.size() == 0) {
            cout << " * Vui long nhap b." << endl;
            check = false;
            continue;
        }

        int bLen = bStr.length();
        for (int i = 0; i < bLen; i++) {
            if ((bStr[i] < '0') || (bStr[i] > '9')) {
                cout << " * Vui long nhap b la chuoi so nguyen khong am." << endl;
                check = false;
                break;
            }
        }

        if (!check) continue;

        while (bLen > 1) {
            if (bStr[0] == '0') {
                bStr.erase(bStr.begin());
            }
            bLen--;
        }

        if (comp(bStr, mStr) == 1) {
            cout << " * Vui long nhap b < m." << endl;
            check = false;
        }

    } while (!check);
    
    cout << endl;
    cout << modulo(aStr, bStr);
}