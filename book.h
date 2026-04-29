#ifndef BOOK_H
#define BOOK_H


#define MAX_SENTENCE_LINE 200
#define MAX_SENTENCE_COUNT 100

#define SPELL_HEAL 1
#define SPELL_ATTACK 2
#define SPELL_ABILITY 3

typedef struct {
  int sentence_count;
  char sentences[MAX_SENTENCE_COUNT][MAX_SENTENCE_LINE];
} Book;

void load_books();
void unload_books();

char * get_random_sentence(int spellType);

#endif