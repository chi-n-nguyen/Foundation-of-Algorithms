
/* Text generation with transition probabilities:
 *
 * Skeleton code written by Jianzhong Qi, May 2025
 * Edited by: (Nhat) Chi Nguyen, 1492182
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define STAGE_NUM_ONE 1							
#define STAGE_NUM_TWO 2
#define STAGE_NUM_THREE 3
#define STAGE_NUM_FOUR 4
#define STAGE_HEADER "Stage %d\n==========\n"	
#define VOCABULARY_SIZE 50						
#define MAX_TOP_WORDS 10
#define BEAM_WIDTH 2
#define MAX_ITERATIONS 10

typedef struct word_rec word_rec_t;
typedef struct language_model language_model_t;
typedef struct sent sent_t;
typedef struct node node_t;
typedef struct list list_t;
typedef int data_t;

/* Word record struct */
struct word_rec {
	char word[21];
	double probability;
	int index;
};

/* Language model struct */
struct language_model{
	word_rec_t words[VOCABULARY_SIZE];
	int total_words;
	double transition[VOCABULARY_SIZE][VOCABULARY_SIZE];
};

/* Sentence record struct for Stage 4 */
struct sent {
	int sentence[MAX_TOP_WORDS + 2];
	int last;
	double probability;
	int length;
	int insertion_order;
};

/* Linked list node struct */
struct node {
	data_t data;
	node_t *next;
};

/* Linked list struct */
struct list {
	node_t *head;
	node_t *foot;
};

/****************************************************************/

/* Function prototypes */
list_t *make_empty_list(void);
void free_list(list_t *list);
list_t *insert_at_foot(list_t *list, data_t value);

void print_stage_header(int stage_num);
void stage_one(language_model_t *model);
void stage_two(language_model_t *model);
void stage_three(language_model_t *model);
void stage_four(language_model_t *model);

/* Helper function prototypes */
void read_words(language_model_t *model);
void print_top_words(language_model_t *model);
int get_next_word(language_model_t *model, int current_word);
void print_sentence(language_model_t *model, list_t *sentence);
void initialise_beam(sent_t *sentences);
void expand_beam(language_model_t *model, sent_t *current_sentences, int current_count, 
				sent_t *new_sentences, int *new_count);
void expand_single_sentence(language_model_t *model, sent_t *sentence, 
						   sent_t *new_sentences, int *new_count, int *insertion_counter);
int prune_beam(sent_t *candidates, int candidate_count, sent_t *selected);
int all_sentences_complete(sent_t *sentences, int count);
void ensure_sentence_complete(sent_t *sentence);
void copy_sentence(sent_t *dest, sent_t *src);
int compare_sentences(const void *a, const void *b);
void print_sent_array(language_model_t *model, sent_t *sentence);

/****************************************************************/

/* Main function controls all the action */
int
main(int argc, char *argv[]) {
	language_model_t model;
	model.total_words = 0;

	/* Stage 1: Read word records */
	stage_one(&model); 
	
	/* Stage 2: Read transition probabilities */
	stage_two(&model);
	
	/* Stage 3: Generate text with transition probabilities */
	stage_three(&model);
	
	/* Stage 4: Generate text with transition probabilities, advanced */
	stage_four(&model);

	return 0;
}

/****************************************************************/

/** 
 * @brief: Stage 1: Read word records and output top words by probability
 */
void 
stage_one(language_model_t *model) {
	read_words(model);
	
	print_stage_header(STAGE_NUM_ONE);
	print_top_words(model);
	printf("\n");
}

/** 						
 * @brief: Stage 2: Read transition probabilities and find most likely next words
 */
void 
stage_two(language_model_t *model) {
	/* Read transition probability matrix */
	for (int i = 0; i < model->total_words; i++) {
		for (int j = 0; j < model->total_words; j++) {
			scanf("%lf", &model->transition[i][j]);
		}
	}

	print_stage_header(STAGE_NUM_TWO);

	/* For each word except <end>, find most likely next word */
	for (int i = 1; i < model->total_words; i++) {
		int best_next = 0;
		double max_prob = model->transition[i][0];

		/* Find column with maximum probability, tiebreak by choosing smaller column */
		for (int j = 1; j < model->total_words; j++) {
			if (model->transition[i][j] > max_prob) {
				max_prob = model->transition[i][j];
				best_next = j;
			}
		}
		
		printf("%s -> %s\n", model->words[i].word, model->words[best_next].word);
	}
 	
	printf("\n");
}

/**
 * @brief: Stage 3: Generate text using transition probabilities with linked list
 */
void 
stage_three(language_model_t *model) {
	print_stage_header(STAGE_NUM_THREE);

	list_t *sentence = make_empty_list();

	/* Start with <start> token */
	sentence = insert_at_foot(sentence, 1);
	int current_word = 1;
	int regular_word_count = 0;

	/* Generate sentence by following most likely transitions */
	while (regular_word_count < 10) {
		int next_word = get_next_word(model, current_word);
		
		sentence = insert_at_foot(sentence, next_word);
		current_word = next_word;

		/* Stop if <end> token is reached */
		if (next_word == 0) {
			break;
		}

		regular_word_count++;
	}

	/* Add <end> if not already present */
	if (current_word != 0) {
		sentence = insert_at_foot(sentence, 0);
	}

	print_sentence(model, sentence);	
	free_list(sentence);
	printf("\n");
}

/** 
 * @brief: Stage 4: Generates sentences using beam search algorithm with transition probabilities
 *         Maintains top 2 partial sentences at each iteration, expanding each by considering
 *         all possible next words with non-zero transition probabilities
 *         Terminates when all sentences end with <end> or after 10 iterations
 *         Outputs the highest probability sentence from the final beam
 */
void 
stage_four(language_model_t *model) {
	print_stage_header(STAGE_NUM_FOUR);

	sent_t current_sentences[BEAM_WIDTH];
	sent_t candidate_sentences[BEAM_WIDTH * VOCABULARY_SIZE];
	int current_count = 1;

	/* Zero out all memory to prevent garbage data */
	memset(current_sentences, 0, sizeof(current_sentences));
	memset(candidate_sentences, 0, sizeof(candidate_sentences));

	/* Step 1: Initialise beam search with starting sentence containing "<start>" */
	initialise_beam(current_sentences);
	
	/* Main beam search loop - maximum 10 iterations */
	for (int iteration = 0; iteration < MAX_ITERATIONS; iteration++) {
		int candidate_count = 0;
		
		/* Step 2: Expansion phase - For each partial sentence in current beam,
		 * generate all possible extensions by adding words with non-zero transition probabilities
		 */
		expand_beam(model, current_sentences, current_count, 
				   candidate_sentences, &candidate_count);

		/* Step 3: Selection phase - Sort all candidate sentences by probability (descending)
		 * and keep only top 2 for next iteration
		 */
		qsort(candidate_sentences, candidate_count, sizeof(sent_t), compare_sentences);
		
		/* Keep only top BEAM_WIDTH sentences for next iteration */
		current_count = prune_beam(candidate_sentences, candidate_count, current_sentences);
		
		/* Step 4: Termination check - Stop if all sentences end with <end> */
		if (all_sentences_complete(current_sentences, current_count)) {
			break;
		}
	}

	/* Step 5: Output - Print highest probability sentence, adding <end> if missing */
	ensure_sentence_complete(&current_sentences[0]);
	print_sent_array(model, &current_sentences[0]);
}

/****************************************************************/
/* Helper functions */

/**
 * @brief: Read words and probabilities from input
 * Assumption: Test data is guaranteed valid, no need for input validity checking
 */
void 
read_words(language_model_t *model) {
	scanf("%d", &model->total_words);

	for (int i = 0; i < model->total_words; i++) {
		scanf("%20s %lf", model->words[i].word, &model->words[i].probability);
		model->words[i].index = i;
	}
}

/** 
 * @brief: Filter, sort, print the top words by probability
 * Insertion sort adapted from Alistair Moffat's example for the book
 * "Programming, Problem Solving, and Abstraction with C", Pearson
 * Custom Books, Sydney, Australia, 2002; revised edition 2012,
 * ISBN 9781486010974
 * https://edstem.org/au/courses/19957/lessons/72882/slides/487112 
 */
void 
print_top_words(language_model_t *model) {
	word_rec_t words_sorted[VOCABULARY_SIZE];
	int sorted_count = 0;

	/* Filter out <start> and <end> */
	for (int i = 0; i < model->total_words; i++) {
		const char *w = model->words[i].word;
		if (strcmp(w, "<start>") != 0 && strcmp(w, "<end>") != 0) {
			words_sorted[sorted_count++] = model->words[i];
		}
	}

	/* Insertion sort by probability (DESC), tiebreak on index (ASC) */
	for (int i = 1; i < sorted_count; i++) {
		word_rec_t key = words_sorted[i];
		int j = i - 1;
		while (j >= 0 &&
			   ( words_sorted[j].probability <  key.probability
			   || (words_sorted[j].probability == key.probability
				   && words_sorted[j].index > key.index) )) {
			words_sorted[j + 1] = words_sorted[j];
			j--;
		}
		words_sorted[j + 1] = key;
	}
	
	/* Print up to MAX_TOP_WORDS of the sorted list */
	printf("<start>");
	int to_print = (sorted_count < MAX_TOP_WORDS) ? sorted_count : MAX_TOP_WORDS;
	for (int i = 0; i < to_print; i++) {
		printf(" %s", words_sorted[i].word);
	}
	printf(" <end>\n");
}

/** 
 * @brief: Get the next word based on transition probabilities
 * @return: 0 if "<end>" is reached, otherwise return the index of the next word
 */
int
get_next_word(language_model_t *model, int current_word) {
	int best_next = 0;
	double max_prob = model->transition[current_word][0];

	for (int j = 1; j < model->total_words; j++) {
		if (model->transition[current_word][j] > max_prob) {
			max_prob = model->transition[current_word][j];
			best_next = j;
		}
	}
	
	return best_next;
}

/** 
 * @brief: Print sentence from linked list
 */
void 
print_sentence(language_model_t *model, list_t *sentence) {
	node_t *current = sentence->head;

	while (current != NULL) {
		printf("%s", model->words[current->data].word);
		current = current->next;

		if (current != NULL) {
			printf(" ");
		}
	}
	printf("\n");
}

/**
 * @brief: Initialise the beam search with 
 * 		   a starting sentence containing "<start>"
 */
void 
initialise_beam(sent_t *sentences) {
	memset(&sentences[0], 0, sizeof(sent_t));

	sentences[0].sentence[0] = 1;  
	sentences[0].last = 1;        
	sentences[0].probability = 1.0; 
	sentences[0].length = 1;      
	sentences[0].insertion_order = 0;
}

/**
 * @brief: Expand all current sentences by adding possible next words
 */
void 
expand_beam(language_model_t *model, sent_t *current_sentences, int current_count, 
		   sent_t *new_sentences, int *new_count) {
	*new_count = 0;
	int insertion_counter = 0;
	
	for (int i = 0; i < current_count; i++) {
		/* If sentence already ends with <end>, copy unchanged to candidates */
		if (current_sentences[i].last == 0) {
			copy_sentence(&new_sentences[*new_count], &current_sentences[i]);
			new_sentences[*new_count].insertion_order = insertion_counter++;
			(*new_count)++;
		} else {
			/* Otherwise, generate all possible extensions */
			expand_single_sentence(model, &current_sentences[i], 
								 new_sentences, new_count, &insertion_counter);
		}
	}
}

/**
 * @brief: Expand a single sentence with all possible next words
 */
void 
expand_single_sentence(language_model_t *model, sent_t *sentence, 
					  sent_t *new_sentences, int *new_count, int *insertion_counter) {
	for (int next_word = 0; next_word < model->total_words; next_word++) {
		double trans_prob = model->transition[sentence->last][next_word];
		
		if (trans_prob > 0.0) {
			/* Bounds check: Ensure we don't overflow arrays */
			if (*new_count >= BEAM_WIDTH * VOCABULARY_SIZE) {
				break;
			}
			if (sentence->length >= MAX_TOP_WORDS + 2) {
				break;
			}

			/* Create new sentence by extending current sentence with next_word */
			copy_sentence(&new_sentences[*new_count], sentence);
			new_sentences[*new_count].sentence[sentence->length] = next_word;
			new_sentences[*new_count].last = next_word;

			/* Maintain sentence probability by multiplying transition probabilities */
			new_sentences[*new_count].probability *= trans_prob;
			new_sentences[*new_count].length++;
			new_sentences[*new_count].insertion_order = (*insertion_counter)++;
			(*new_count)++;
		}
	}
}

/**
 * @brief: Select the top BEAM_WIDTH sentences from candidates
 */
int 
prune_beam(sent_t *candidates, int candidate_count, sent_t *selected) {
	int selected_count = (candidate_count < BEAM_WIDTH) ? candidate_count : BEAM_WIDTH;
	
	for (int i = 0; i < selected_count; i++) {
		copy_sentence(&selected[i], &candidates[i]);
	}
	
	return selected_count;
}

/**
 * @brief: Check if all sentences in the beam are complete (end with <end>)
 */
int 
all_sentences_complete(sent_t *sentences, int count) {
	for (int i = 0; i < count; i++) {
		if (sentences[i].last != 0) {  /* 0 is <end> token index */
			return 0;
		}
	}
	return 1;
}

/**
 * @brief: Ensure sentence ends with <end> token
 */
void 
ensure_sentence_complete(sent_t *sentence) {
	if (sentence->last != 0) {
		sentence->sentence[sentence->length] = 0;  /* Add <end> token */
		sentence->length++;
	}
}

/**
 * @brief: Copy one sentence structure to another
 */
void 
copy_sentence(sent_t *dest, sent_t *src) {
	memset(dest, 0, sizeof(sent_t));

	for (int i = 0; i < src->length; i++) {
		dest->sentence[i] = src->sentence[i];
	}

	dest->last = src->last;
	dest->probability = src->probability;
	dest->length = src->length;
	dest->insertion_order = src->insertion_order;
}

/**
 * @brief: Comparator for qsort - sorts by probability (descending), 
 * 		   maintains original order for ties
 * 		   This implements the required tie-breaking rule: earlier records win ties
 */
int 
compare_sentences(const void *a, const void *b) {
	const sent_t *sent_a = (const sent_t *)a;
	const sent_t *sent_b = (const sent_t *)b;
	
	/* Primary sort: by probability (descending) */
	if (sent_a->probability > sent_b->probability) {
		return -1;  /* a has higher probability */
	} else if (sent_a->probability < sent_b->probability) {
		return 1;   /* b has higher probability */
	} else {
		/* Tie-breaking: earlier insertion order wins */
		return sent_a->insertion_order - sent_b->insertion_order;
	}
}

/**
 * @brief: Print a sentence array for Stage 4 output
 */
void 
print_sent_array(language_model_t *model, sent_t *sentence) {
	for (int i = 0; i < sentence->length; i++) {
		/* Bounds check to prevent memory corruption */
		if (sentence->sentence[i] >= 0 && sentence->sentence[i] < model->total_words) {
			printf("%s", model->words[sentence->sentence[i]].word);
			if (i < sentence->length - 1) {
				printf(" ");
			}
		}
	}
	printf("\n");
}

/****************************************************************/
/* Provided functions */

/* Print stage header given stage number */
void 
print_stage_header(int stage_num) {
	printf(STAGE_HEADER, stage_num);
}

/****************************************************************/
/* Linked list implementation below, adapted from
   https://people.eng.unimelb.edu.au/ammoffat/ppsaa/c/listops.c 
*/

/* Create an empty list */
list_t
*make_empty_list(void) {
	list_t *list;

	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;

	return list;
}

/* Free the memory allocated for a list (and its nodes) */
void
free_list(list_t *list) {
	node_t *curr, *prev;

	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}

	free(list);
}

/* Insert a new data element into the end of a linked list */
list_t
*insert_at_foot(list_t *list, data_t value) {
	node_t *new;

	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->data = value;
	new->next = NULL;

	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}

	return list;
}

/****************************************************************/
/*
	Assuming N to denote the number of input words, 
	L to denote the number of words to be generated in the output sentence, 
	and K to denote the number of partial sentences kept after each iteration:
	
	What is the worst-case time complexity of the algorithm described in Stage 4, 
	and why?
	
	Answer: O(L * K * N * log(K * N))
	
	Explanation:
	- For each of L iterations (maximum sentence length)
	- We expand K partial sentences, each trying N possible next words: O(K * N)
	- We then sort K * N candidates using qsort: O(K * N * log(K * N))
	- Total per iteration: O(K * N + K * N * log(K * N)) = O(K * N * log(K * N))
	- Over L iterations: O(L * K * N * log(K * N))
*/