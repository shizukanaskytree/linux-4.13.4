//wxf. 2017.10,23
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include "gc_pid_timer.h"

SYSCALL_DEFINE2(start_gc_timer,
                int __user *, gc_tid,
                int __user  , nr_gc){
    int kgc_tid[nr_gc];
    copy_from_user(kgc_tid, gc_tid, sizeof(int)*nr_gc);
    int i = 0;
    for(i = 0; i < nr_gc; ++i){
        (find_task_by_vpid(kgc_tid[i]))->gc_start_time =
        (find_task_by_vpid(kgc_tid[i]))->se.sum_exec_runtime;

        //print for test
        printk("gc tid: %d, gc_start_time[%d] = %llu \n", kgc_tid[i], i, (find_task_by_vpid(kgc_tid[i]))->gc_start_time);
    }
    return 0;
}
