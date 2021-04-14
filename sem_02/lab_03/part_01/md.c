#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Romanov Alexey");

static int __init md_init(void) {
    struct task_struct *task = &init_task;

    do
    {
      printk(KERN_INFO " %s-%d, %s-%d\n",
          task->comm, task->pid,
          task->parent->comm, task->parent->pid);

    } while ((task = next_task(task)) != &init_task);

    printk(KERN_INFO " %s-%d", current->comm, current->pid);

    return 0;
}

static void __exit md_exit(void) { printk("lsproc module unloaded!\n"); }

module_init(md_init);
module_exit(md_exit);
