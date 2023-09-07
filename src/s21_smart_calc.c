#include "s21_smart_calc.h"

void preparse(char *str, char *res) {
  int len = strlen(str);
  char result[256];
  int i_result = 0;
  for (int i = 0; i < len; ++i) {
    if (str[i] == '(' &&
        (i + 2 < len && (str[i + 2] == '+' || str[i + 2] == '-'))) {
      result[i_result] = str[i];
      ++i_result;
      result[i_result] = '0';
      ++i_result;
    } else if (i == 0 && i + 1 < len &&
               (str[i + 1] == '+' || str[i + 1] == '-')) {
      result[i_result] = '0';
      ++i_result;
      result[i_result] = ' ';
      ++i_result;
    } else {
      result[i_result] = str[i];
      ++i_result;
    }
  }
  result[i_result] = '\0';
  strcpy(res, result);
}

int s21_smart_calc(char *str, float x, double *result) {
  int ExitCode = OK;
  Node main_stack;
  main_stack.next = NULL;
  preparse(str, str);
  if (parse(&main_stack, str)) {
    ExitCode = ERROR;
  } else {
    reverse_linked_list(&main_stack);
    ExitCode = calculate(&main_stack, x, result);
  }
  return ExitCode;
}

void print_head(Node head) {
  while (head.next != NULL) {
    printf("%d\n", check_lexeme(pop(&head)));
  }
}

void print_linked_list(Node head) {
  while (head.next != NULL) {
    printf("%c\n", head.next->Data);
    head.next = head.next->next;
  }
}

void push(Node *head, Data new_data) {
  Node *new = (Node *)malloc(sizeof(Node));
  new->Data = new_data;
  new->next = head->next;
  head->next = new;
}

Data pop(Node *head) {
  Data ExitCode = head->next->Data;
  Node *temp = head->next->next;
  free(head->next);
  head->next = temp;
  return ExitCode;
}

int check_lexeme(char symbol) {
  int ExitCode = ERROR;
  char operations[9] = "()/+-*^m";  // mcstasl
  if (symbol >= '0' && symbol <= '9') {
    ExitCode = NUMBER;
  } else if (symbol == '(') {
    ExitCode = OPEN_BRACKET;
  } else if (symbol == ')') {
    ExitCode = CLOSE_BRACKET;
  } else if (strchr(operations, symbol) != NULL) {
    ExitCode = OPERATION;
  } else if (symbol == ' ') {
    ExitCode = SPACE;
  } else if (symbol == '.') {
    ExitCode = POINT;
  } else if (symbol >= 'a' && symbol <= 'z') {
    ExitCode = LETTER;
  }
  return ExitCode;
}

int operation_priority(char symbol) {
  int ExitCode = -1;
  char zero_priority[10] = "cstainqlo";
  char first_priority[2] = "^";
  char second_priority[4] = "*/m";
  char third_priority[3] = "+-";
  if (strchr(zero_priority, symbol) != NULL) {
    ExitCode = 0;
  } else if (strchr(first_priority, symbol) != NULL) {
    ExitCode = 1;
  } else if (strchr(second_priority, symbol) != NULL) {
    ExitCode = 2;
  } else if (strchr(third_priority, symbol) != NULL) {
    ExitCode = 3;
  }
  return ExitCode;
}

int separator(Node *temp_stack, Node *main_stack);

int check_long_operations(char *str, int *i) {
  int ExitCode = ERROR;
  if (str[*i] == 'm') {
    ExitCode = MOD;
    *i += 2;
  } else if (str[*i] == 'c') {
    ExitCode = COS;
    *i += 2;
  } else if (str[*i] == 's') {
    if (str[*i + 1] == 'i') {
      ExitCode = SIN;
      *i += 2;
    } else if (str[*i + 1] == 'q') {
      ExitCode = SQRT;
      *i += 3;
    }
  } else if (str[*i] == 't') {
    ExitCode = TAN;
    *i += 2;
  } else if (str[*i] == 'a') {
    if (str[*i + 1] == 'c') {
      ExitCode = ACOS;
    } else if (str[*i + 1] == 's') {
      ExitCode = ASIN;
    } else if (str[*i + 1] == 't') {
      ExitCode = ATAN;
    }
    *i += 3;
  } else if (str[*i] == 'l') {
    if (str[*i + 1] == 'n') {
      ExitCode = LN;
      *i += 1;
    } else if (str[*i + 1] == 'o') {
      ExitCode = LOG;
      *i += 2;
    }
  }
  return ExitCode;
}

int parse(Node *main_stack, char *str) {
  int ExitCode = OK;
  Node temp_stack;
  temp_stack.next = NULL;
  int len_str = strlen(str);
  for (int i = 0; i < len_str; ++i) {
    int check_lexeme_i = check_lexeme(str[i]);
    if (check_lexeme_i == NUMBER || str[i] == 'x') {
      push(main_stack, str[i]);
      if (i + 1 < len_str && str[i + 1] == ' ') {
        push(main_stack, str[i + 1]);
        ++i;
      }
    } else if (check_lexeme_i == OPEN_BRACKET || check_lexeme_i == LETTER) {
      if (check_lexeme_i == OPEN_BRACKET) {
        push(&temp_stack, str[i]);
      } else {
        int operation_code = check_long_operations(str, &i);
        push(&temp_stack, operation_code);
      }
    } else if (check_lexeme_i == SEPARATOR) {
      // разделитиль аргументов функции (например, запятая)
      ExitCode = separator(&temp_stack, main_stack);
    } else if (check_lexeme_i == OPERATION || check_lexeme_i == MOD) {
      if (check_lexeme_i == OPERATION) {
        while (temp_stack.next != NULL &&
               (check_lexeme(temp_stack.next->Data) == OPERATION ||
                check_lexeme(temp_stack.next->Data) == LETTER) &&
               operation_priority(temp_stack.next->Data) <=
                   operation_priority(str[i])) {
          push(main_stack, pop(&temp_stack));
        }
        push(&temp_stack, str[i]);
      }
    } else if (check_lexeme_i == CLOSE_BRACKET) {
      ExitCode = separator(&temp_stack, main_stack);
      if (!ExitCode) {
        pop(&temp_stack);
        //  если ф-я, то перекладываем её в выходную очередь
      }
    } else if (check_lexeme_i == POINT) {
      push(main_stack, str[i]);
    }
  }
  while (temp_stack.next != NULL) {
    if (temp_stack.next->Data == OPEN_BRACKET ||
        temp_stack.next->Data == CLOSE_BRACKET) {
      ExitCode = ERROR;
    } else {
      push(main_stack, pop(&temp_stack));
    }
  }
  return ExitCode;
}

int separator(Node *temp_stack, Node *main_stack) {
  int ExitCode = OK;
  int flag = 0;
  while (temp_stack->next != NULL && !flag) {
    if (temp_stack->next->Data == '(') {
      flag = 1;
    } else {
      push(main_stack, pop(temp_stack));
    }
  }
  if (!flag) {
    ExitCode = ERROR;
  }
  return ExitCode;
}

int calculate(Node *main_stack, float x, double *result) {
  int ExitCode = OK;
  calc_node temp_stack;
  temp_stack.next = NULL;
  Data temp_data;

  while (main_stack->next != NULL && !ExitCode) {
    temp_data = pop(main_stack);
    if (temp_data == 'x') {
      calc_push(&temp_stack, x);
    } else if (check_lexeme(temp_data) == NUMBER) {
      int counter = 0;
      int counter_flag = 0;
      if (main_stack->next != NULL &&
          (check_lexeme(main_stack->next->Data) == NUMBER ||
           check_lexeme(main_stack->next->Data) == POINT)) {
        float temp_float = 0.0;
        temp_float = (float)(temp_data - '0');
        temp_data = pop(main_stack);
        int flag = 1;

        while (temp_data != ' ' && flag && !ExitCode) {
          if (temp_data == '.') {
            if (main_stack->next != NULL) {
              temp_data = pop(main_stack);
              counter_flag = 1;
            } else {
              ExitCode = ERROR;
            }
          }
          if (!ExitCode) {
            temp_float *= 10;
            temp_float += (float)(temp_data - '0');
            if (main_stack->next != NULL) {
              temp_data = pop(main_stack);
            } else {
              ExitCode = INPUT;
            }
            if (counter_flag) {
              ++counter;
            }
            if (check_lexeme(temp_data) != NUMBER &&
                check_lexeme(temp_data) != POINT) {
              push(main_stack, temp_data);
              flag = 0;
            }
          }
        }
        if (ExitCode == INPUT) {
          ExitCode = OK;
        }
        if (counter_flag) {
          calc_push(&temp_stack, temp_float / pow(10, counter));
        } else {
          calc_push(&temp_stack, temp_float);
        }
      } else {
        calc_push(&temp_stack, temp_data - '0');
      }
    } else if (check_lexeme(temp_data) == OPERATION) {
      if (temp_data == '+') {
        if (temp_stack.next == NULL || temp_stack.next->next == NULL) {
          ExitCode = ERROR;
        } else {
          calc_data second = calc_pop(&temp_stack);
          calc_data first = calc_pop(&temp_stack);
          calc_push(&temp_stack, first + second);
        }
      } else if (temp_data == '-') {
        if (temp_stack.next == NULL || temp_stack.next->next == NULL) {
          ExitCode = ERROR;
        } else {
          calc_data second = calc_pop(&temp_stack);
          calc_data first = calc_pop(&temp_stack);
          calc_push(&temp_stack, first - second);
        }
      } else if (temp_data == '*') {
        if (temp_stack.next == NULL || temp_stack.next->next == NULL) {
          ExitCode = ERROR;
        } else {
          calc_data second = calc_pop(&temp_stack);
          calc_data first = calc_pop(&temp_stack);
          calc_push(&temp_stack, first * second);
        }
      } else if (temp_data == '/') {
        if (temp_stack.next == NULL || temp_stack.next->next == NULL) {
          ExitCode = ERROR;
        } else {
          calc_data second = calc_pop(&temp_stack);
          calc_data first = calc_pop(&temp_stack);
          if (second == 0.0) {
            ExitCode = ERROR;
          } else {
            calc_push(&temp_stack, first / second);
          }
        }
      } else if (temp_data == '^') {
        if (temp_stack.next == NULL || temp_stack.next->next == NULL) {
          ExitCode = ERROR;
        } else {
          calc_data second = calc_pop(&temp_stack);
          calc_data first = calc_pop(&temp_stack);
          calc_push(&temp_stack, pow(first, second));
        }
      } else if (temp_data == MOD) {
        if (temp_stack.next == NULL || temp_stack.next->next == NULL) {
          ExitCode = ERROR;
        } else {
          int second = (int)(calc_pop(&temp_stack));
          int first = (int)(calc_pop(&temp_stack));
          calc_push(&temp_stack, first % second);
        }
      }
    } else if (check_lexeme(temp_data) == LETTER) {
      if (temp_data == COS) {
        if (temp_stack.next != NULL) {
          double first = calc_pop(&temp_stack);
          calc_push(&temp_stack, cos(first));
        } else {
          ExitCode = ERROR;
        }
      } else if (temp_data == SIN) {
        if (temp_stack.next != NULL) {
          double first = calc_pop(&temp_stack);
          calc_push(&temp_stack, sin(first));
        } else {
          ExitCode = ERROR;
        }
      } else if (temp_data == TAN) {
        if (temp_stack.next != NULL) {
          double first = calc_pop(&temp_stack);
          calc_push(&temp_stack, tan(first));
        } else {
          ExitCode = ERROR;
        }
      } else if (temp_data == ACOS) {
        if (temp_stack.next != NULL) {
          double first = calc_pop(&temp_stack);
          calc_push(&temp_stack, acos(first));
        } else {
          ExitCode = ERROR;
        }
      } else if (temp_data == ASIN) {
        if (temp_stack.next != NULL) {
          double first = calc_pop(&temp_stack);
          calc_push(&temp_stack, asin(first));
        } else {
          ExitCode = ERROR;
        }
      } else if (temp_data == ATAN) {
        if (temp_stack.next != NULL) {
          double first = calc_pop(&temp_stack);
          calc_push(&temp_stack, atan(first));
        } else {
          ExitCode = ERROR;
        }
      } else if (temp_data == SQRT) {
        if (temp_stack.next != NULL) {
          double first = calc_pop(&temp_stack);
          calc_push(&temp_stack, sqrt(first));
        } else {
          ExitCode = ERROR;
        }
      } else if (temp_data == LN) {
        if (temp_stack.next != NULL) {
          double first = calc_pop(&temp_stack);
          calc_push(&temp_stack, log(first));
        } else {
          ExitCode = ERROR;
        }
      } else if (temp_data == LOG) {
        if (temp_stack.next != NULL) {
          double first = calc_pop(&temp_stack);
          calc_push(&temp_stack, log10(first));
        } else {
          ExitCode = ERROR;
        }
      }

    } else if (check_lexeme(temp_data) == POINT) {
      ExitCode = ERROR;
    }
  }
  if (!ExitCode) {
    *result = temp_stack.next->Data;
  }
  return ExitCode;
}

void reverse_linked_list(Node *head) {
  Node temp_head;
  temp_head.next = NULL;
  while (head->next != NULL) {
    push(&temp_head, pop(head));
  }
  *head = temp_head;
}

void calc_push(calc_node *head, calc_data new_data) {
  calc_node *new = (calc_node *)malloc(sizeof(calc_node));
  new->Data = new_data;
  new->next = head->next;
  head->next = new;
}

calc_data calc_pop(calc_node *head) {
  calc_data ExitCode = head->next->Data;
  calc_node *temp = head->next->next;
  free(head->next);
  head->next = temp;
  return ExitCode;
}
