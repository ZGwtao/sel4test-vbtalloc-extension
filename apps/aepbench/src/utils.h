#pragma once

/* seL4 libraries */
#include <sel4/sel4.h>
#include <vka/vka.h>
#include <sel4utils/vspace.h>
#include <simple-default/simple-default.h>
#include <sel4platsupport/io.h>
#include <sel4platsupport/timer.h>
#include <sel4platsupport/platsupport.h>
#include <platsupport/timer.h>
#include <platsupport/local_time_manager.h>

#ifndef CONFIG_ARCH_ARM
#define CACHE_LINE_BYTES 64
#endif

#if 0
#define D(x...) printf(x)
#else
#define D(x...)
#endif

#define aepprintf(x...) do { printf("AEPBENCH | "); printf(x); } while (0);


#define MAX_TIMER_IRQS 4

struct timer_callback_info {
    irq_callback_fn_t callback;
    void *callback_data;
};
typedef struct timer_callback_info timer_callback_info_t;

typedef struct env {
    seL4_BootInfo* bootinfo;
    vka_t* vka;
    vspace_t* vspace;
    simple_t* simple;
    ps_io_ops_t ops;
    ltimer_t ltimer;

    /* The main timer notification that sel4-driver receives ltimer IRQ on */
    vka_object_t timer_notification;

    /* The badged notifications that are paired with the timer IRQ handlers */
    cspacepath_t badged_timer_notifications[MAX_TIMER_IRQS];

    /* Only needed if we're on RT kernel */
    vka_object_t reply;

    int num_timer_irqs;

    /* timer IRQ handler caps */
    sel4ps_irq_t timer_irqs[MAX_TIMER_IRQS];
    /* timer callback information */
    timer_callback_info_t timer_cbs[MAX_TIMER_IRQS];
} env_t;

typedef struct thread_config {
    seL4_CPtr init_ep;
    seL4_Word affinity;
    seL4_Word params[3];
    
    uint8_t padding[32];
} thread_config_t;

float 
calc_mean(int* array, int len);

float
calc_stddev(int* array, int len, int mean);
