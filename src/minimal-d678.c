/** \file
 * Minimal test code for DIGIC 6
 * ROM dumper & other experiments
 */

#include "dryos.h"
#include "bmp.h"
#include "log-d678.h"
#include "extfunctions.h"




//For DOOM
extern void D_DoomMain(void);
extern void ml_gui_main_task();
//event ring buffer
 int inited = 0;

static void DUMP_ASM dump_task()
{
     while (!bmp_vram_raw())
    {
        msleep(100);
    }
    msleep(10000);
    uart_printf("Starting doom!");
    D_DoomMain ();
while (true)
{
uart_printf("Hello! I am in a error state so um yeah\n");
msleep(1000);
}

}

static void
my_task_dispatch_hook(
    struct context **p_context_old, /* on new DryOS (6D+), this argument is different (small number, unknown meaning) */
    struct task *prev_task_unused,  /* only present on new DryOS */
    struct task *next_task_new      /* only present on new DryOS; old versions use HIJACK_TASK_ADDR */
)
{
    struct task *next_task = next_task_new;
    if (!next_task)
        return;

    struct context *context = next_task->context;
    if (!context)
        return;
    // Do nothing unless a new task is starting via the trampoile
    if (context->pc != (uint32_t)task_trampoline)
        return;

    thunk entry = (thunk)next_task->entry;

    if (entry == &gui_main_task)
    { //gui_main_task entry
        next_task->entry = &ml_gui_main_task;
    }
}
/* called before Canon's init_task */
void boot_pre_init_task(void)
{
    task_dispatch_hook = &my_task_dispatch_hook;
}

/* called right after Canon's init_task, while their initialization continues in background */
void boot_post_init_task(void)
{

    msleep(1000);

    task_create("dump", 0x1e, 0x1000, dump_task, 0 );
}

/* used by font_draw */
/* we don't have a valid display buffer yet */
void disp_set_pixel(int x, int y, int c)
{
}

#ifndef CONFIG_5D4
/* dummy */

#endif

void ml_assert_handler(char* msg, char* file, int line, const char* func) { };
