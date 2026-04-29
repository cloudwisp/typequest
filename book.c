#include <stdio.h>
#include <string.h>
#include <alloc.h>
#include "book.h"


Book * heal_book;
Book * ability_book;
Book * attack_book;
char temporary_sentence[MAX_SENTENCE_LINE];

int thisIsNotRandom = 0;

Book * load_book(const char * path){
  Book * newBook = NULL;
  FILE *fp = fopen(path, "r");
  char line[MAX_SENTENCE_LINE];


  if (!fp) {
    printf("Error opening file: %s\n", path);
    return;
  }
  
  newBook = (Book *)farmalloc(sizeof(Book));
  newBook->sentence_count = 0;

  while (fgets(line, MAX_SENTENCE_LINE, fp)) {
      
    _fstrcpy(newBook->sentences[newBook->sentence_count], line);
    //*(newBook->sentences)[254] = "\n"; //safety, is there a better way to do this?
    newBook->sentence_count++;
  }

  fclose(fp);
  return newBook;
}

void load_books(){

  heal_book = load_book("heal.txt");
  ability_book = load_book("ability.txt");
  attack_book = load_book("attack.txt");

}

void unload_books(){

  farfree(heal_book);
  farfree(ability_book);
  farfree(attack_book);

}

char * get_random_sentence_book(Book * book){
  //TODO: Make it random
  if (thisIsNotRandom >= book->sentence_count){
    thisIsNotRandom = 0;
  }
  thisIsNotRandom++;
  _fstrcpy(temporary_sentence, book->sentences[thisIsNotRandom - 1]);
  return temporary_sentence;
}

char * get_random_sentence(int spellType){
  switch (spellType){
    case SPELL_HEAL:
      return get_random_sentence_book(heal_book);
    case SPELL_ATTACK:
      return get_random_sentence_book(attack_book);
    case SPELL_ABILITY:
      return get_random_sentence_book(ability_book);
  }
  return "";
}