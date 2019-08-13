// SPDX-FileCopyrightText: 2019 Jean-Louis Fuchs <ganwell@fangorn.ch>
//
// SPDX-License-Identifier: AGPL-3.0-or-later

struct item_s;
typedef struct item_s item_t;
struct item_s {
    int     value;
    item_t* next;
};

int
test_enqueue(item_t* item);
int
test_dequeue(int item);
void
test_queue_init(void);
int
test_queue_null(void);
int
test_queue_iter(int* values);
int
test_queue_head(void);
int
test_queue_tail(void);
