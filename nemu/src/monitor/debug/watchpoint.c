#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_pool[NR_WP] = {}; /* why use static */
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  wp_pool[0].prev = NULL;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }

  for (i = 1; i< NR_WP; i++)
	wp_pool[i].prev = &wp_pool[i-1];
  
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp() {
	if (free_ == NULL) { /* there is no free wp */
		assert(0);
	}

	WP* node = free_;
	free_ = free_->next;
	return node;
}

void free_wp(WP *wp) {
	if (head == NULL) {
		assert(0);
	}

	if (wp == NULL) {
		assert(0);	
	}

	/* delete the wp from the list */
	wp->prev->next = wp->next;
	wp->next->prev = wp->prev;

	/* insert to free_ */
	wp->next = free_;
	free_->prev = wp;

	/* move the free_ to wp */

	free_ = wp;


}
