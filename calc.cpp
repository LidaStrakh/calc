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

Token lex_num(const std::string& expr, size_t* index) {
  int result = 0;
  size_t i = *index;
  for (; i < expr.length(); ++i) {
    if (expr[i] >= '0' && expr[i] <= '9') {
      result = result * 10 + (expr[i] - '0');
    } else {
      break;
    }
  }
  *index = i;
  return Token(result);
}

void lex_space(const std::string& expr, size_t* index) {
  size_t i = *index;
  for (; i < expr.length(); ++i) {
    if (expr[i] != ' ') {
      break;
    }
  }
  *index = i;
}

void lexer(const std::string& expr, std::vector<Token>& tokens) {
  size_t index = 0;
  while (index < expr.length()) {
    size_t i = index;
    Token t = lex_num(expr, &index);
    if (i < index) {
      tokens.push_back(t); 
      continue;
    } 
    lex_space(expr, &index);
    if (i < index) {
      continue;
    }
    assert(i == index);
    return;
  } 
}

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
    std::getline(std::cin, expr);
    std::vector<Token> tokens;
    lexer(expr, tokens);

    for(Token t : tokens) {
      std::cout << t << "\n";
    }
   

    return 0;
}