#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "helper_windows.h"


void page_printer();
void current_line_printer();
void checker_printer(char ch);
void rotate_to_right();
void tenWords_reader_fromFile();
void oneWord_adder_toStak();
char word_type(char str[]);
int isLong(char str[]);
int isHard(char str[]);
int is(char str[], char ch);
void obscureWords();
void wave();
int strcompar(char str1[], char str2[]);


FILE *fptr;
clock_t t;
char gamer_name[31]="Hossein";
int gamer_point=0;
char words_in_stak[10][21];
int typingWord_enteredCharacters = 0;
int typingWord_index_inStak = -1;
char file_words[10][21];
char typeOfWords[10]={'U', 'L', 'H'};/*U:usuall, L:long, H:hard*/
char obscure_words[3][21];
int counter_of_enteredWords = 0;
int counter_of_wordsInStack = 0;
char game_level;
float addingWord_toStack_time;
int lineNumber = 0;
int readed_lines_since_now = 0;


void my_callback_on_key_arrival(char c)
{
  system("cls");//befor there was printf("/e[1;1H/e[2J"); here
  checker_printer(c);

  if(strlen(words_in_stak[typingWord_index_inStak]) != 0
  && typingWord_enteredCharacters == strlen(words_in_stak[typingWord_index_inStak]))
  {
    counter_of_enteredWords++;
    if(counter_of_enteredWords == 10)
    {
      wave();
    }

    if(strcmp(obscure_words[0], words_in_stak[typingWord_index_inStak]))
    {
      gamer_point += 1;
    }
    if(strcmp(obscure_words[0], words_in_stak[typingWord_index_inStak]))
    {
      gamer_point += 1;
    }      
    if(strcmp(obscure_words[0], words_in_stak[typingWord_index_inStak]))
    {
      gamer_point += 1;
    }

    switch (word_type(words_in_stak[typingWord_index_inStak]))
    {
    case 'U':
      gamer_point += 1;
      break;
    case 'L':
      gamer_point += 2;
      break;
    case 'H':
      gamer_point += 3;
      break;

    default:
      break;
    }
    typingWord_index_inStak--;
    typingWord_enteredCharacters = 0;
  }

  if((float)(clock() - t)/CLOCKS_PER_SEC >= addingWord_toStack_time)
  {
    oneWord_adder_toStak();
    t = clock();
  }
  printf("processing time: (%f)", (float)(clock() - t)/CLOCKS_PER_SEC);

}


int main()
{
  printf("Welcome to the TYPE_GAME\n");
  printf("Choose level of the game by entering:\nE: easy level, M: medium level and H: hard level\n");
  scanf("%c", &game_level);
  switch (game_level)
  {
  case 'E':
    addingWord_toStack_time = 10;
    break;
  case 'M':
    addingWord_toStack_time = 8;
    break;
  case 'H':
    addingWord_toStack_time = 2;
    break;
  }

  fptr = fopen("C:/Users/yasbo/Desktop/University/FOP/Final Project/base_windows/words_file.txt", "r");
  int i;
  for(i=0; i<10; i++)
  {
      fgets(file_words[i], 21, fptr);
      file_words[i][strlen(file_words[i])-1] = '\0';
      readed_lines_since_now++;
  }
  fclose(fptr);

  oneWord_adder_toStak();
  obscureWords();

  page_printer();

  t = clock();//start calculating

  HANDLE thread_id = start_listening(my_callback_on_key_arrival);

  WaitForSingleObject(thread_id, INFINITE);
  return 0;
}
void checker_printer(char ch)
{
  if(words_in_stak[typingWord_index_inStak][typingWord_enteredCharacters] == ch)
  {
    typingWord_enteredCharacters++;
    page_printer();  
  }
  else 
    page_printer();
}
void page_printer()
{
  int i, j, numberOfSpaces;

  printf("######################\n");

  for(i=0 ; i<10; i++)
  {
    if(i<=typingWord_index_inStak)
    {
      if(i==typingWord_index_inStak)
      {
        current_line_printer();
        continue;
      }

      if(strcompar(words_in_stak[i], obscure_words[0]) && strlen(words_in_stak[i]) != 0)
      {
        printf("#********************#\n");
        continue;
      } 
      if(strcompar(words_in_stak[i], obscure_words[1]) && strlen(words_in_stak[i]) != 0)
      {
        printf("#********************#\n");
        continue;
      }
      if(strcompar(words_in_stak[i], obscure_words[2]) && strlen(words_in_stak[i]) != 0)
      {
        printf("#********************#\n");
        continue;
      }

      else
      {
        numberOfSpaces = 20-strlen(words_in_stak[i]);

        putchar('#');

        for(j=0; j<(numberOfSpaces/2); j++)
          putchar(' ');

        printf("%s", words_in_stak[i]);

        for(; j<numberOfSpaces; j++)
          putchar(' ');

        putchar('#');
        printf("\n"); 
      }
    }
    else
    {
      printf("#                    #\n");
    }
  }

  printf("######################\n");
  printf("your point: %d\n", gamer_point);
  printf("obscure words in this wave index: %s, %s, %s\n", obscure_words[0], obscure_words[1], obscure_words[2]);
  
}
void current_line_printer()
{
  int size_of_word = strlen(words_in_stak[typingWord_index_inStak]);
  int i, j, numberOfSpaces;

  numberOfSpaces = 20-size_of_word;
  putchar('#');
  
  for(j=0; j<(numberOfSpaces/2); j++)
    putchar(' ');

  printf("\033[0;31m");

  for(i=0; i<typingWord_enteredCharacters; i++)
    putchar(words_in_stak[typingWord_index_inStak][i]);

  printf("\033[0m");

  for(; i<size_of_word; i++)
    putchar(words_in_stak[typingWord_index_inStak][i]);

  for(; j<numberOfSpaces; j++)
    putchar(' ');

  putchar('#');
  printf("\n");
}

void rotate_to_right()
{
  int i;

  for(i=0; i<10-1; i++)
  {
      strcpy(file_words[i], file_words[i+1]);
  }
  strcpy(file_words[9], "");

  for(i=9-1; i>=0; i--)
  {
      strcpy(words_in_stak[i+1], words_in_stak[i]);
  }
  strcpy(words_in_stak[0], "");
}
void tenWords_reader_fromFile()
{
  
  fptr = fopen("C:/Users/yasbo/Desktop/University/FOP/Final Project/base_windows/words_file.txt", "r");
  int i;
  char temp[21];

  for(i=0; i<readed_lines_since_now; i++)
  {
    fgets(temp, 21, fptr);
  }
  for(i=0; i<10; i++)
  {
    fgets(file_words[i], 21, fptr);
    file_words[i][strlen(file_words[i])-1] = '\0';
    readed_lines_since_now++;
  }
  fclose(fptr);
}
void oneWord_adder_toStak()
{
  counter_of_wordsInStack++;
  typingWord_index_inStak++;

  char temp[21];
  strcpy(temp, file_words[0]);
  rotate_to_right();
  strcpy(words_in_stak[0], temp);

  if(counter_of_enteredWords == 10)
  {
    tenWords_reader_fromFile();
    counter_of_enteredWords = 0;
    typingWord_index_inStak = -1;
  }
}

char word_type(char str[])
{
  char type = 'U';
  if(strlen(str) == 0) type = '!';
  if(isLong(str)) type = 'L';
  if(isHard(str)) type = 'H';

  return type;
}
int isLong(char str[])
{
  if(strlen(str)>10) return 1;
  if(is(str, '_')) return 1;

  return 0;
}
int isHard(char str[])
{
  char characters[6] = {'@', '$', '%', '^', '&', '!'};

  int i;
  for(i=0; i<6; i++)
  {
    if(is(str, characters[i])) return 1;
  }

  return 0;
}
int is(char str[], char ch)
{
  int i;
  for(i=0; i<strlen(str); i++)
  {
    if(str[i] == ch) return 1;
  }

  return 0;
}

void obscureWords()
{
  char temp[21];

  srand(time(0));

  strcpy(obscure_words[0], file_words[rand()%10]);

  while(1)
  {
    strcpy(temp, file_words[rand()%10]);
    if(!strcmp(obscure_words[0], file_words[rand()%10] ))
    {
      strcpy(obscure_words[1], temp);
      break;
    }
  }

  while(1)
  {
    strcpy(temp, file_words[rand()%10]);
    if( !strcmp(obscure_words[0], file_words[rand()%10]) && !strcmp(obscure_words[1], file_words[rand()%10]))
    {
      strcpy(obscure_words[2], temp);
      break;
    }
  }    

}
void wave()
{
  printf("\a");
  obscureWords();
  switch (game_level)
  {
  case 'E':
    addingWord_toStack_time *= 0.8;
    break;
  case 'M':
    addingWord_toStack_time *= 0.7;
    break;
  case 'H':
    addingWord_toStack_time *= 0.5;
    break;  
  }
}

int strcompar(char str1[], char str2[])
{
    if(strlen(str1) != strlen(str2)) return 0;

    int i;
    for(i=0; i<strlen(str1); i++)
    {
        if(str1[i] != str2[i])
            return 0;
    }

    return 1;
}
