#include <string.h>
#include "markov_chain.h"

#define LAST_IND(word) (strlen (word) - 1)
#define POINT_CHAR '.'
#define MAX_WORDS_LENGTH 20
#define ONE 1

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number maximal number to return (not including)
 * @return Random number
*/
int get_random_number (int max_number)
{
  return rand () % max_number;
}

// See full documentation in header file
MarkovChain *new_markov_chain ()
{
  MarkovChain *new_markov_chain = malloc (sizeof (MarkovChain));
  LinkedList *new_linked_list = malloc (sizeof (LinkedList));
  if (new_markov_chain == NULL || new_linked_list == NULL)
  {
    return NULL;
  }
  new_markov_chain->database = new_linked_list;
  return new_markov_chain;
}

// See full documentation in header file
bool is_ending_word (char *word)
{
  return (word[LAST_IND(word)] == POINT_CHAR);
}

// See full documentation in header file
Node *get_node_by_index (MarkovChain *markov_chain, int ind)
{
  Node *temp = NULL;
  temp = markov_chain->database->first;
  for (int i = 0; i < ind; i++)
  {
    temp = temp->next;
  }
  return temp;
}

// See full documentation in header file
MarkovNode *get_first_random_node (MarkovChain *markov_chain)
{
  int rand_num = get_random_number (markov_chain->database->size);
  Node *chosen_node = get_node_by_index (markov_chain, rand_num);
  char *chosen_word_ptr = chosen_node->data->data;
  while (is_ending_word (chosen_word_ptr) == true)
  {
    rand_num = get_random_number (markov_chain->database->size);
    chosen_node = get_node_by_index (markov_chain, rand_num);
    chosen_word_ptr = chosen_node->data->data;
  }
  return chosen_node->data;
}

// See full documentation in header file
MarkovNode *get_next_random_node (MarkovNode *state_struct_ptr)
{
  NextNodeCounter *arr = state_struct_ptr->counter_list;
  int cur_index = 0, rand_num = (get_random_number (state_struct_ptr
                                                        ->counter_list_size));
  while (rand_num > 0)
  {
    rand_num -= arr[cur_index].frequency;
    if (rand_num >= 0)
    {
      cur_index++;
    }
  }
  return arr[cur_index].markov_node;
}

// See full documentation in header file
void generate_random_sequence (MarkovChain *markov_chain, MarkovNode *
first_node, int max_length) // TODO max max_length words, or 1000 chars?
{
  if (max_length > MAX_WORDS_LENGTH)
  {
    max_length = MAX_WORDS_LENGTH;
  }
  if (first_node == NULL)
  {
    first_node = get_first_random_node (markov_chain);
  }
  printf ("%s", first_node->data);
  max_length--;
  MarkovNode *next = get_next_random_node (first_node);
  printf (" %s", next->data);
  max_length--;
  while (max_length > 0 && (!is_ending_word (next->data)))
  {
    next = get_next_random_node (next);
    printf (" %s", next->data);
    max_length--;
  }
  printf ("\n");
}

// See full documentation in header file

bool safe_realloc (NextNodeCounter **arr, size_t new_size)
{
  NextNodeCounter *temp = realloc (*arr, new_size);
  if (temp == NULL)
  {
    return false;
  }
  *arr = temp;
  return true;
}
// See full documentation in header file

bool add_node_to_counter_list (MarkovNode *first_node, MarkovNode
*second_node)
{
  if (is_ending_word (first_node->data) == true)
  {
    return true;
  }
  NextNodeCounter *arr = first_node->counter_list;
  int i = 0, cur_index = 0;
  while (i < first_node->counter_list_size)
  {
    if (strcmp (arr[cur_index].markov_node->data, second_node->data)
        == 0)
    {
      arr[cur_index].frequency++;
      first_node->counter_list_size++;
      return true;
    }
    i += arr[cur_index].frequency;
    cur_index++;
  }
  cur_index++;
  if (!safe_realloc (&first_node->counter_list, cur_index * sizeof
      (NextNodeCounter)))
  {
    return false;
  }
  first_node->counter_list_size++;
  NextNodeCounter temp = {second_node, ONE};
  cur_index--;
  first_node->counter_list[cur_index] = temp;
  return true;
}
// See full documentation in header file
Node *get_node_from_database (MarkovChain *markov_chain, char *data_ptr)
{
  Node *temp = NULL;
  temp = markov_chain->database->first;
  for (int i = 0; i < markov_chain->database->size; i++)
  {
    if (strcmp (temp->data->data, data_ptr) == 0)
    {
      return temp;
    }
    temp = temp->next;
  }
  return NULL;
}

// See full documentation in header file
Node *add_to_database (MarkovChain *markov_chain, char *data_ptr)
{
  Node *temp = get_node_from_database (markov_chain, data_ptr);
  if (temp != NULL)
  {
    return temp;
  }
  MarkovNode *new_markov_node = malloc (sizeof (MarkovNode));
  int res = add (markov_chain->database, new_markov_node);
  if (new_markov_node == NULL || res)
  {
    return NULL;
  }
  char *temp_str = malloc (strlen (data_ptr) + ONE);
  if (temp_str == NULL)
  {
    return NULL;
  }
  strcpy (temp_str, data_ptr);
  new_markov_node->data = temp_str;
  new_markov_node->counter_list = NULL;
  new_markov_node->counter_list_size = 0;
  return markov_chain->database->last;
}

// See full documentation in header file
void free_markov_chain (MarkovChain **ptr_chain)
{
  Node *cur_node = (*ptr_chain)->database->first, *temp;
  for (int i = 0; i < (*ptr_chain)->database->size; i++)
  {
    // free *data from every markov_node
    free (cur_node->data->data);
    // free counter list from every markov_node
    free (cur_node->data->counter_list);
    // free every markov_node
    free (cur_node->data);
    temp = cur_node->next;
    // free every node
    free (cur_node);
    cur_node = temp;
  }
  free ((*ptr_chain)->database);
  free (*ptr_chain);
  *ptr_chain = NULL;
}
