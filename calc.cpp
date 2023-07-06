#include <cassert>
#include <iostream>
#include <string>
#include <vector>

union TokenValue {
  int num; 
  char oper;
};

enum class TokenType {
  NUM,
  OPER,
  LPAREN,
  RPAREN
};

struct Token {
  TokenType type;
  TokenValue value;

  Token(int num) : type(TokenType::NUM), value() {
    value.num = num;
  }
  
  Token(char oper) : type(TokenType::OPER), value() {
    value.oper = oper;
  }
  
  Token(TokenType type) : type(type), value() {
    assert(type == TokenType::LPAREN || type == TokenType::RPAREN);
  }
};

/*void lexer(string expr) {
  for (size_t i  = 0, i < expr.length(); ++i) {
    if ()
  }
}*/

std::ostream& operator << (std::ostream& os, const Token& tok) {
  switch(tok.type) {
    case TokenType::NUM: 
      os << tok.value.num;
      break;
    case TokenType::OPER:
      os << tok.value.oper;
      break;
    case TokenType::LPAREN:
      os << '(';
      break;
    case TokenType::RPAREN:
      os << ')';
      break;
  }
  return os;
}

int main() {
    std::string expr;
    std::cout << "Enter your expression:\n";
    std::cin >> expr;

    std::vector<Token> tokens = {
        Token(123), 
        Token('+'), 
        Token(12), 
        Token(TokenType::LPAREN), 
        Token(TokenType::RPAREN)
    };

    for(Token t : tokens) {
      std::cout << t << "\n";
    }

    return 0;
}