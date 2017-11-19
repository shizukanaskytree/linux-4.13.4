//wxf, 2017.10.23
asmlinkage long sys_start_gc_timer(int __user *gc_tid, int __user nr_gc);
asmlinkage long sys_end_gc_timer(int __user *gc_tid, int __user nr_gc, unsigned long long __user *time);
//wxf, 20171024
asmlinkage long sys_start_vm_timer(int __user vm_tid);
asmlinkage long sys_end_vm_timer(int __user vm_tid, unsigned long long __user *time);
