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

bool lex_num (const std::string& expr, size_t* index, std::vector<Token>& tokens) {
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

bool lex_space (const std::string& expr, size_t* index, std::vector<Token>& tokens) {
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

bool lex_paren (const std::string& expr, size_t* index, std::vector<Token>& tokens) {
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

bool lex_oper (const std::string& expr, size_t* index, std::vector<Token>& tokens) {
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

bool lexer (const std::string& expr, std::vector<Token>& tokens) {
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

Tree* parse_num (const std::vector<Token>& tokens, size_t* index) {
  size_t i = *index;
  if (i < tokens.size() && tokens[i].type == TokenType::NUM) {
    Tree* t = new Tree;
    t->num = tokens[i].value.num;
    t->is_leaf = true;
    *index = ++i;
    return t;
  }
  return nullptr;
}

Tree* parse (const std::vector<Token>& tokens, size_t* index) {
  size_t i = *index;
  Tree* t1 = parse_num(tokens, &i);
  if (t1 != nullptr) {
      
  } else if (i < tokens.size() && tokens[i].type == TokenType::LPAREN) {
    ++i;
    t1 = parse(tokens, &i);
    assert(i < tokens.size() && tokens[i].type == TokenType::RPAREN);
    ++i;
  }
  if (i == tokens.size() || tokens[i].type == TokenType::RPAREN) {
    *index = i;
    return t1;
  }

  assert(i < tokens.size() && tokens[i].type == TokenType::OPER);
  char oper = tokens[i].value.oper;
  ++i;

  Tree* t2 = parse_num(tokens, &i);
  if (t2 != nullptr) {
      
  } else if (i < tokens.size() && tokens[i].type == TokenType::LPAREN) {
    ++i;
    t2 = parse(tokens, &i);
    assert(i < tokens.size() && tokens[i].type == TokenType::RPAREN);
    ++i;
  }

  Tree* t3 = new Tree;
  t3->node.oper = oper;
  t3->node.left = t1;
  t3->node.right = t2;
  t3->is_leaf = false;

  *index = i;
  return t3;
}

int main() {
    std::string expr;
    std::cout << "Enter your expression:\n";
    std::getline(std::cin, expr);
    std::vector<Token> tokens;
    if (!lexer(expr, tokens)) {
      return 1;
    }
    /*for(Token t : tokens) {
      std::cout << t << "\n";
    }*/

    size_t index = 0;
    Tree* t = parse(tokens, &index);
    if (t == nullptr) {
      std::cout << "Empty tree.\n";
    } else {
      std::cout << "Tree: " << *t << "\n";
    }

    return 0;
}