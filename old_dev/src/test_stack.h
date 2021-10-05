// SPDX-FileCopyrightText: 2019 Jean-Louis Fuchs <ganwell@fangorn.ch>
//
// SPDX-License-Identifier: AGPL-3.0-or-later

struct item_s;
typedef struct item_s item_t;
int
test_push(item_t* item);
int
test_pop(int item);
void
test_stack_init(void);
int
test_stack_null(void);
int
test_stack_iter(int* values);
int
test_stack_top(void);
