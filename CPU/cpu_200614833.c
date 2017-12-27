#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/seq_file.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

void recorrer(struct seq_file *m, struct task_struct *s, int sum)
{
    struct list_head *list;
    struct task_struct *task;
    char Estado[15];
    unsigned int usuario;
    char user[16];
    int i;
    for (i = 0; i < sum; i++)
        seq_printf(m,"");

    if(s->state == TASK_RUNNING){
        strcpy(Estado,"Corriendo");	
    }else if(s->state == TASK_STOPPED){
        strcpy(Estado,"Detenido");	
    }else if(s->state == TASK_INTERRUPTIBLE){
        strcpy(Estado,"Interrumpible");
    }else if(s->state == TASK_UNINTERRUPTIBLE){
        strcpy(Estado,"No Interrumpible");
    }else if(s->state == EXIT_ZOMBIE){
        strcpy(Estado,"Zombie");
    }else{
        strcpy(Estado,"Desconocido");
    }


    seq_printf(m," %d - %s - %s \n" , s->pid, s->comm,Estado);


    sum = sum + strlen(s->comm);
    list_for_each(list, &s->children) {
        task = list_entry(list, struct task_struct, sibling);
        recorrer(m, task, sum);
    }

}


static int inicio_arbol(struct seq_file *m, void *v)
{
	seq_printf(m, "Carnet: 200614833\n");
	seq_printf(m, "Nombre: Mario Roberto Morales Sitavi\n");
	seq_printf(m, "Sistema Operativo: Debian\n\n");
	seq_printf(m, "PID - NOMBRE - ESTADO\n");

	struct task_struct *parent = current; 
	while (parent->pid != 1)
		parent = parent->parent; 
	recorrer(m, parent, 0);
	return 0;
}


static int cpuinfo_proc_open(struct inode *inode, struct  file *file) {
  return single_open(file, inicio_arbol , NULL);
}

static const struct file_operations hello_proc_fops = {
  .owner = THIS_MODULE,
  .open = cpuinfo_proc_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

//Inicio
static int __init cpuinfo_proc_init(void) {
	proc_create("cpu_200614833", 0, NULL, &hello_proc_fops);
	printk(KERN_INFO "Estudiante: Mario Roberto Morales Sitavi\r\n");
  	return 0;
}

//Fin
static void __exit cpuinfo_proc_exit(void) {
	remove_proc_entry("cpu_200614833", NULL); 
  	printk(KERN_INFO "Curso: Sistemas Opeativos 1\r\n");
}

MODULE_LICENSE("GPL");
module_init(cpuinfo_proc_init);
module_exit(cpuinfo_proc_exit);
