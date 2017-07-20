/*
 * The Tiny Torero Shell (TTSH)
 *
 * Add your top-level comments here.
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "history_queue.h"

int historystate = 0;
// global variables: add only globals for history list state
//                   all other variables should be allocated on the stack
// static: means these variables are only in scope in this .c module
//static HistoryEntry history[MAXHIST]; 

// Look at your lab 10 implementation of a circular queue of integers and
// reuse liberally.

// TODO: Add some more global variables for the history list.
//       These should be the only globals in your program
//       They should be static so that only functions in this file can
//       access them.

HistoryEntry historyQueue[MAXHIST];
static int queue_start = 0;
static int queue_next = 0;
static int queue_size = 0;

void add_queue(HistoryEntry entry)
{
	historyQueue[queue_next] = entry;
	queue_size++;
	if (queue_size > MAXHIST)
	{
		queue_start++;
	}
	queue_next++;
	queue_next = queue_next%10;
}

void print_queue()
{
	int i;
	for (i = 0; i < MAXHIST; i++)
	{
		printf("command num: %d   %s\n", historyQueue[i].cmd_num, historyQueue[i].cmdline);
	}
}
// TODO: implement your history queue functions here
