#include <iostream>
#include <string>
#include <vector>
using namespace std;

class BigInt {
    string number;    // Stores the number as a string
    bool isNegative;  // True if number is negative

    // Remove unnecessary leading zeros from the number string
    void removeLeadingZeros() {
        for(size_t i = 0; i < number.size() - 1; ++i) {
            if (number[i] != '0') {
                break;
            }
            number.erase(i, 1);
            --i;
        }
    }
    //ADDING OPERATORS FOR MAGNITUDES ARTHIMETIC OPERATIONS (BEGIN)
    static string add(const string& a, const string& b){
        string result;
        int carry = 0;
        int i = (int)a.size() - 1;
        int j = (int)b.size() - 1;
        while (i >= 0 || j >= 0 || carry) {
            int sum = carry;
            if (i >= 0) sum += (a[i--] - '0');
            if (j >= 0) sum += (b[j--] - '0');
            carry = sum / 10;
            result.push_back('0' + sum % 10);
        }
        return result.empty() ? "0" : string(result.rbegin(), result.rend());
    }

    static string subtract(const string& a, const string& b) {
        string result;
        int borrow = 0;
        int i = (int)a.size() - 1;
        int j = (int)b.size() - 1;
        while (i >= 0 || j >= 0) {
            int diff = (a[i] - '0') - borrow;
            if (j >= 0) diff -= (b[j--] - '0');
            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }
            result.push_back('0' + diff);
            --i;
        }
        // Remove leading zeros
        while (result.size() > 1 && result.back() == '0') {
            result.pop_back();
        }
        return string(result.rbegin(), result.rend());
    }
    static string multiply(const string& a,const string& b){
        int n = (int)a.size(), m = (int)b.size();
        vector<int> digits(n + m, 0);

        for (int i = n - 1; i >= 0; --i) {
            for (int j = m - 1; j >= 0; --j) {
                int mul = (a[i] - '0') * (b[j] - '0');
                int p1 = i + j, p2 = i + j + 1;
                int sum = mul + digits[p2];
                digits[p2] = sum % 10;
                digits[p1] += sum / 10;
            }
        }

        string result;
        for (int d : digits) {
            if (!(result.empty() && d == 0)) {
                result.push_back('0' + d);
            }
        }
        return result.empty() ? "0" : result;
    }
    static int compareMagnitude(const string& a, const string& b) {
        if (a.size() != b.size())
            return (a.size() > b.size()) ? 1 : -1;
        if (a == b) return 0;
        return (a > b) ? 1 : -1;
    }
    static string divide(const string& a, const string& b) {
        if (b == "0") {
            throw runtime_error("Error: Division by zero");
        }
        string result;
        string current;
        for (size_t i = 0; i < a.size(); ++i) {
            current.push_back(a[i]);
            int count = 0;
            while (compareMagnitude(current, b) >= 0) {
                current = subtract(current, b);
                ++count;
            }
            result.push_back('0' + count);
        }
        // Remove leading zeros
        while (result.size() > 1 && result[0] == '0') {
            result.erase(0, 1);
        }
        return result.empty() ? "0" : result;
    }
    //(END)
    // Compare absolute values of two BigInts (ignore signs)
    // Returns: 1 if |this| > |other|, 0 if equal, -1 if |this| < |other|


public:
    // Default constructor - initialize to zero
    BigInt() {
        number = "0";
        isNegative = false;
    }

    // Constructor from 64-bit integer
    BigInt(long long value) {
        if(value < 0) {
            isNegative = true;
            value = -value;
        } else {
            isNegative = false;
        }
        number = to_string(value);
    }

    // Constructor from string representation
    BigInt(const string& str) {
        if(str.empty()) {
            number = "0";
            isNegative = false;
            return;
        }
        number = str;
        if(str[0]=='-') {
            isNegative = true;
            number.erase(0, 1);
        } else {
            isNegative = false;
        }

        for(char c : number) {
            if(c < '0' || c > '9') {
                cout << "Invalid character in input string: " << c << " Automatically set to zero." << endl;
                number = "0";
                isNegative = false;
                return;
            }
        }
        removeLeadingZeros();
    }

    // Copy constructor
    BigInt(const BigInt& other) {
        number = other.number;
        isNegative = other.isNegative;
    }

    // Destructor
    ~BigInt() {}
    bool getSign() const {
        return isNegative;
    }

    int compareMagnitude(const BigInt& other) const {
        if(number.size() > other.number.size()) {
            return 1;
        } else if (number.size() < other.number.size()) {
            return -1;
        } else {
            for (size_t i = 0; i < number.size(); ++i) {
                if (number[i] > other.number[i]) {
                    return 1;
                } else if (number[i] < other.number[i]) {
                    return -1;
                }
            }
        }
        return 0;
    }

    // Assignment operator
    BigInt& operator=(const BigInt& other) {
        number = other.number;
        isNegative = other.isNegative;
        return *this;
    }

    // Unary negation operator (-x)
    BigInt operator-() const {
        BigInt result;
        if(number != "0") {
            result.number = number;
            result.isNegative = !isNegative;
        } else {
            result.number = "0";
            result.isNegative = false;
        }
        return result;
    }

    // Unary plus operator (+x)
    BigInt operator+() const {
        BigInt result;
        result.number = number;
        result.isNegative = isNegative;
        return result;
    }

    // Addition assignment operator (x += y)
    BigInt& operator+=(const BigInt& other) {
        if(isNegative == other.isNegative) {
            // Same sign: add magnitudes
            number = add(number, other.number);
        }else {
            // Different signs: subtract smaller from larger
            int cmp = compareMagnitude(other);
            if(cmp == 0) {
                number = "0";
                isNegative = false;
            } else if(cmp > 0) {
                number = subtract(number, other.number);
            } else {
                number = subtract(other.number, number);
                isNegative = other.isNegative;
            }
        }

        return *this;
    }

    // Subtraction assignment operator (x -= y)
    BigInt& operator-=(const BigInt& other) {
            if(isNegative != other.isNegative) {
                number = add(number, other.number);
            } else {
                int cmp = compareMagnitude(other);
                if(cmp == 0) {
                    number = "0";
                    isNegative = false;
                } else if(cmp > 0) {
                    number = subtract(number, other.number);
                } else {
                    number = subtract(other.number, number);
                    isNegative = !other.isNegative;
                }
            }
        return *this;
    }

    // Multiplication assignment operator (x *= y)
    BigInt& operator*=(const BigInt& other) {
        isNegative = (isNegative != other.isNegative);
        number = multiply(number, other.number);
        return *this;
    }

    // Division assignment operator (x /= y)
    BigInt& operator/=(const BigInt& other) {
        if (other.number == "0") {
            throw runtime_error("Division by zero");
        }
        isNegative = (isNegative != other.isNegative);
        number = divide(number, other.number);
        return *this;
    }

    // Modulus assignment operator (x %= y)
    BigInt& operator%=(const BigInt& other) {
    if(other.number == "0") {
        throw runtime_error("Division by zero");
    }
    BigInt quotient = *this;
    quotient /= other;                   
    BigInt product = quotient;
    product *= other;                    
    *this -= product;
    return *this;
}

    // Pre-increment operator (++x)
    BigInt& operator++() {
        *this += BigInt(1);
        return *this;
    }

    // Post-increment operator (x++)
    BigInt operator++(int) {
        BigInt temp;
        temp = *this;
        *this += BigInt(1);
        return temp;
    }

    // Pre-decrement operator (--x)
    BigInt& operator--() {
        *this -= BigInt(1);
        return *this;
    }

    // Post-decrement operator (x--)
    BigInt operator--(int) {
        BigInt temp;
        temp = *this;
        *this -= BigInt(1);
        return temp;
    }

    // Convert BigInt to string representation
    string toString() const {
            if (number == "0") {
                return "0";
            }
            return (isNegative ? "-" : "") + number;

        return this->number;
    }

    // Output stream operator (for printing)
    friend ostream& operator<<(ostream& os, const BigInt& num) {
        os << num.toString();
        return os;
    }

    // Input stream operator (for reading from input)
    friend istream& operator>>(istream& is, BigInt& num) {
        string input;
        is >> input;
        num = BigInt(input);
        return is;
    }

    // Friend declarations for comparison operators
    friend bool operator==(const BigInt& lhs, const BigInt& rhs);
    friend bool operator<(const BigInt& lhs, const BigInt& rhs);
};

// Binary addition operator (x + y)
BigInt operator+(BigInt lhs, const BigInt& rhs) {
    return lhs += rhs;
}

// Binary subtraction operator (x - y)
BigInt operator-(BigInt lhs, const BigInt& rhs) {
    return lhs -= rhs;
}

// Binary multiplication operator (x * y)
BigInt operator*(BigInt lhs, const BigInt& rhs) {
    return lhs *= rhs;
}
// Binary division operator (x / y)
BigInt operator/(BigInt lhs, const BigInt& rhs) {
    return lhs /= rhs;
}

// Binary modulus operator (x % y)
BigInt operator%(BigInt lhs, const BigInt& rhs) {
    return lhs %= rhs;
}

// Equality comparison operator (x == y)
bool operator==(const BigInt& lhs, const BigInt& rhs) {
    return (lhs.compareMagnitude(rhs) == 0) && (lhs.isNegative == rhs.isNegative);
}

// Inequality comparison operator (x != y)
bool operator!=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs == rhs);
}

// Less-than comparison operator (x < y)
bool operator<(const BigInt& lhs, const BigInt& rhs) {
    if(lhs.isNegative == false && rhs.isNegative == true) {
        return false;
    } else if(lhs.isNegative == true && rhs.isNegative == false) {
        return true;
    } else if(lhs.isNegative == false && rhs.isNegative == false) {
        return lhs.compareMagnitude(rhs) < 0;
    } else {
        return lhs.compareMagnitude(rhs) > 0;
    }
}

// Less-than-or-equal comparison operator (x <= y)
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
    if(lhs.getSign() == false && rhs.getSign() == true) {
        return true;
    } else if(lhs.getSign() == true && rhs.getSign() == false) {
        return false;
    } else if(lhs.getSign() == false && rhs.getSign() == false) {
        return lhs.compareMagnitude(rhs) <= 0;
    } else {
        return lhs.compareMagnitude(rhs) >= 0;
    }

}

// Greater-than comparison operator (x > y)
bool operator>(const BigInt& lhs, const BigInt& rhs) {
    if(lhs.getSign() == false && rhs.getSign() == true) {
        return true;
    } else if(lhs.getSign() == true && rhs.getSign() == false) {
        return false;
    } else if(lhs.getSign() == false && rhs.getSign() == false) {
        return lhs.compareMagnitude(rhs) > 0;
    } else {
        return lhs.compareMagnitude(rhs) < 0;
    }
}

// Greater-than-or-equal comparison operator (x >= y)
bool operator>=(const BigInt& lhs, const BigInt& rhs) {
    if(lhs.getSign() == false && rhs.getSign() == true) {
        return true;
    } else if(lhs.getSign() == true && rhs.getSign() == false) {
        return false;
    } else if(lhs.getSign() == false && rhs.getSign() == false) {
        return lhs.compareMagnitude(rhs) >= 0;
    } else {
        return lhs.compareMagnitude(rhs) <= 0;
    }
}

int main() {
    cout << "=== BigInt Class Test Program ===" << endl << endl;
    cout << "NOTE: All functions are currently empty." << endl;
    cout << "Your task is to implement ALL the functions above." << endl;
    cout << "The tests below will work once you implement them correctly." << endl << endl;


    // Test 1: Constructors and basic output
    cout << "1. Constructors and output:" << endl;
    BigInt a(12345);              // Should create BigInt from integer
    BigInt b("-67890");           // Should create BigInt from string
    BigInt c("0");                // Should handle zero correctly
    BigInt d = a;                 // Should use copy constructor
    cout << "a (from int): " << a << endl;        // Should print "12345"
    cout << "b (from string): " << b << endl;     // Should print "-67890"
    cout << "c (zero): " << c << endl;            // Should print "0"
    cout << "d (copy of a): " << d << endl << endl; // Should print "12345"

    // Test 2: Arithmetic operations
    cout << "2. Arithmetic operations:" << endl;
    cout << "a + b = " << a + b << endl;          // Should calculate 12345 + (-67890)
    cout << "a - b = " << a - b << endl;          // Should calculate 12345 - (-67890)
    cout << "a * b = " << a * b << endl;          // Should calculate 12345 * (-67890)
    cout << "b / a = " << b / a << endl;          // Should calculate (-67890) / 12345
    cout << "a % 100 = " << a % BigInt(100) << endl << endl; // Should calculate 12345 % 100

    // Test 3: Relational operators
    cout << "3. Relational operators:" << endl;
    cout << "a == d: " << (a == d) << endl;       // Should be true (12345 == 12345)
    cout << "a != b: " << (a != b) << endl;       // Should be true (12345 != -67890)
    cout << "a < b: " << (a < b) << endl;         // Should be false (12345 < -67890)
    cout << "a > b: " << (a > b) << endl;         // Should be true (12345 > -67890)
    cout << "c == 0: " << (c == BigInt(0)) << endl << endl; // Should be true (0 == 0)

    // Test 4: Unary operators and increments
    cout << "4. Unary operators and increments:" << endl;
    cout << "-a: " << -a << endl;                 // Should print "-12345"
    cout << "++a: " << ++a << endl;               // Should increment and print "12346"
    cout << "a--: " << a-- << endl;               // Should print "12346" then decrement
    cout << "a after decrement: " << a << endl << endl; // Should print "12345"

    // Test 5: Large number operations
    cout << "5. Large number operations:" << endl;
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    cout << "Very large addition: " << num1 + num2 << endl;
    cout << "Very large multiplication: " << num1 * num2 << endl << endl;

    // Test 6: Edge cases and error handling
    cout << "6. Edge cases:" << endl;
    BigInt zero(0);
    BigInt one(1);
    try {
        BigInt result = one / zero;               // Should throw division by zero error
        cout << "Division by zero succeeded (unexpected)" << endl;
    } catch (const runtime_error& e) {
        cout << "Division by zero correctly threw error: " << e.what() << endl;
    }
    cout << "Multiplication by zero: " << one * zero << endl;        // Should be "0"
    cout << "Negative multiplication: " << BigInt(-5) * BigInt(3) << endl;  // Should be "-15"
    cout << "Negative division: " << BigInt(-10) / BigInt(3) << endl;       // Should be "-3"
    cout << "Negative modulus: " << BigInt(-10) % BigInt(3) << endl;        // Should be "-1"


    return 0;
}
