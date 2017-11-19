//wxf. 2017.10,23
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include "gc_pid_timer.h"

SYSCALL_DEFINE2(start_gc_timer,
                int __user *, gc_tid,
                int __user  , nr_gc){
    int i = 0;
    int kgc_tid[nr_gc];
    copy_from_user(kgc_tid, gc_tid, sizeof(int)*nr_gc);
    for(i = 0; i < nr_gc; ++i){
        (find_task_by_vpid(kgc_tid[i]))->gc_start_time =
        (find_task_by_vpid(kgc_tid[i]))->se.sum_exec_runtime;

        //print for test
        // printk("gc tid: %d, gc_start_time[%d] = %llu \n", kgc_tid[i], i, (find_task_by_vpid(kgc_tid[i]))->gc_start_time);
    }
    return 0;
}

SYSCALL_DEFINE3(end_gc_timer,
                int __user *, gc_tid,
                int __user, nr_gc,
                unsigned long long __user *, time){
    int i = 0;
    int kgc_tid[nr_gc];
    u64 gc_elapsed_time[nr_gc];
    unsigned long long time_max = 0;

    copy_from_user(kgc_tid, gc_tid, sizeof(int)*nr_gc);
    for(i = 0; i < nr_gc; ++i){
        (find_task_by_vpid(kgc_tid[i]))->gc_end_time =
        (find_task_by_vpid(kgc_tid[i]))->se.sum_exec_runtime;
        gc_elapsed_time[i] = (find_task_by_vpid(kgc_tid[i]))->gc_end_time - (find_task_by_vpid(kgc_tid[i]))->gc_start_time;
        //print for test
        if(gc_elapsed_time[i] > time_max) {
            time_max = gc_elapsed_time[i];
        }
        printk("-------------------\n");
        printk("gc tid: %d, gc_end_time[%d] = %llu, gc_start_time[%d] = %llu, elapsed time = %llu \n",
                kgc_tid[i],
                i,
                (find_task_by_vpid(kgc_tid[i]))->gc_end_time,
                i,
                (find_task_by_vpid(kgc_tid[i]))->gc_start_time,
                gc_elapsed_time[i]
        );
    }
    printk("end_gc_time: max gc_elapsed_time = %llu \n", time_max);
    copy_to_user(time, &(time_max), sizeof(unsigned long long));
    return 0;
}

// wxf, 20171024, vm start timer
SYSCALL_DEFINE1(start_vm_timer,
                int __user, vm_tid){
    struct task_struct *tsk = NULL;
    tsk = find_task_by_vpid(vm_tid);
    printk("-------------------\n");
    printk("tid = %d \n", vm_tid);
    if(!tsk){
        printk("start_vm_timer, tsk is NULL, error!");
        return -1;
    }else{
        //tsk->vm_start_time = tsk->se.sum_exec_runtime;
        tsk->vm_start_time = tsk->utime + tsk->stime;
        //printk("start_vm_timer:: sum_exec_runtime = %llu \n", tsk->se.sum_exec_runtime);
        printk("start_vm_timer:: vm_start_time = %llu \n", tsk->vm_start_time);
        printk("start_vm_timer:: utime = %llu \n", tsk->utime);
        printk("start_vm_timer:: stime = %llu \n", tsk->stime);
    }
    return 0;
}

SYSCALL_DEFINE2(end_vm_timer,
        int __user, vm_tid,
        unsigned long long __user *, time){
    struct task_struct *tsk = NULL;
    unsigned long long vm_time = 0;
    printk("-------------------\n");
    printk("tid = %d \n", vm_tid);
    tsk = find_task_by_vpid(vm_tid);
    if(!tsk){
        printk("end_vm_timer, tsk is NULL, error!");
        return -1;
    }else{
        // tsk->vm_end_time = tsk->se.sum_exec_runtime;
        tsk->vm_end_time = tsk->utime + tsk->stime;
        //printk("end_vm_timer:: sum_exec_runtime = %llu \n", tsk->se.sum_exec_runtime);
        printk("end_vm_timer = %llu \n", tsk->vm_end_time);
        printk("end_vm_timer:: utime = %llu \n", tsk->utime);
        printk("end_vm_timer:: stime = %llu \n", tsk->stime);

        printk("end_vm_time: vm_elapsed_time = %llu \n", tsk->vm_end_time - tsk->vm_start_time );
        vm_time = tsk->vm_end_time - tsk->vm_start_time;
        copy_to_user(time, &vm_time, sizeof(unsigned long long));
    }
    return 0;
}
