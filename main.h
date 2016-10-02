//  Copyright 2016 Ryo Shinoki. Released under the MIT license.

#ifndef calculator_main_h
#define calculator_main_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

enum NodeType {
  OPERAND,
  OPERATOR_PLUS,
  OPERATOR_MINUS,
  OPERATOR_MULTI,
  OPERATOR_DIV,
};

typedef struct _Node {
  enum NodeType nodeType;
  int operandNum;
  struct _Node *leftNode;
  struct _Node *rightNode;
} Node;

//main.c
Node* read_expression(FILE *file);
Node* read_term(FILE *file);
Node* read_factor(FILE *file);
Node* read_num(FILE *file);
void compile_nodeTree(Node *tree);

//textController.c
int write_text(const char *formula, const char *name);
FILE* get_file_ptr(const char *fileName);


#endif
