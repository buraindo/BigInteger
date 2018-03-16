#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

void copy_array(vector<int> &lhs, const vector<int> &rhs) {
    for (int i = 0; i != (int) lhs.size(); i++) {
        lhs[i] = rhs[i];
    }
}

class BigInteger {
public:

    friend ostream &operator<<(ostream &s, BigInteger &bg) {
        s << bg.get_string();
        return s;
    }

    friend istream &operator>>(istream &s, BigInteger &bg) {
        string input;
        s >> input;
        bg.ZERO = false;
        if (input[0] == '-')
            bg.number.assign(input.size() - 1, 0);
        else
            bg.number.assign(input.size(), 0);
        bg.parse_string(input);
        return s;
    }

    BigInteger &operator+=(const BigInteger &_rhs) {
        BigInteger lhs = *this;
        BigInteger rhs = _rhs;
        if (lhs.ZERO && rhs.ZERO) {
            return *this;
        } else if (lhs.ZERO) {
            initialize(rhs.number, rhs.get_sign(), rhs.get_length());
            set_zero(false);
            return *this;
        } else if (rhs.ZERO) {
            return *this;
        }
        string value;
        string s1 = lhs.get_string();
        string s2 = rhs.get_string();
        auto length1 = (int) s1.size();
        auto length2 = (int) s2.size();
        //cout << s1 << " " << s2;
        if (lhs.get_sign() == 1 && rhs.get_sign() == 1) {
            string r = add(s1, s2);
            //cout << r << "\n";
            value = r;
        } else if (lhs.get_sign() == -1 && rhs.get_sign() == -1) {
            lhs.set_sign(1);
            rhs.set_sign(1);
            BigInteger ret = lhs + rhs;
            ret.set_sign(-1);
            initialize(ret.number, ret.get_sign(), ret.get_length());
            set_zero(ret.get_zero());
            return *this;
        } else if (lhs.get_sign() == -1) {
            lhs.set_sign(1);
            BigInteger ret = rhs - lhs;
            initialize(ret.number, ret.get_sign(), ret.get_length());
            set_zero(ret.get_zero());
            return *this;
        } else if (rhs.get_sign() == -1) {
            rhs.set_sign(1);
            BigInteger ret = lhs - rhs;
            initialize(ret.number, ret.get_sign(), ret.get_length());
            set_zero(ret.get_zero());
            return *this;
        }
        string res = value;
        BigInteger ret(res);
        initialize(ret.number, ret.get_sign(), ret.get_length());
        set_zero(ret.get_zero());
        return *this;
    }

    BigInteger &operator-=(const BigInteger &_rhs) {
        BigInteger lhs = *this;
        BigInteger rhs = _rhs;
        if (lhs.ZERO && rhs.ZERO) {
            return *this;
        } else if (lhs.ZERO) {
            initialize(rhs.number, -rhs.get_sign(), rhs.get_length());
            set_zero(false);
            return *this;
        } else if (rhs.ZERO) {
            return *this;
        }
        if (lhs == rhs) {
            BigInteger ret("0");
            initialize(ret.number, 1, ret.get_length());
            set_zero(true);
            return *this;
        }
        string value;
        string s1 = lhs.get_string();
        string s2 = rhs.get_string();
        auto length1 = (int) s1.size();
        auto length2 = (int) s2.size();
        if (lhs.get_sign() == 1 && rhs.get_sign() == 1) {
            string r;
            if (compare(s1, s2) == 1) {
                //cout << stoll(r1) << " " << stoll(r2) << " " << r1 << " " << r2;
                r = subtract(s1, s2);
            } else {
                r = subtract(s2, s1);
                r.insert(0, "-");
            }
            value = r;
        } else if (lhs.get_sign() == -1 && rhs.get_sign() == -1) {
            lhs.set_sign(1);
            rhs.set_sign(1);
            BigInteger ret = rhs - lhs;
            initialize(ret.number, ret.get_sign(), ret.get_length());
            return *this;
        } else if (lhs.get_sign() == -1) {
            lhs.set_sign(1);
            BigInteger ret = lhs + rhs;
            ret.set_sign(-1);
            initialize(ret.number, ret.get_sign(), ret.get_length());
            return *this;
        } else {
            rhs.set_sign(1);
            BigInteger ret = lhs + rhs;
            initialize(ret.number, ret.get_sign(), ret.get_length());
            return *this;
        }
        string res = value;
        //cout << " result : " << res << endl;
        BigInteger ret(res);
        initialize(ret.number, ret.get_sign(), ret.get_length());
        return *this;
    }

    BigInteger &operator*=(const BigInteger &_rhs) {
        BigInteger lhs = *this;
        BigInteger rhs = _rhs;
        if (lhs.ZERO || rhs.ZERO) {
            BigInteger ret("0");
            initialize(ret.number, 1, ret.get_length());
            set_zero(true);
            return *this;
        }
        string value;
        string s1 = lhs.get_string();
        string s2 = rhs.get_string();
        auto length1 = (int) s1.size();
        auto length2 = (int) s2.size();
        if (lhs.get_sign() == 1 && rhs.get_sign() == 1 || lhs.get_sign() == -1 && rhs.get_sign() == -1) {
            lhs.set_sign(1);
            rhs.set_sign(1);
            s1 = lhs.get_string();
            s2 = rhs.get_string();
            string r = multiply(s1, s2);
            value = r;
        } else if (lhs.get_sign() == -1 || rhs.get_sign() == -1) {
            lhs.set_sign(1);
            rhs.set_sign(1);
            BigInteger ret = lhs * rhs;
            ret.set_sign(-1);
            initialize(ret.number, ret.get_sign(), ret.get_length());
            return *this;
        }
        string res = value;
        //cout << " result : " << res << endl;
        BigInteger ret(res);
        initialize(ret.number, ret.get_sign(), ret.get_length());
        return *this;
    }

    friend BigInteger operator-(BigInteger lhs, const BigInteger &_rhs) {
        lhs -= _rhs;
        return lhs;
    }

    friend BigInteger operator+(BigInteger lhs, const BigInteger &_rhs) {
        lhs += _rhs;
        return lhs;
    }

    friend BigInteger operator*(BigInteger lhs, const BigInteger &_rhs) {
        lhs *= _rhs;
        return lhs;
    }

    friend bool operator==(BigInteger &_lhs, BigInteger &_rhs) {
        return _lhs.get_string() == _rhs.get_string();
    }

    string subtract(string lhs, const string &rhs) {
        int length = (int) min(lhs.size(), rhs.size());
        string dominant = lhs.size() > rhs.size() ? lhs : rhs;
        string res;
        for (int i = 0; i != length; i++) {
            int digit = lhs[lhs.size() - i - 1] - rhs[rhs.size() - i - 1];
            string str_digit;
            if (digit < 0) {
                if (i != (int) max(lhs.size(), rhs.size()) - 1) {
                    if (lhs[lhs.size() - i - 2] != '0') {
                        lhs[lhs.size() - i - 2]--;
                        str_digit = to_string(10 + digit);
                    } else {
                        int pos = -1;
                        for (int q = (int) lhs.size() - i - 2; q != -1; q--) {
                            if (lhs[q] != '0') {
                                pos = q;
                                break;
                            }
                        }
                        lhs[pos]--;
                        for (int q = (int) lhs.size() - i - 2; q != pos; q--) {
                            lhs[q] = '9';
                        }
                        str_digit = to_string(10 + digit);
                    }
                } else {
                    str_digit = to_string(abs(digit));
                }
            } else str_digit = to_string(digit);
            res += str_digit;
        }
        string res1 = rhs.size() > lhs.size() ? rhs.substr(0, rhs.size() - length) : lhs.substr(0, lhs.size() - length);
        //cout << res1 << " :RES1" << endl;
        reverse(res1.begin(), res1.end());
        res += res1;
        reverse(res.begin(), res.end());
        //cout << "RES: " << res << "\n RES1: " << res1 << "\n";
        return res;
    }

    string add(const string &lhs, const string &rhs) {
        int length = (int) min(lhs.size(), rhs.size());
        //cout << "LENGTH: " << length << endl;
        string res;
        string carry = "0";
        for (int i = 0; i != length; i++) {
            string digit = to_string(
                    lhs[lhs.size() - i - 1] - '0' + rhs[rhs.size() - i - 1] - '0' + stoll(carry));
            if (digit.size() > 1) {
                res += digit[1];
            } else res += digit;
            carry = digit.size() > 1 ? to_string(digit[0] - '0') : "0";
        }
        string res1 = rhs.size() > lhs.size() ? rhs.substr(0, rhs.size() - length) : lhs.substr(0, lhs.size() - length);
        //cout << res1 << " :RES1" << endl;
        reverse(res1.begin(), res1.end());
        res += res1;
        reverse(res.begin(), res.end());
        int pos = (int) max(lhs.size(), rhs.size()) - length - 1;
        if (res1.empty()) {
            if (carry != "0")
                res.insert(0, carry);
        } else {
            while (carry != "0") {
                if (pos == -1 && carry != "0") {
                    res.insert(0, carry);
                    carry = "0";
                    break;
                }
                int digit = (res[pos] - '0') + stoll(carry);
                if (digit > 9) {
                    res[pos] = (char) (digit - 10) + '0';
                    carry = "1";
                } else {
                    res[pos] = (char) digit + '0';
                    carry = "0";
                }
                pos--;
            }
        };
        //cout << "RES: " << res << "\n RES1: " << res1 << "\n";
        return res;
    }

    string multiply(const string &lhs, const string &rhs) {
        int length = (int) min(lhs.size(), rhs.size());
        int max_length = (int) max(lhs.size(), rhs.size());
        string dominant = lhs.size() > rhs.size() ? lhs : rhs;
        string defeated = lhs.size() < rhs.size() ? lhs : rhs;
        if (lhs.size() == rhs.size()) {
            dominant = lhs;
            defeated = rhs;
        }
        //cout << "LENGTH: " << length << endl;
        string res;
        string carry = "0";
        int zeros = 0;
        for (int i = length - 1; i != -1; i--) {
            string temp;
            carry = "0";
            for (int j = max_length - 1; j != -1; j--) {
                string digit = to_string((dominant[j] - '0') * (defeated[i] - '0') + stoll(carry));
                if (digit.size() > 1) carry = to_string(digit[0] - '0');
                else carry = "0";
                if (digit.size() > 1) temp += digit[1]; else temp += digit;
            }
            reverse(temp.begin(), temp.end());
            temp += string(zeros++, '0');
            if (carry != "0")
                temp.insert(0, carry);
            if (res.empty()) res = temp;
            else res = add(res, temp);
            //cout << "TEMP: " << temp << "\n";
        }
        return res;
    }

    void initialize(const vector<int> &n, int _sign, int _length) {
        if (number.size() < n.size())
            number.resize(n.size());
        copy_array(number, n);
        length = 0;
        sign = 0;
        set_sign(_sign);
        set_length(_length);
    }

    void initialize(bool _ZERO) {
        set_sign(1);
        set_length(1);
        set_zero(_ZERO);
    }

    explicit BigInteger(const string &input) {
        ZERO = false;
        if (input[0] == '-')
            number.assign(input.size() - 1, 0);
        else number.assign(input.size(), 0);
        parse_string(input);
    }

    explicit BigInteger(const int &input) {
        ZERO = false;
        if (input < 0)
            number.assign(to_string(input).size() - 1, 0);
        else
            number.assign(to_string(input).size(), 0);
        parse_string(to_string(input));
    }

    explicit BigInteger(const long long &input) {
        ZERO = false;
        if (input < 0)
            number.assign(to_string(input).size() - 1, 0);
        else
            number.assign(to_string(input).size(), 0);
        parse_string(to_string(input));
    }

    BigInteger() = default;

    bool get_zero() {
        return ZERO;
    }

    int get_sign() { return sign; }

    int get_length() { return length; }

    string get_string() {
        if (ZERO) {
            return "0";
        }
        string result;
        bool zero_achieved = false;
        result += copy_chr(number, zero_achieved);
        if (sign == -1) {
            result.insert(0, "-");
        }
        return result;
    }

    void parse_string(const string &input) {
        if (input == "0") {
            ZERO = true;
            return;
        }
        if (!isdigit(input[1]) && input.size() > 1) {
            cout <<
                 "You are trying to parse an incorrect string, make sure to parse a string which contains only digits and a sign, string: " +
                 input << endl;
            throw exception();
        }
        auto length = (int) input.length();
        char sign_chr = input[0];
        int sign = sign_chr == '-' ? -1 : 1;
        vector<int> n;
        if (sign == -1)
            n.assign(length - 1, 0);
        else
            n.assign(length, 0);
        if (sign == -1) {
            if (n.size() == 1) {
                n[0] = input[1] - '0';
            } else
            for (int i = 1; i != length; i++) {
                n[i - 1] = input[i] - '0';
            }
        } else {
            if (n.size() == 1) {
                n[0] = input[0] - '0';
            } else
            for (int i = 0; i != length; i++) {
                n[i] = input[i] - '0';
            }
        }
        initialize(n, sign, length);
    }

    string copy_chr(const vector<int> &arg, bool &zero_achieved, int k = 0) {
        string result;
        if (k == 0 && arg[k] == 0) {
            zero_achieved = true;
            k = -1;
        }
        for (int i = 0; i != arg.size(); i++) {
            if (arg[i] == 0) {
                if (!zero_achieved) {
                    result += to_string(arg[i]);
                }
            } else {
                result += to_string(arg[i]);
                zero_achieved = false;
            }
        }
        return result;
    }

    int compare(const string &lhs, const string &rhs) {
        if (lhs.size() > rhs.size())
            return 1;
        if (lhs.size() < rhs.size())
            return -1;
        for (int i = 0; i != (int) lhs.size(); i++) {
            if (lhs[i] - '0' > rhs[i] - '0')
                return 1;
            if (lhs[i] - '0' < rhs[i] - '0')
                return -1;
        }
        return 0;
    }


private:

    bool ZERO{};
    int length{};
    int sign{};
    vector<int> number;

    void set_zero(bool _zero) {
        ZERO = _zero;
    }

    void set_length(int length) {
        this->length = length;
    }

    void set_sign(int sign) {
        if (sign != -1 && sign != 1) {
            cout << "Wrong sign: it must be either -1 or 1 \n";
            return;
        }
        this->sign = sign;
    }

    void set_sign(char sign) {
        if (sign != '+' && sign != '-') {
            cout << "Wrong sign: it must be either - or +";
            return;
        }
        this->sign = sign == '+' ? 1 : -1;
    }

};

int main() {
    BigInteger bg1;
    BigInteger bg2;
    cout << "Type first number, please: \n";
    cin >> bg1;
    cout << "Type second number, please: \n";
    cin >> bg2;
    cout << "\n" << flush;
    BigInteger result = bg1 + bg2;
    cout << "Result of sum operation: " << result << "\n";
    result = bg1 - bg2;
    cout << "Result of subtract operation: " << result << "\n";
    result = bg1 * bg2;
    cout << "Result of multiply operation: " << result;
    return 0;
}
