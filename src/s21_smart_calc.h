#ifndef S21_SMART_CALC_H_
#define S21_SMART_CALC_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char Data;
typedef struct Node Node;
struct Node {
  Data Data;
  Node *next;
};

typedef float calc_data;
typedef struct calc_node calc_node;
struct calc_node {
  calc_data Data;
  calc_node *next;
};

#define MOD 'm'
#define COS 'c'
#define SIN 's'
#define TAN 't'
#define ACOS 'a'
#define ASIN 'i'
#define ATAN 'n'
#define SQRT 'q'
#define LN 'l'
#define LOG 'o'

#define ERROR 404
#define NUMBER 1
#define OPERATION 2
#define OPEN_BRACKET 3
#define CLOSE_BRACKET 7
#define SEPARATOR 4
#define SPACE 8
#define POINT 9
#define LETTER 10
#define INPUT 11
#define O2 5
#define O1 6  //  приоретет О2 выше приоретета О1
#define OK 0

void push(Node *head, Data new_data);
void print_linked_list(Node head);
Data pop(Node *head);

calc_data calc_pop(calc_node *head);
void calc_push(calc_node *head, calc_data new_data);

int check_lexeme(char symbol);
void print_head(Node head);
int parse(Node *main_stack, char *str);
int calculate(Node *main_stack, float x, double *result);
void reverse_linked_list(Node *head);
int s21_smart_calc(char *str, float x, double *result);
void preparse(char *str, char *result);
int check_long_operations(char *str, int *i);

#endif  //  S21_SMART_CALC_H_
