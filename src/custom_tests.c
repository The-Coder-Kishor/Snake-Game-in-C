#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"
#include "custom_testcases.h"
// Necessary due to static functions in state.c
#include "state.c"

/* Look at asserts.c for some helpful assert functions */

int greater_than_forty_two(int x) {
  return x > 42;
}

bool is_vowel(char c) {
  char* vowels = "aeiouAEIOU";
  for (int i = 0; i < strlen(vowels); i++) {
    if (c == vowels[i]) {
      return true;
    }
  }
  return false;
}

/*
  Example 1: Returns true if all test cases pass. False otherwise.
    The function greater_than_forty_two(int x) will return true if x > 42. False otherwise.
    Note: This test is NOT comprehensive
*/
bool test_greater_than_forty_two() {
  int testcase_1 = 42;
  bool output_1 = greater_than_forty_two(testcase_1);
  if (!assert_false("output_1", output_1)) {
    return false;
  }

  int testcase_2 = -42;
  bool output_2 = greater_than_forty_two(testcase_2);
  if (!assert_false("output_2", output_2)) {
    return false;
  }

  int testcase_3 = 4242;
  bool output_3 = greater_than_forty_two(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  return true;
}

/*
  Example 2: Returns true if all test cases pass. False otherwise.
    The function is_vowel(char c) will return true if c is a vowel (i.e. c is a,e,i,o,u)
    and returns false otherwise
    Note: This test is NOT comprehensive
*/
bool test_is_vowel() {
  char testcase_1 = 'a';
  bool output_1 = is_vowel(testcase_1);
  if (!assert_true("output_1", output_1)) {
    return false;
  }

  char testcase_2 = 'e';
  bool output_2 = is_vowel(testcase_2);
  if (!assert_true("output_2", output_2)) {
    return false;
  }

  char testcase_3 = 'i';
  bool output_3 = is_vowel(testcase_3);
  if (!assert_true("output_3", output_3)) {
    return false;
  }

  char testcase_4 = 'o';
  bool output_4 = is_vowel(testcase_4);
  if (!assert_true("output_4", output_4)) {
    return false;
  }

  char testcase_5 = 'u';
  bool output_5 = is_vowel(testcase_5);
  if (!assert_true("output_5", output_5)) {
    return false;
  }

  char testcase_6 = 'k';
  bool output_6 = is_vowel(testcase_6);
  if (!assert_false("output_6", output_6)) {
    return false;
  }

  return true;
}

/* Task 4.1 */

bool test_is_tail() {
  for(int i = 0; i < 95; i++){
    if(i == 0 || i == 3 || i == 18 || i == 22){
      if(!assert_true("is_tail", is_tail(testcases[i]))){
        return false;
      }
    }
    else{
      if(!assert_false("is_tail", is_tail(testcases[i]))){
        return false;
      }
    }
  }
  printf("%s\n\n", "test_is_tail passed");
  return true;
}

bool test_is_head() {
  for(int i = 0; i < 95; i++){
    if(i == 23 || i == 26 || i == 29 || i == 44 || i == 48){
      if(!assert_true("is_head", is_head(testcases[i]))){
        return false;
      }
    }
    else{
      if(!assert_false("is_head", is_head(testcases[i]))){
        return false;
      }
    }
  }
  printf("%s\n\n", "test_is_head passed");
  return true;
}

bool test_is_snake() {
  for(int i = 0; i < 95; i++){
    if(i == 0 || i == 3 || i == 18 || i == 21 || i == 22 || i == 23 || i == 26 || i == 29 || i == 44 || i == 48 || i == 80 || i == 82 || i == 88){
      if(!assert_true("is_snake", is_snake(testcases[i]))){
        return false;
      }
    }
    else{
      if(!assert_false("is_snake", is_snake(testcases[i]))){
        return false;
      }
    }
  }
  printf("%s\n\n", "test_is_snake passed");
  return true;
}

bool test_body_to_tail() {
  for(int i = 0; i < 95; i++){
    if(i == 21){
      if('s' != body_to_tail(testcases[i])){
        return false;
      }
    }
    else if(i == 80){
      if('a' != body_to_tail(testcases[i])){
        return false;
      }
    }
    else if(i == 82){
      if('d' != body_to_tail(testcases[i])){
        return false;
      }
    }
    else if(i == 88){
      if('w' != body_to_tail(testcases[i])){
        return false;
      }
    }
    else{
      if('?' != body_to_tail(testcases[i])){
        return false;
      }
    }
  }
  printf("%s\n\n", "test_body_to_tail passed");
  return true;
}

bool test_head_to_body() {
  for(int i = 0; i < 95; i++){
    if(i == 26){
      if('<' != head_to_body(testcases[i])){
        return false;
      }
    }
    else if(i == 29){
      if('>' != head_to_body(testcases[i])){
        return false;
      }
    }
    else if(i == 44){
      if('v' != head_to_body(testcases[i])){
        return false;
      }
    }
    else if(i == 48){
      if('^' != head_to_body(testcases[i])){
        return false;
      }
    }
    else{
      if('?' != head_to_body(testcases[i])){
        return false;
      }
    }
  }
  printf("%s\n\n", "test_head_to_body passed");
  return true;
}

bool test_get_next_row() {
  unsigned int row = 3;
  for(int i = 0; i < 95; i++){
    if(i == 18 || i == 21 || i == 44){
      if(4 != get_next_row(row, testcases[i])){
        return false;
      }
    }
    else if(i == 22 || i == 48 || i == 88){
      if(2 != get_next_row(row, testcases[i])){
        return false;
      }
    }
    else{
      if(3 != get_next_row(row, testcases[i])){
        return false;
      }
    }
  }
  printf("%s\n\n", "test_get_next_row passed");
  return true;
}

bool test_get_next_col() {
  unsigned int col = 3;
  for(int i = 0; i < 95; i++){
    if(i == 0 || i == 26 || i == 80){
      if(2 != get_next_col(col, testcases[i])){
        return false;
      }
    }
    else if(i == 3 || i == 29 || i == 82){
      if(4 != get_next_col(col, testcases[i])){
        return false;
      }
    }
    else{
      if(3 != get_next_col(col, testcases[i])){
        return false;
      }
    }
  }
  printf("%s\n\n", "test_get_next_col passed");
  return true;
}

bool test_customs() {
  if (!test_greater_than_forty_two()) {
    printf("%s\n", "test_greater_than_forty_two has failed.");
    return false;
  }
  if (!test_is_vowel()) {
    printf("%s\n", "test_is_vowel has failed.");
    return false;
  }
  if (!test_is_tail()) {
    printf("%s\n", "test_is_tail has failed");
    return false;
  }
  if (!test_is_head()) {
    printf("%s\n", "test_is_head has failed");
    return false;
  }
  if (!test_is_snake()) {
    printf("%s\n", "test_is_snake has failed");
    return false;
  }
  if (!test_body_to_tail()) {
    printf("%s\n", "test_body_to_tail has failed");
    return false;
  }
  if (!test_head_to_body()) {
    printf("%s\n", "test_head_to_body has failed");
    return false;
  }
  if (!test_get_next_row()) {
    printf("%s\n", "test_get_next_row has failed");
    return false;
  }
  if (!test_get_next_col()) {
    printf("%s\n", "test_get_next_col has failed");
    return false;
  }
  return true;
}

int main(int argc, char* argv[]) {
  init_colors();

  if (!test_and_print("custom", test_customs)) {
    return 0;
  }

  return 0;
}
