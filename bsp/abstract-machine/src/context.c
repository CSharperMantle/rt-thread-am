#include "rtdef.h"
#include <am.h>
#include <klib.h>
#include <rtthread.h>

typedef struct CallContext {
  void *tentry;
  void *parameter;
  void *texit;
} CallContext_t;

static __attribute__((noreturn)) void kthread(void *arg) {
  const CallContext_t *const ctx = arg;
  ((void (*)(void *))(ctx->tentry))(ctx->parameter);
  ((void (*)(void))(ctx->texit))();
  while (1) {
    ;
  }
}

static rt_ubase_t ctx_from = 0;
static rt_ubase_t ctx_to = 0;

static Context *ev_handler(Event e, Context *c) {
  switch (e.event) {
  case EVENT_YIELD:
    if (ctx_from != 0) {
      *((Context **)ctx_from) = c;
    }
    c = *((Context **)ctx_to);
    break;
  case EVENT_IRQ_TIMER:
    // printf("Warn: EVENT_IRQ_TIMER at ctx %p\n", c);
    break;
  default:
    printf("Unhandled event ID = %d\n", e.event);
    assert(0);
  }
  return c;
}

void __am_cte_init(void) { cte_init(ev_handler); }

void rt_hw_context_switch_to(rt_ubase_t to) {
  ctx_from = 0;
  ctx_to = to;
  yield();
}

void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to) {
  ctx_from = from;
  ctx_to = to;
  yield();
}

void rt_hw_context_switch_interrupt(void *context, rt_ubase_t from,
                                    rt_ubase_t to,
                                    struct rt_thread *to_thread) {
  assert(0);
}

rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter,
                             rt_uint8_t *stack_addr, void *texit) {
  rt_uint8_t *sp =
      (rt_uint8_t *)RT_ALIGN_DOWN((uintptr_t)stack_addr, sizeof(uintptr_t));
  sp -= sizeof(CallContext_t);

  CallContext_t *const callCtx = (CallContext_t *)sp;
  *callCtx = (CallContext_t){
      .tentry = tentry,
      .parameter = parameter,
      .texit = texit,
  };

  Context *const ctx =
      kcontext((Area){.start = NULL, .end = sp}, kthread, callCtx);

  return (rt_uint8_t *)ctx;
}
