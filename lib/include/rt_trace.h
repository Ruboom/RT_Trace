/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-16     liboran       the first version
 */

#ifndef __RT_TRACE_H__
#define __RT_TRACE_H__

#include "rtthread.h"
#include "rtconfig.h"
#include "rtdef.h"

#ifdef PKG_USING_RT_TRACE

typedef struct
{
    char buf[RT_NAME_MAX];
} ctf_bounded_string_t;

int rt_hw_jlink_rtt_init(void);

void ctf_event_thread_switch_out(rt_uint8_t pri, ctf_bounded_string_t name);
void ctf_event_thread_switch_in(rt_uint8_t pri, ctf_bounded_string_t name);
void ctf_event_memory_malloc(rt_uint32_t ptr, rt_uint32_t size, ctf_bounded_string_t name);
void ctf_event_memory_free(rt_uint32_t ptr, ctf_bounded_string_t name);
void ctf_event_timer_enter(ctf_bounded_string_t name);
void ctf_event_timer_exit(ctf_bounded_string_t name);
void ctf_event_sem_take(ctf_bounded_string_t name);
void ctf_event_sem_release(ctf_bounded_string_t name);
void ctf_event_mutex_take(ctf_bounded_string_t name);
void ctf_event_mutex_release(ctf_bounded_string_t name);
void ctf_event_event_recv(ctf_bounded_string_t name, rt_uint32_t set);
void ctf_event_event_send(ctf_bounded_string_t name, rt_uint32_t set);
void ctf_event_mb_recv(ctf_bounded_string_t name);
void ctf_event_mb_send(ctf_bounded_string_t name);
void ctf_event_mq_recv(ctf_bounded_string_t name);
void ctf_event_mq_send(ctf_bounded_string_t name);

#endif /* PKG_USING_RT_TRACE */
#endif /* __RT_TRACE_H__ */
