#include <iostream>
#include <string>

using namespace std;
using Int = int64_t;

enum class Tokens { OPEN, CLOSE, PLUS, MUL, NUM, MINUS, DIV, END };

class Tokanizer {
 public:
  Tokanizer() = delete;
  Tokanizer(string _exp) {
    tokenizedExpression = new char[_exp.size() + 1];
    copy(_exp.begin(), _exp.end(), tokenizedExpression);
    tokenizedExpression[_exp.size()] = '\0';
    expressionPointer = tokenizedExpression;
  }
  Tokanizer(const Tokanizer&) = delete;
  Tokanizer(Tokanizer&&) = delete;
  ~Tokanizer() { delete[] tokenizedExpression; }

  Tokens getToken() {
    string tempNumBuf = "";

    while (*expressionPointer != '\0') {
      switch (*expressionPointer) {
        case ' ':
          ++expressionPointer;
          continue;
        case '+':
          currentToken = Tokens::PLUS;
          ++expressionPointer;
          return currentToken;
        case '-':
          currentToken = Tokens::MINUS;
          ++expressionPointer;
          return currentToken;
        case '*':
          currentToken = Tokens::MUL;
          ++expressionPointer;
          return currentToken;
        case '/':
          currentToken = Tokens::DIV;
          ++expressionPointer;
          return currentToken;
        case '(':
          currentToken = Tokens::OPEN;
          ++expressionPointer;
          return currentToken;
        case ')':
          currentToken = Tokens::CLOSE;
          ++expressionPointer;
          return currentToken;
      }

      if (*expressionPointer > '0' && *expressionPointer <= '9') {
        while (*expressionPointer > '0' && *expressionPointer <= '9') {
          tempNumBuf += *expressionPointer;
          expressionPointer++;
        }
        currentToken = Tokens::NUM;
        value = stoi(tempNumBuf);
        return currentToken;
      } else {
        throw runtime_error("Invalid character");
      }
    }
    return Tokens::END;
  }

  Int getNumValue() const { return this->value; }

 private:
  Tokens currentToken;
  Int value;
  char* expressionPointer;
  char* tokenizedExpression;
};

class Calculator {
 public:
  Calculator() = delete;
  Calculator(string _exp) : tokanizer(_exp) { token = tokanizer.getToken(); }
  Calculator(const Calculator&) = delete;
  Calculator(Calculator&&) = delete;
  ~Calculator() {}

  Int getExpressionResult() {
    result = sumAndSubHandler();
    return result;
  }

 private:
  Int sumAndSubHandler() {
    Int sumAndSubStorage = mulAndDivHandler();
    while (token == Tokens::PLUS || token == Tokens::MINUS) {
      if (token == Tokens::PLUS) {
        token = tokanizer.getToken();
        sumAndSubStorage += mulAndDivHandler();
      } else if (token == Tokens::MINUS) {
        token = tokanizer.getToken();
        sumAndSubStorage -= mulAndDivHandler();
      }
    }
    return sumAndSubStorage;
  }

  Int mulAndDivHandler() {
    Int mulAndDivStorage = 0;
    mulAndDivStorage = itemHandler();

    while (token == Tokens::MUL || token == Tokens::DIV) {
      if (token == Tokens::MUL) {
        token = tokanizer.getToken();
        mulAndDivStorage *= itemHandler();
      } else if (token == Tokens::DIV) {
        token = tokanizer.getToken();
        Int divisor = itemHandler();
        if (divisor == 0) {
          throw runtime_error("Division by zero");
        } else {
          mulAndDivStorage /= divisor;
        }
      }
    }
    return mulAndDivStorage;
  }

  Int itemHandler() {
    Int itemOrExressionStorage = 0;

    if (token == Tokens::CLOSE) {
      throw runtime_error("Opening bracket expected");
    }
    switch (token) {
      case Tokens::NUM:
        itemOrExressionStorage = tokanizer.getNumValue();
        token = tokanizer.getToken();
        if (token == Tokens::OPEN) {
          throw runtime_error("The bracket did not expect");
        }
        break;
      case Tokens::OPEN:
        token = tokanizer.getToken();
        itemOrExressionStorage = sumAndSubHandler();
        if (token == Tokens::CLOSE) {
          token = tokanizer.getToken();
        } else {
          throw runtime_error("Parenthesis mismatch");
        }
        break;
      case Tokens::MINUS:
        token = tokanizer.getToken();
        itemOrExressionStorage = itemHandler() * -1;
        break;
    }
    return itemOrExressionStorage;
  }

  Tokens token;
  Tokanizer tokanizer;
  Int result;
  char* inputExpr;
};

int main() {
  string inputExpression;
  cout << "Enter the expression to be evaluated" << endl;
  getline(cin, inputExpression);

  try {
    Calculator tok(inputExpression);
    auto res = tok.getExpressionResult();
    cout << "Result of expression = " << res << endl;
  } catch (const exception& ex) {
    cout << ex.what() << endl;
  }
}
