/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-16     liboran       the first version
 */

#include "rt_trace.h"

#ifdef PKG_USING_RT_TRACE

extern rt_uint32_t (*rt_trace_timestamp)(void);

extern struct rt_trace_ctf trace_ctf;

#ifdef TRACE_LISTEN_SCHEDULE
static void rt_scheduler_hook(struct rt_thread *from, struct rt_thread *to)
{
    ctf_bounded_string_t name_to = {"unknown"};
    ctf_bounded_string_t name_from = {"unknown"};

    rt_strncpy((char *)&name_from, from->name, RT_NAME_MAX);
    rt_strncpy((char *)&name_to, to->name, RT_NAME_MAX);

    ctf_event_thread_switch_in(from->current_priority, name_from);
    ctf_event_thread_switch_out(to->current_priority, name_to);
}
#endif /* TRACE_LISTEN_SCHEDULE */

#ifdef TRACE_LISTEN_TIMER
static void rt_timer_enter_hook(struct rt_timer *timer)
{
    ctf_bounded_string_t name = {"unknown"};
    rt_strncpy((char *)&name, rt_thread_self()->name, RT_NAME_MAX);

    ctf_event_timer_enter(name);
}

static void rt_timer_exit_hook(struct rt_timer *timer)
{
    ctf_bounded_string_t name = {"unknown"};
    rt_strncpy((char *)&name, rt_thread_self()->name, RT_NAME_MAX);

    ctf_event_timer_exit(name);
}
#endif /* TRACE_LISTEN_TIMER */

#ifdef TRACE_LISTEN_IPC
static void rt_object_take_hook(struct rt_object *object)
{
    ctf_bounded_string_t name = {"unknown"};

    rt_strncpy((char *)&name, object->name, RT_NAME_MAX);
    switch (object->type & (~RT_Object_Class_Static))
    {
#ifdef TRACE_LISTEN_SEM
        case RT_Object_Class_Semaphore:
            ctf_event_sem_take(name);
            break;
#endif /* TRACE_LISTEN_SEM */
#ifdef TRACE_LISTEN_MUTEX
        case RT_Object_Class_Mutex:
            ctf_event_mutex_take(name);
            break;
#endif /* TRACE_LISTEN_MUTEX */
#ifdef TRACE_LISTEN_EVENT
        case RT_Object_Class_Event:
            ctf_event_event_recv(name, ((rt_event_t)object)->set);
            break;
#endif /* TRACE_LISTEN_EVENT */
#ifdef TRACE_LISTEN_MB
        case RT_Object_Class_MailBox:
            ctf_event_mb_recv(name);
            break;
#endif /* TRACE_LISTEN_MB */
#ifdef TRACE_LISTEN_MQ
        case RT_Object_Class_MessageQueue:
            ctf_event_mq_recv(name);
            break;
#endif /* TRACE_LISTEN_MQ */
        default:
            break;
    }
}

static void rt_object_put_hook(struct rt_object *object)
{
    ctf_bounded_string_t name = {"unknown"};

    rt_strncpy((char *)&name, object->name, RT_NAME_MAX);
    switch (object->type & (~RT_Object_Class_Static))
    {
#ifdef TRACE_LISTEN_SEM
        case RT_Object_Class_Semaphore:
            ctf_event_sem_release(name);
            break;
#endif /* TRACE_LISTEN_SEM */
#ifdef TRACE_LISTEN_MUTEX
        case RT_Object_Class_Mutex:
            ctf_event_mutex_release(name);
            break;
#endif /* TRACE_LISTEN_MUTEX */
#ifdef TRACE_LISTEN_EVENT
        case RT_Object_Class_Event:
            ctf_event_event_send(name, ((rt_event_t)object)->set);
            break;
#endif /* TRACE_LISTEN_EVENT */
#ifdef TRACE_LISTEN_MB
        case RT_Object_Class_MailBox:
            ctf_event_mb_send(name);
            break;
#endif /* TRACE_LISTEN_MB */
#ifdef TRACE_LISTEN_MQ
        case RT_Object_Class_MessageQueue:
            ctf_event_mq_send(name);
            break;
#endif /* TRACE_LISTEN_MQ */
        default:
            break;
    }
}
#endif /* TRACE_LISTEN_IPC */

static void _rt_trace_timestamp_set(rt_uint32_t (*timestamp)(void))
{
    rt_trace_timestamp = timestamp;
}

RT_WEAK rt_uint32_t rt_trace_get_time(void)
{
    return 0;
}

int ctf_init(void)
{
#ifdef TRACE_LISTEN_SCHEDULE
    rt_scheduler_sethook(rt_scheduler_hook);
#endif /* TRACE_LISTEN_SCHEDULE */

#ifdef TRACE_LISTEN_TIMER
    rt_timer_enter_sethook(rt_timer_enter_hook);
    rt_timer_exit_sethook(rt_timer_exit_hook);
#endif /* TRACE_LISTEN_TIMER */

#ifdef TRACE_LISTEN_IPC
    rt_object_take_sethook(rt_object_take_hook);
    rt_object_put_sethook(rt_object_put_hook);
#endif /* TRACE_LISTEN_IPC */

    _rt_trace_timestamp_set(rt_trace_get_time);

    trace_ctf.buffer1 = (rt_uint8_t *)rt_malloc(TRACE_PACKET_SIZE);
    if (trace_ctf.buffer1 == RT_NULL)
    {
        rt_kprintf("no memory\n");
        return -1;
    }
    trace_ctf.buffer2 = (rt_uint8_t *)rt_malloc(TRACE_PACKET_SIZE);
    if (trace_ctf.buffer2 == RT_NULL)
    {
        rt_kprintf("no memory\n");
        return -1;
    }
    trace_ctf.buffer_align_size = RT_ALIGN_DOWN(TRACE_PACKET_SIZE, RT_ALIGN_SIZE);

    return 0;
}
INIT_APP_EXPORT(ctf_init);

#endif /* PKG_USING_RT_TRACE */
