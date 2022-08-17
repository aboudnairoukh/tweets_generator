#include "markov_chain.h"
#include <string.h>

#define SEED_ARG (argv[1])
#define NUM_OF_TWEETS_TO_MAKE (argv[2])
#define FILE_PATH (argv[3])
#define NUM_OF_WORDS_TO_READ (argv[4])
#define FOUR_ARGS 5
#define THREE_ARGS 4
#define DECIMAL_BASE 10
#define BUFFER_SIZE 1002
#define READ_ALL_FILE_STR "-1"
#define READ_ALL_FILE_INT -1
#define MAX_WORDS_LENGTH 20
#define SPACE_STR " "
#define NEWLINE_STR "\n"

/**
 * This function fills the given markov chain with the data from the given fp
 * @param fp the file that contains the data
 * @param words_to_read num of words to read
 * @param markov_chain the given markov chain
 * @return 0 if success
 */
int fill_database (FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
  bool continue_reading = (words_to_read != 0) ? true : false;
  char input[BUFFER_SIZE], *token = NULL;
  Node *new = NULL, *new_next = NULL;
  while (fgets (input, BUFFER_SIZE, fp) != NULL && continue_reading == true)
  {
    char *input_without_newline = strtok (input, NEWLINE_STR);
    token = strtok (input_without_newline, SPACE_STR);
    words_to_read = (words_to_read == READ_ALL_FILE_INT) ? words_to_read :
                    words_to_read - 1;
    if (token == NULL)
    {
      continue;
    }
    new = add_to_database (markov_chain, token);
    if (new == NULL)
    {
      return EXIT_FAILURE;
    }
    token = strtok (NULL, SPACE_STR);
    words_to_read = (words_to_read == READ_ALL_FILE_INT) ? words_to_read :
                    words_to_read - 1;
    while (token != NULL && continue_reading == true)
    {
      new_next = add_to_database (markov_chain, token);
      if (new_next == NULL
          || add_node_to_counter_list (new->data, new_next->data) == false)
      {
        return EXIT_FAILURE;
      }
      token = strtok (NULL, SPACE_STR);
      new = new_next;
      if (words_to_read != READ_ALL_FILE_INT)
      {
        words_to_read--;
        continue_reading = (words_to_read > 0) ? true : false;
        if (continue_reading == false)
        {
          new_next = add_to_database (markov_chain, token);
          if (add_node_to_counter_list (new->data, new_next->data) == false)
          {
            return EXIT_FAILURE;
          }
        }
      }
    }
  }
  return EXIT_SUCCESS;
}

/**
 * This function updates the seed with the given seed
 * @param str_seed given seed
 */
void get_and_update_seed (char *str_seed)
{
  int seed = (int) strtol (str_seed, NULL, DECIMAL_BASE);
  srand (seed);
}

/**
 * This functions checks if the number of the given args is valid
 * @return 0 if valid
 */
int check_num_of_args (int argc)
{
  if (argc != FOUR_ARGS && argc != THREE_ARGS)
  {
    printf ("USAGE: THE program receives 3 or 4 arguments");
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

/**
 * This function creates a markov chain and fills it
 * @param words_to_read_str num of words to read ("-1" if all file)
 * @param in_file the file to read
 * @return the markov chain, NULL if failure
 */
MarkovChain *fill_data (char *words_to_read_str, FILE *in_file)
{
  MarkovChain *markov_chain = new_markov_chain ();
  if (markov_chain == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  int words_to_read = (int) strtol (words_to_read_str, NULL, DECIMAL_BASE);
  if (fill_database (in_file, words_to_read, markov_chain) == EXIT_FAILURE)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    return NULL;
  }
  return markov_chain;
}

/**
 * This function makes and prints a given number of tweets
 * @param markov_chain The database
 * @param num_of_tweets_str number of tweets to make
 */
void print_tweets (MarkovChain *markov_chain, char *num_of_tweets_str)
{
  int num_of_tweets = (int) strtol (num_of_tweets_str, NULL, DECIMAL_BASE);
  for (int i = 1; i <= num_of_tweets; i++)
  {
    printf ("Tweet %d: ", i);
    generate_random_sequence (markov_chain, NULL, MAX_WORDS_LENGTH);
  }
}

/**
 * This function closes the file and frees the markov chain
 * @param markov_chain
 * @param file
 */
void free_and_close (MarkovChain *markov_chain, FILE *file)
{
  fclose (file);
  free_markov_chain (&markov_chain);
}

int main (int argc, char *argv[])
{
  get_and_update_seed (SEED_ARG);
  if (check_num_of_args (argc) == EXIT_FAILURE)
  {
    return EXIT_FAILURE;
  }
  FILE *in_file = fopen (FILE_PATH, "r");
  if (in_file == NULL)
  {
    printf ("ERROR: The given file is not valid");
    return EXIT_FAILURE;
  }
  MarkovChain *markov_chain = (argc == FOUR_ARGS) ?
                              fill_data (NUM_OF_WORDS_TO_READ, in_file) :
                              fill_data (READ_ALL_FILE_STR, in_file);
  if (markov_chain == NULL)
  {
    fclose (in_file);
    return EXIT_FAILURE;
  }
  print_tweets (markov_chain, NUM_OF_TWEETS_TO_MAKE);
  free_and_close (markov_chain, in_file);
}

