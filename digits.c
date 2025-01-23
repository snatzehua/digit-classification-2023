/**
 * CS1010 Semester 1 AY22/23
 * Assignment 8: Digits
 *
 * @file: digits.c
 * @author: Kieran Ho (Group D06)
 */

#include "cs1010.h"

#define NCOLS 28
#define NROWS 28
#define WHITE '.'
#define BLACK '#'
#define K 5
  
typedef struct digit { 
  long num;
  char **array;
} digit;


typedef struct neighbor {
  long num;
  long distance;
} neighbor;
 
long get_diff(char **testing, char **array) {

  long total = 0;
  for (size_t i = 0 ; i < 28 ; i += 1) {
    for (size_t j = 0 ; j < 28 ; j += 1) {
      if (testing[i][j] != array[i][j]) {
        total += 1;
      }
    }
  }
  return total;
}

void shift_values(neighbor *k_array, size_t pos, long val, long digit) {
  
  for (size_t i = 4 ; i > pos ; i -= 1) {
    k_array[i].num = k_array[i - 1].num;
    k_array[i].distance = k_array[i - 1].distance;
  }
  k_array[pos].num = digit;
  k_array[pos].distance = val;
}

void test(neighbor *k_array, char **testing, digit *digit_list, size_t n) {
  
  for (size_t i = 0 ; i < n ; i += 1) {
    long total = get_diff(testing, digit_list[i].array);
    for (size_t j = 0 ; j < 5 ; j += 1) {
      if (total < k_array[j].distance || 
          (total == k_array[j].distance && digit_list[i].num < k_array[j].num)) {
        shift_values(k_array, j, total, digit_list[i].num);
        break;
      }
    }
  }
}

long tie_break(neighbor *k_array, long a, long b) {

  for (size_t i = 0 ; i < 5 ; i += 1) {
    if (k_array[i].num == a || k_array[i].num == b) {
      return k_array[i].num;
    }
  }
  return k_array[0].num;
}

long get_k_final(neighbor *k_array) {

  long max[10] = {0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0};
  for (size_t i = 0 ; i < 5 ; i += 1) {
    max[k_array[i].num] += 1;
  }
  long max_d = -1;
  long max_v = -1;
  long alt_d = -1;
  for (long i = 0 ; i < 10 ; i += 1) {
    if (max[i] > max_v) {
      max_d = i;
      max_v = max[i];
    }
    if (max_v == 2 && max[i] == 2) {
      alt_d = i;
    }
    if (max_v >= 3) {
      return max_d;
    }
  }
  if (max_v == 1) {
    return k_array[0].num;
  }
  if (max_v == 2 && alt_d > 0) {
    return tie_break(k_array, max_d, alt_d);
  }
  return max_d;
}

void free_mem(digit *digit_list, size_t n) {

  for (size_t i = 0 ; i < n ; i += 1) {
    for (size_t j = 0 ; j < 28 ; j += 1) {
      free(digit_list[i].array[j]);
    }
    free(digit_list[i].array);
  }
  free(digit_list);
}

int main() {
  
  size_t n = cs1010_read_size_t();
  digit *digit_list = calloc(n, sizeof (digit));

  for (size_t i = 0 ; i < n ; i += 1) {
    digit_list[i].num = cs1010_read_long();
    digit_list[i].array = cs1010_read_word_array(28);;
  }
  
  size_t m = cs1010_read_size_t();
  
  neighbor filler = {
    .num = 0,
    .distance = 784,
  };
  neighbor k_array[5] = {filler, filler, filler, filler, filler};
  long correct = 0;
  for (size_t i = 0 ; i < m ; i += 1) {
    long number = cs1010_read_long();
    char **testing = cs1010_read_word_array(28);
    test(k_array, testing, digit_list, n);
    cs1010_print_long(number);
    putchar(' ');
    long k_final = get_k_final(k_array);
    if (number == k_final) {
      correct += 1;
    }
    cs1010_println_long(k_final);
    for (size_t j = 0 ; j < 5 ; j += 1) {
      k_array[j] = filler;
    }
    for (size_t k = 0 ; k < 28 ; k += 1) {
      free(testing[k]);
    }
    free(testing);
  }
  cs1010_println_double(100 * (float) correct / (float) m);
  free_mem(digit_list, n);
}

