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

bool lex_num(const std::string& expr, size_t* index, std::vector<Token>& tokens) {
  int result = 0;
  size_t i = *index;
  for (; i < expr.length(); ++i) {
    if (expr[i] >= '0' && expr[i] <= '9') {
      result = result * 10 + (expr[i] - '0');
    } else {
      break;
    }
  }
  if (i > *index) {
    tokens.push_back(Token(result));
    *index = i;
    return true;
  }
  return false;
}

bool lex_space(const std::string& expr, size_t* index, std::vector<Token>& tokens) {
  size_t i = *index;
  for (; i < expr.length(); ++i) {
    if (expr[i] != ' ') {
      break;
    }
  }
  if (i > *index) {
    *index = i;
    return true;
  } 
  return false;
}

bool lex_paren(const std::string& expr, size_t* index, std::vector<Token>& tokens) {
  size_t i = *index;
  if (i < expr.length()) {
    if (expr[i] == '(') {
      tokens.push_back(Token(TokenType::LPAREN));
      *index = ++i;
      return true;
    } else if (expr[i] == ')') {
      tokens.push_back(Token(TokenType::RPAREN));
      *index = ++i;
      return true;
    }
  }
  return false;
}

bool lex_oper(const std::string& expr, size_t* index, std::vector<Token>& tokens) {
  size_t i = *index;
  if (i < expr.length()) {
    char oper = expr[i];
    if (oper == '+' || oper == '-' || oper == '*' || oper == '/' ) {
      *index = ++i;
      tokens.push_back(Token(oper));
      return true;
    }
  }
  return false;
}

bool lexer(const std::string& expr, std::vector<Token>& tokens) {
  size_t index = 0;
  while (index < expr.length()) {
    if (lex_num(expr, &index, tokens) ||
        lex_space(expr, &index, tokens) || 
        lex_paren(expr, &index, tokens) ||
        lex_oper(expr, &index, tokens)) {
      continue;
    } else {
      std::cout << "Error at index " << index << ": \"" << expr.substr(index, std::string::npos) << "\".\n";
      return false;
    }
  } 
  return true;
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

struct Tree; 

struct TreeNode {
  Tree* left;
  Tree* right;
  char oper;
};

struct Tree {
  union {         // anonymous union
    TreeNode node;
    size_t num;
  };
  bool is_leaf;
};

std::ostream& operator << (std::ostream& os, const Tree& t) {
  if (t.is_leaf) {
    os << t.num;
  } else {
    os << "(";
    os << *t.node.left << " ";
    os << t.node.oper << " ";
    os << *t.node.right;
    os << ")";
  }
  return os;
}

int main() {
    std::string expr;
    std::cout << "Enter your expression:\n";
    std::getline(std::cin, expr);
    std::vector<Token> tokens;
    if (!lexer(expr, tokens)) {
      return 1;
    }
    for(Token t : tokens) {
      std::cout << t << "\n";
    }
    // 3 * (1 + 2)
    Tree t2;
    t2.num = 3;
    t2.is_leaf = true;

    Tree t4;
    t4.num = 1;
    t4.is_leaf = true;

    Tree t5;
    t5.num = 2;
    t5.is_leaf = true;

    Tree t3;
    t3.node.oper = '+';
    t3.node.left = &t4;
    t3.node.right = &t5;
    t3.is_leaf = false;

    Tree t1;
    t1.node.oper = '*';
    t1.node.left = &t2;
    t1.node.right = &t3;
    t1.is_leaf = false;

    std::cout << "t1 = " << t1 << "\n";

    return 0;
}