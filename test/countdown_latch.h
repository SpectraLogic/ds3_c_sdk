#ifndef COUNTDOWN_LATCH_H_
#define COUNTDOWN_LATCH_H_

typedef struct _countdown_latch countdown_latch;

countdown_latch* countdown_latch_new(guint count);
void countdown_latch_free(countdown_latch* latch);

void countdown_latch_wait(countdown_latch* latch);
void countdown_latch_count_down(countdown_latch* latch);
void countdown_latch_count_down_and_wait(countdown_latch* latch);

#endif /* COUNTDOWN_LATCH_H_ */
#endif
