#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);

/* Task 1 */
game_state_t* create_default_state() {
  //Game board details
  game_state_t* default_game_state = (game_state_t*)malloc(sizeof(game_state_t));
  default_game_state->num_rows = 18;
  default_game_state->board = (char**)malloc(sizeof(char*) * default_game_state->num_rows);
  for(int i = 0; i < default_game_state->num_rows; i++)
  {
    default_game_state->board[i] = (char*)malloc(sizeof(char) * 21);
  }
  //21 to include \0 character
  char default_board[18][21] = {
    "####################",
    "#                  #",
    "# d>D    *         #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "#                  #",
    "####################"
  };
  for(int i = 0; i < default_game_state->num_rows; i++)
  {
    //copies from the default board to the game board
    strcpy(default_game_state->board[i], default_board[i]);
  }
  //Snake details
  default_game_state->num_snakes = 1;
  default_game_state->snakes = (snake_t*)malloc(sizeof(snake_t));

  //Snake details
  default_game_state->snakes->tail_row = 2;
  default_game_state->snakes->tail_col = 2;
  default_game_state->snakes->head_row = 2;
  default_game_state->snakes->head_col = 4;
  default_game_state->snakes->live = true;
  
  return default_game_state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  //Freeing each row of the board
  for(int i = 0; i < state->num_rows; i++)
  {
    free(state->board[i]);
  }
  //Freeing the board
  free(state->board);
  //Freeing the snakes
  free(state->snakes);
  //Freeing the state
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  //if file is NULL, return
  if(fp == NULL){
    return;
  }
  //prints each row of the board
  for(int i = 0; i < state->num_rows; i++){
    fprintf(fp, "%s\n", state->board[i]);
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  switch(c){
    case 'w':
    case 'a':
    case 's':
    case 'd':
      return true;
    default:
      return false;
  }
  return false;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  switch(c){
    case 'W':
    case 'A':
    case 'S':
    case 'D':
    case 'x':
      return true;
    default:
      return false;
  }
  return false;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  switch(c){
    case 'w':
    case 'a':
    case 's':
    case 'd':
    case '^':
    case '<':
    case '>':
    case 'v':
    case 'W':
    case 'A':
    case 'S':
    case 'D':
    case 'x':
      return true;
    default:
      return false;
  }
  return false;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  switch(c){
    case '^':
      return 'w';
    case '<':
      return 'a';
    case 'v':
      return 's';
    case '>':
      return 'd';
    default:
      return '?';
  }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  switch(c){
    case 'W':
      return '^';
    case 'A':
      return '<';
    case 'S':
      return 'v';
    case 'D':
      return '>';
    default:
      return '?';
  }
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  switch(c){
    case 'v':
    case 's':
    case 'S':
      return cur_row + 1;
    case '^':
    case 'w':
    case 'W':
      return cur_row - 1;
    default:
      return cur_row;
  }
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  switch(c){
    case '>':
    case 'd':
    case 'D':
      return cur_col + 1;
    case '<':
    case 'a':
    case 'A':
      return cur_col - 1;
    default:
      return cur_col;
  }
  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  unsigned int head_row = state->snakes[snum].head_row;
  unsigned int head_col = state->snakes[snum].head_col;
  char head = get_board_at(state, head_row, head_col);
  switch(head){
    case 'W':
      return get_board_at(state, head_row - 1, head_col);
    case 'A':
      return get_board_at(state, head_row, head_col - 1);
    case 'S':
      return get_board_at(state, head_row + 1, head_col);
    case 'D':
      return get_board_at(state, head_row, head_col + 1);
  }
  return '?';
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
  unsigned int head_row = state->snakes[snum].head_row;
  unsigned int head_col = state->snakes[snum].head_col;
  char head = get_board_at(state, head_row, head_col);
  unsigned int new_row = get_next_row(head_row,head);
  unsigned int new_col = get_next_col(head_col,head);
  set_board_at(state, new_row, new_col, head);
  set_board_at(state,head_row, head_col, head_to_body(head));
  state->snakes[snum].head_row = new_row;
  state->snakes[snum].head_col = new_col;
  return;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  unsigned tail_row = state->snakes[snum].tail_row;
  unsigned int tail_col = state->snakes[snum].tail_col;
  char tail = get_board_at(state, tail_row, tail_col);
  unsigned int new_row = get_next_row(tail_row,tail);
  unsigned int new_col = get_next_col(tail_col,tail);
  char body = get_board_at(state, new_row, new_col);
  set_board_at(state, new_row, new_col, body_to_tail(body));
  set_board_at(state,tail_row, tail_col, ' ');
  state->snakes[snum].tail_row = new_row;
  state->snakes[snum].tail_col = new_col;
  return;
}

/*
update_snake helper function to help in update_state
Added because conditions for two special cases were becoming too complex to handle in update_snake:
1. Two snakes moving into one square.
2. One snake which was heading into the other snake's tail which can cause errors in certain scenarios.
*/
void update_snake(game_state_t* state, int (*add_food)(game_state_t* state), unsigned int i, int* updated){
  if(updated[i] == 1){
    return;
  }

  if(!state->snakes[i].live){
    return;
  }
  char next = next_square(state, i);

  unsigned int next_row = get_next_row(state->snakes[i].head_row, next);
  unsigned int next_col = get_next_col(state->snakes[i].head_col, next);

  if(is_head(next)){
    for(unsigned int j = i+1; j < state->num_snakes; j++){
      if(j!=i && !updated[j] && !state->snakes[j].live){
        unsigned int next_row2 = get_next_row(state->snakes[j].head_row, next);
        unsigned int next_col2 = get_next_col(state->snakes[j].head_col, next);
        if(next_col == next_col2 && next_row == next_row2){
          set_board_at(state, state->snakes[i].head_row, state->snakes[i].head_col, 'x');
          set_board_at(state, state->snakes[j].head_row, state->snakes[j].head_col, 'x');
          state->snakes[i].live = false;
          state->snakes[j].live = false;
          updated[i] = 1;
          updated[j] = 1;
          return;
        }          
      }
    }
  }

  if(is_tail(next)){
    for(unsigned int j = i+1; j < state->num_snakes; j++){
      if(i!=j && !updated[j] && !state->snakes[j].live){
        if(next_col == state->snakes[j].tail_col && next_row == state->snakes[j].tail_row){
          update_snake(state, add_food, j, updated);
          update_snake(state, add_food, i, updated);
          return;
        }          
      }
    }
  }

  if(next == ' '){
    update_head(state,i);
    update_tail(state,i);
  }
  else if(next == '*'){
    update_head(state,i);
    add_food(state);
  }
  else if(next == '#' || is_snake(next)){
    set_board_at(state, state->snakes[i].head_row, state->snakes[i].head_col, 'x');
    state->snakes[i].live = false;
  }
  updated[i] = 1;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  int updated[state->num_snakes];
  for(unsigned int i = 0; i < state->num_snakes; i++){
    updated[i] = 0;
  }
  for(unsigned int i = 0; i < state->num_snakes; i++){
    update_snake(state, add_food, i, updated);
  }
  return;
}

/* Task 5 */
game_state_t* load_board(FILE* fp) {
  if (fp == NULL) {
      return NULL;
  }

  game_state_t* loaded_game_state = (game_state_t*)malloc(sizeof(game_state_t));
  loaded_game_state->num_snakes = 0;
  loaded_game_state->snakes = NULL;

  int inp;

  unsigned int rows = 0;
  unsigned int cols = 0;

  loaded_game_state->board = (char**)malloc(sizeof(char*) * (rows + 1));
  loaded_game_state->board[rows] = (char*)malloc(sizeof(char) * (cols + 1));

  while((inp = fgetc(fp)) != EOF){
    char ch = (char)inp;
    loaded_game_state->board[rows] = (char*)realloc(loaded_game_state->board[rows], sizeof(char) * ++cols);
    set_board_at(loaded_game_state, rows, cols - 1, ch);
    if(ch == '\n'){
      set_board_at(loaded_game_state, rows, cols - 1, '\0');
      cols = 0;
      loaded_game_state->board = (char**)realloc(loaded_game_state->board, sizeof(char*) * (++rows + 1));
      loaded_game_state->board[rows] = (char*)malloc(sizeof(char) * (cols + 1));
    }
  }
  loaded_game_state->num_rows = rows;
  return loaded_game_state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  if(state == NULL){
    return;
  }
  unsigned int tail_row = state->snakes[snum].tail_row;
  unsigned int tail_col = state->snakes[snum].tail_col;
  char curr = get_board_at(state, tail_row, tail_col);
  unsigned int new_row = tail_row;
  unsigned int new_col = tail_col;
  while(!is_head(get_board_at(state, new_row, new_col))){
    curr = get_board_at(state, new_row, new_col);
    new_row = get_next_row(new_row,curr);
    new_col = get_next_col(new_col,curr);
  }
  state->snakes[snum].head_row = new_row;
  state->snakes[snum].head_col = new_col;
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  if(state == NULL){
    return NULL;
  }
  unsigned int snakes = 0;
  state->snakes = (snake_t*)malloc(sizeof(snake_t));
  for(unsigned int i = 0; i < state->num_rows; i++){
    for(unsigned int j = 0; j < strlen(state->board[i]); j++){
      if(is_tail(get_board_at(state, i, j))){
        snakes++;
        if(snakes > 1)
          state->snakes = (snake_t*)realloc(state->snakes, sizeof(snake_t) * snakes);
        state->snakes[snakes - 1].tail_row = i;
        state->snakes[snakes - 1].tail_col = j;
        state->snakes[snakes - 1].live = true;
        find_head(state, snakes - 1);
      }
    }
  }
  state->num_snakes = snakes;
  return state;
}