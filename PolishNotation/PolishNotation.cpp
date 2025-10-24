#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <cctype>
#include <functional>
#include <random>
#include <limits>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_E
#define M_E 2.71828182845904523536
#endif

static std::mt19937 gen(std::random_device{}());
enum class Arity { Unary, Binary, Ternary};

struct Operator {
    Arity arity;
    int precedence;
    bool rightAssociative;
    std::function<double(double, double)> func;
    std::function<double(double)> func_unary;
    std::function<double(double, double, double)> func_ternary;

    Operator() = default;
    Operator(Arity a, int p, bool r, std::function<double(double, double)> f)
        : arity(a), precedence(p), rightAssociative(r),
        func(f), func_unary(nullptr), func_ternary(nullptr) {
    }
    Operator(Arity a, int p, bool r, std::function<double(double)> f)
        : arity(a), precedence(p), rightAssociative(r),
        func(nullptr), func_unary(f), func_ternary(nullptr) {
    }
    Operator(Arity a, int p, bool r, std::function<double(double, double, double)> f)
        : arity(a), precedence(p), rightAssociative(r),
        func(nullptr), func_unary(nullptr), func_ternary(f) {
    }
};


// --- Реєстр операторів ---
std::unordered_map<std::string, Operator> operators = {
    {">", Operator(Arity::Binary, 1, false, [](double a, double b) { return a > b ? 1 : 0; })},
    {"<", Operator(Arity::Binary, 1, false, [](double a, double b) { return a < b ? 1 : 0; })},
    {">=", Operator(Arity::Binary, 1, false, [](double a, double b) { return a >= b ? 1 : 0; })},
    {"<=", Operator(Arity::Binary, 1, false, [](double a, double b) { return a <= b ? 1 : 0; })},
    {"==", Operator(Arity::Binary, 1, false, [](double a, double b) { return std::fabs(a - b) < 1e-9 ? 1 : 0; })},
    {"!=", Operator(Arity::Binary, 1, false, [](double a, double b) { return std::fabs(a - b) >= 1e-9 ? 1 : 0; })},

    {"xor", Operator(Arity::Binary,0,false,[](double a,double b) {return (a != 0) ^ (b != 0) ? 1 : 0; })},
    {"&&", Operator(Arity::Binary, 1, false, [](double a, double b) { return (a != 0 && b != 0) ? 1 : 0; })},
    {"||", Operator(Arity::Binary, 0, false, [](double a, double b) { return (a != 0 || b != 0) ? 1 : 0; })},

    {"+", Operator(Arity::Binary, 2, false, [](double a, double b) { return a + b; })},
    {"-", Operator(Arity::Binary, 2, false, [](double a, double b) { return a - b; })},
    {"*", Operator(Arity::Binary, 3, false, [](double a, double b) { return a * b; })},
    {"/", Operator(Arity::Binary, 3, false, [](double a, double b) { return a / b; })},
    {"^", Operator(Arity::Binary, 4, true,  [](double a, double b) { return std::pow(a, b); })},
    {"pow", Operator(Arity::Binary, 4, true,  [](double a, double b) { return std::pow(a, b); })},

    // --- Розширена арифметика ---
    {"mod", Operator(Arity::Binary, 4, false, [](double a, double b) { return std::fmod(a, b); })},
    {"div", Operator(Arity::Binary, 4, false, [](double a, double b) { return std::floor(a / b); })},
    {"mean", Operator(Arity::Binary, 6, false, [](double a, double b) { return (a + b) / 2.0; })},
    {"harmonic", Operator(Arity::Binary, 6, false, [](double a, double b) { return 2.0 / ((1.0 / a) + (1.0 / b)); })},
    {"geomean", Operator(Arity::Binary, 6, false, [](double a, double b) { return std::sqrt(a * b); })},

    // --- Логарифми та корені ---
    {"ln", Operator(Arity::Unary, 6, true, [](double x) { return std::log(x); })},
    {"log10", Operator(Arity::Unary, 6, true, [](double x) { return std::log10(x); })},
    {"log", Operator(Arity::Binary, 6, true, [](double base, double value) { return std::log(value) / std::log(base); })},
    {"sqrt", Operator(Arity::Unary, 5, true, [](double x) { return std::sqrt(x); })},
    {"cbrt", Operator(Arity::Unary, 5, true, [](double x) { return std::cbrt(x); })},
    {"square", Operator(Arity::Unary, 5, true, [](double x) { return x * x; })},
    {"inv", Operator(Arity::Unary, 5, true, [](double x) { return 1.0 / x; })},
    {"root", Operator(Arity::Binary, 5, true, [](double base, double n) { return std::pow(base, 1.0 / n); })},

    // --- Експоненти, модулі, унарні ---
    {"exp", Operator(Arity::Unary, 6, true, [](double x) { return std::exp(x); })},
    {"abs", Operator(Arity::Unary, 6, true, [](double x) { return std::fabs(x); })},
    {"!", Operator(Arity::Unary, 6, true, [](double x) { return (x == 0) ? 1 : 0; })},
    {"+u", Operator(Arity::Unary, 6, true, [](double x) { return x; })},
    {"-u", Operator(Arity::Unary, 6, true, [](double x) { return -x; })},

    // --- Тригонометрія ---
    {"sin", Operator(Arity::Unary, 6, true, [](double x) { return std::sin(x); })},
    {"cos", Operator(Arity::Unary, 6, true, [](double x) { return std::cos(x); })},
    {"tan", Operator(Arity::Unary, 6, true, [](double x) { return std::tan(x); })},
    {"sinh", Operator(Arity::Unary, 6, true, [](double x) { return std::sinh(x); })},
    {"cosh", Operator(Arity::Unary, 6, true, [](double x) { return std::cosh(x); })},
    {"tanh", Operator(Arity::Unary, 6, true, [](double x) { return std::tanh(x); })},
    {"asin", Operator(Arity::Unary, 6, true, [](double x) { return std::asin(x); })},
    {"acos", Operator(Arity::Unary, 6, true, [](double x) { return std::acos(x); })},
    {"atan", Operator(Arity::Unary, 6, true, [](double x) { return std::atan(x); })},

    {"min", Operator(Arity::Binary, 7, true, [](double a, double b) { return std::min(a, b); })},
    {"max", Operator(Arity::Binary, 7, true, [](double a, double b) { return std::max(a, b); })},
    {"avg", Operator(Arity::Binary, 7, true, [](double a, double b) { return (a + b) / 2.0; })},
    {"min3", Operator(Arity::Ternary,7,true,[](double a,double b,double c) {return std::min({a,b,c}); })},
    {"max3", Operator(Arity::Ternary,7,true,[](double a,double b,double c) {return std::max({a,b,c}); })},
    {"mix", Operator(Arity::Ternary,7,true,[](double a,double b,double t) {return a + (b - a) * t; })}, // альтернатива lerp

    // --- Округлення ---
    {"ceil", Operator(Arity::Unary, 6, true, [](double x) { return std::ceil(x); })},
    {"floor", Operator(Arity::Unary, 6, true, [](double x) { return std::floor(x); })},
    {"round", Operator(Arity::Unary, 6, true, [](double x) { return std::round(x); })},
    {"trunc", Operator(Arity::Unary, 6, true, [](double x) { return std::trunc(x); })},
    {"sign", Operator(Arity::Unary, 6, true, [](double x) { return (x > 0) - (x < 0); })},

    // --- Геометрія ---
    {"hypot", Operator(Arity::Binary, 6, true, [](double a, double b) { return std::hypot(a, b); })},
    {"dist2d", Operator(Arity::Binary, 6, true, [](double x, double y) { return std::sqrt(x * x + y * y); })},
    {"deg2rad", Operator(Arity::Unary, 6, true, [](double x) { return x * M_PI / 180.0; })},
    {"rad2deg", Operator(Arity::Unary, 6, true, [](double x) { return x * 180.0 / M_PI; })},

    // --- Факторіали та комбінаторика ---
    {"fact", Operator(Arity::Unary, 6, true, [](double x) {
        if (x < 0) return std::numeric_limits<double>::quiet_NaN();
        double r = 1;
        for (int i = 1; i <= (int)x; ++i) r *= i;
        return r;
    })},
    {"nCr", Operator(Arity::Binary, 6, true, [](double n, double r) {
        if (r > n) return 0.0;
        double res = 1;
        for (int i = 1; i <= r; ++i) res *= (n - i + 1) / i;
        return res;
    })},
    {"nPr", Operator(Arity::Binary, 6, true, [](double n, double r) {
        if (r > n) return 0.0;
        double res = 1;
        for (int i = 0; i < r; ++i) res *= (n - i);
        return res;
    })},
    {"binom", Operator(Arity::Binary, 6, true, [](double n, double k) {
        return std::tgamma(n + 1) / (std::tgamma(k + 1) * std::tgamma(n - k + 1));
    })},
    {"gamma", Operator(Arity::Unary, 6, true, [](double x) { return std::tgamma(x); })},

    // --- Фінансові функції ---
    {"percent", Operator(Arity::Binary, 6, true, [](double total, double part) { return (part / total) * 100.0; })},
    {"discount", Operator(Arity::Binary, 6, true, [](double price, double percent) { return price * (1 - percent / 100.0); })},
    {"compound", Operator(Arity::Binary, 6, true, [](double rate, double periods) { return std::pow(1 + rate / 100.0, periods); })},
    {"future", Operator(Arity::Binary, 6, true, [](double principal, double rate) { return principal * (1 + rate / 100.0); })},

    // --- Випадковість ---
    {"rand", Operator(Arity::Unary, 6, true, [](double max) {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_real_distribution<> dist(0.0, max);
        return dist(gen);
    })},
    {"randint", Operator(Arity::Binary, 6, true, [](double min, double max) {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dist((int)min, (int)max);
        return dist(gen);
    })},
    {"gauss", Operator(Arity::Binary, 6, true, [](double mean, double stddev) {
        static std::mt19937 gen(std::random_device{}());
        std::normal_distribution<> dist(mean, stddev);
        return dist(gen);
    })},

    // --- Тринарні функції ---
    {"if", Operator(Arity::Ternary, 7, true, [](double cond, double a, double b) { return cond != 0 ? a : b; })},
    {"clamp3", Operator(Arity::Ternary, 7, true, [](double x, double min, double max) { return std::max(min, std::min(max, x)); })},
    {"lerp", Operator(Arity::Ternary, 7, true, [](double a, double b, double t) { return a + (b - a) * t; })},
    {"smoothstep", Operator(Arity::Ternary, 6, true, [](double edge0, double edge1, double x) {
        x = std::clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
        return x * x * (3 - 2 * x);
    })},
    {"fma", Operator(Arity::Ternary, 6, true, [](double a, double b, double c) { return std::fma(a, b, c); })},
    {"median", Operator(Arity::Ternary, 6, true, [](double a, double b, double c) {
        return std::max(std::min(a, b), std::min(std::max(a, b), c));
    })},

    // --- Текстові та службові ---
    {"print", Operator(Arity::Unary, 6, true, [](double x) {
        std::cout << "print: " << x << std::endl;
        return x;
    })},
    {"noop", Operator(Arity::Unary, 6, true, [](double x) { return x; })},

    // --- Аналітичні ---
    {"sigmoid", Operator(Arity::Unary, 6, true, [](double x) { return 1.0 / (1.0 + std::exp(-x)); })},
    {"step", Operator(Arity::Binary, 6, true, [](double edge, double x) { return x > edge ? 1.0 : 0.0; })}
};


// --- Токенізація ---
bool isNumber(const std::string& s) {
    if (s.empty()) return false;
    bool dot = false;
    for (char c : s) if (!isdigit(c) && c != '.') return false;
    return true;
}

std::vector<std::string> tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string cur;
    for (size_t i = 0; i < expr.size(); ++i) {
        char c = expr[i];
        if (isspace(c)) continue;

        // --- числа ---
        if (isdigit(c) || c == '.') {
            cur += c;
            if (i + 1 == expr.size() || (!isdigit(expr[i + 1]) && expr[i + 1] != '.')) {
                tokens.push_back(cur);
                cur.clear();
            }
            continue;
        }

        // --- імена функцій ---
        if (isalpha(c)) {
            cur += c;
            while (i + 1 < expr.size() && (isalnum(expr[i + 1]) || expr[i + 1] == '_'))
                cur += expr[++i];
            tokens.push_back(cur);
            cur.clear();
            continue;
        }

        // --- багатосимвольні оператори ---
        std::string two;
        if (i + 1 < expr.size()) two = std::string{ c, expr[i + 1] };
        if (two == "&&" || two == "||" || two == "==" || two == "!=" || two == ">=" || two == "<=" || two == "**") {
            tokens.push_back(two);
            ++i;
            continue;
        }

        // --- односимвольні оператори або дужки ---
        tokens.push_back(std::string(1, c));
    }
    return tokens;
}


// --- Позначаємо унарні + та - ---
void markUnary(std::vector<std::string>& tokens) {
    for (size_t i = 0; i < tokens.size(); ++i) {
        if ((tokens[i] == "+" || tokens[i] == "-") &&
            (i == 0 || tokens[i - 1] == "(" || operators.count(tokens[i - 1]))) {
            tokens[i] += "u";
        }
    }
}

// --- Перетворення інфіксного виразу у префіксний ---
std::vector<std::string> infixToPrefix(const std::string& expr) {
    std::vector<std::string> tokens = tokenize(expr);
    markUnary(tokens);

    std::reverse(tokens.begin(), tokens.end());
    for (auto& t : tokens) {
        if (t == "(") t = ")";
        else if (t == ")") t = "(";
    }

    std::stack<std::string> opStack;
    std::vector<std::string> output;

    for (auto& token : tokens) {
        if (isNumber(token)) {
            output.push_back(token);
        }
        else if (operators.count(token)) {
            auto& op1 = operators[token];
            while (!opStack.empty() && operators.count(opStack.top())) {
                auto& op2 = operators[opStack.top()];
                if ((op1.precedence < op2.precedence) ||
                    (op1.precedence == op2.precedence && !op1.rightAssociative)) {
                    output.push_back(opStack.top());
                    opStack.pop();
                }
                else break;
            }
            opStack.push(token);
        }
        else if (token == "(") {
            opStack.push(token);
        }
        else if (token == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                output.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        }
    }

    while (!opStack.empty()) {
        output.push_back(opStack.top());
        opStack.pop();
    }

    std::reverse(output.begin(), output.end());
    return output;
}

// --- Рекурсивне обчислення польського виразу ---
double evalPrefix(std::istringstream& iss) {
    std::string token;
    if (!(iss >> token))
        throw std::runtime_error("Unexpected end of expression");

    // --- 🔢 Константи ---
    if (token == "pi")  return M_PI;
    if (token == "e")   return M_E;
    if (token == "phi") return 1.61803398875;
    if (token == "tau") return 2 * M_PI;
    if (token == "inf") return std::numeric_limits<double>::infinity();
    if (token == "nan") return std::numeric_limits<double>::quiet_NaN();
    if (token == "true") return 1;
    if (token == "false") return 0;

    // --- 🔠 Числа ---
    bool isNumber = !token.empty() &&
        (isdigit(token[0]) ||
            ((token[0] == '-' || token[0] == '+') && token.size() > 1 && isdigit(token[1])));

    if (isNumber)
        return std::stod(token);

    // --- 🔧 Перевірка на наявність оператора ---
    auto it = operators.find(token);
    if (it == operators.end())
        throw std::runtime_error("Unknown operator: " + token);

    const Operator& op = it->second;

    // --- 🧠 Обчислення ---
    if (op.arity == Arity::Unary) {
        double x = evalPrefix(iss);
        return op.func_unary(x);
    }
    else if (op.arity == Arity::Binary) {
        double a = evalPrefix(iss);
        double b = evalPrefix(iss);
        return op.func(a, b);
    }
    else if (op.arity == Arity::Ternary) {
        double a = evalPrefix(iss);
        double b = evalPrefix(iss);
        double c = evalPrefix(iss);
        return op.func_ternary(a, b, c);
    }

    throw std::runtime_error("Unsupported arity for operator: " + token);
}


double evaluateInfix(const std::string& expr) {
    auto prefix = infixToPrefix(expr);
    std::ostringstream joined;
    for (auto& t : prefix) joined << t << ' ';
    std::istringstream iss(joined.str());
    return evalPrefix(iss);
}

// --- Тестування ---
int main() {

    while (true)
    {
        std::cout << "Enter a regular expression:\n> ";
        std::string expr;
        std::getline(std::cin, expr);

        try {
            auto prefix = infixToPrefix(expr);
            std::cout << "\nPrefix (Polish) notation:\n> ";
            for (auto& t : prefix) std::cout << t << ' ';
            std::cout << "\n";

            double result = evaluateInfix(expr);
            std::cout << "\nResult: " << result << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
