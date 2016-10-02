//  Copyright 2016 Ryo Shinoki. Released under the MIT license.

#include "main.h"

const char *FORMULA      = "1+3+5*(3+5)";
const char *FILE_NAME    = "formula.txt";

int main(int argc, const char * argv[]) {
  write_text(FORMULA, FILE_NAME);
  FILE *file = get_file_ptr(FILE_NAME);
  Node *tree = read_expression(file);
  compile_nodeTree(tree);
  return 0;
}

Node* read_expression(FILE *file) {
  Node *leftNode, *topNode;
  leftNode = read_term(file);
  for (;;) {
    int asciiChar = getc(file);
    if (!isspace(asciiChar)) {
      topNode = (Node*)malloc(sizeof(struct _Node));
      topNode->leftNode  = leftNode;
      if (asciiChar == '+') {
        topNode->nodeType  = OPERATOR_PLUS;
        topNode->rightNode = read_term(file);
      }else if (asciiChar == '-') {
        topNode->nodeType  = OPERATOR_MINUS;
        topNode->rightNode = read_term(file);
      } else {
        ungetc(asciiChar, file);                  // push back char to file
        break;
      }
      //Current topNode will be a left child Node if next asciiChar is + or -.
      leftNode = topNode;
    }
  }
  return leftNode;
}

Node* read_term(FILE *file) {
  Node *leftNode, *topNode;
  leftNode = read_factor(file);
  for (;;) {
    int asciiChar = getc(file);
    topNode = (Node*)malloc(sizeof(struct _Node));
    topNode->leftNode  = leftNode;
    if (!isspace(asciiChar)) {
      if (asciiChar == '*') {
        topNode->nodeType  = OPERATOR_MULTI;
        topNode->rightNode = read_factor(file);
      } else if (asciiChar == '/') {
        topNode->nodeType  = OPERATOR_DIV;
        topNode->rightNode = read_factor(file);
      } else {
        ungetc(asciiChar, file);                   // push back char to file
        break;
      }
      //Current topNode will be a left child Node if next asciiChar is * or /.
      leftNode = topNode;
    }
  }
  
  return leftNode;
}

Node* read_factor(FILE *file) {
  Node* ret;
  // loop for skiping space
  for (;;) {
    int asciiChar = getc(file);
    if (!isspace(asciiChar)) {
      // return number if not '('
      if (isdigit(asciiChar)) {
        ungetc(asciiChar, file);                  // push back char to file
        return read_num(file);
        
      } else if (asciiChar == '(') {
        ret = read_expression(file);
        // loop for skiping space and ')' in factor
        for (;;) {
          int _asciiChar = getc(file);
          if (!isspace(_asciiChar)) {
            if (_asciiChar == ')') {
              return ret;                         // return if find ')'
            } else {
              fprintf(stderr,"Unexpectred character '%c' in read_factor \n", asciiChar);
              return 0;
            }
          }
        }                                         // End loop for skiping space and ')' in factor
      } else {
        fprintf(stderr, "Unexpectred character '%c' in read_factor \n", asciiChar);
        return 0;
      }
    }
  }                                               // End loop for skiping spaces
  return ret;
}

Node* read_num(FILE *file) {
  Node* ret;
  for (;;) {
    int asciiChar = getc(file);
    if (!isspace(asciiChar)) {
      if (isdigit(asciiChar)) {
        int integerChar = 0;
        do {
          integerChar = integerChar * 10 + asciiChar - '0';
          asciiChar = getc(file);
        } while (isdigit(asciiChar));
        ret = (Node *)malloc(sizeof(Node));
        ret->nodeType    = OPERAND;
        ret->operandNum = integerChar;
        ungetc(asciiChar, file);                  // push back char to file
        break;
      } else {
        fprintf(stderr, "Unexpectred character '%c' in read_num \n", asciiChar);
        return 0;
      }
    }
  }
  return ret;
}

void compile_nodeTree(Node *tree)
{
  switch(tree->nodeType) {
    case OPERAND:
      printf("PUSH %d \n",tree->operandNum);
      break;
    case OPERATOR_PLUS:
      compile_nodeTree(tree->leftNode);
      compile_nodeTree(tree->rightNode);
      printf("ADD \n");
      break;
    case OPERATOR_MINUS:
      compile_nodeTree(tree->leftNode);
      compile_nodeTree(tree->rightNode);
      printf("SUB \n");
      break;
    case OPERATOR_MULTI:
      compile_nodeTree(tree->leftNode);
      compile_nodeTree(tree->rightNode);
      printf("MULTI \n");
      break;
    case OPERATOR_DIV:
      compile_nodeTree(tree->leftNode);
      compile_nodeTree(tree->rightNode);
      printf("DIV \n");
      break;
    default:
      break;
  }
}