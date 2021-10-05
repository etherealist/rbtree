// SPDX-FileCopyrightText: 2019 Jean-Louis Fuchs <ganwell@fangorn.ch>
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "testing.h"

char rb_err[1024];

rb_bind_impl_m(my, node_t)

qs_queue_bind_impl_m(qq, item_t)
qs_stack_bind_impl_m(qs, item_t)
