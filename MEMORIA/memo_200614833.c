#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/hugetlb.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/quicklist.h>
#include <linux/swap.h>
#include <linux/swapfile.h>
#include <linux/vmstat.h>
#include <linux/atomic.h>

struct sysinfo i;

static int memoinfo_proc_show(struct seq_file *m, void *v) {
	seq_printf(m, "Carnet: 200614833\n");
	seq_printf(m, "Nombre: Mario Roberto Morales Sitavi\n");
	seq_printf(m, "Sistema Operativo: Debian\n");

	#define K(x) ((x) << (PAGE_SHIFT - 10))
	//Datos de la RAM
	si_meminfo(&i);
	seq_printf(m,"Memoria Total: %8lu kB \n",K(i.totalram) );
	seq_printf(m,"Memoria Libre: %8lu kB \n",K(i.freeram) );
	long porcentaje;
	porcentaje = (100-(i.freeram * 100)/ i.totalram );
	seq_printf(m,"Porcentaje memoria utilizada: %8lu %% \n",porcentaje);

	#undef K
	return 0;
}

static int memoinfo_proc_open(struct inode *inode, struct  file *file) {
  return single_open(file, memoinfo_proc_show, NULL);
}

static const struct file_operations hello_proc_fops = {
  .owner = THIS_MODULE,
  .open = memoinfo_proc_open,
  .read = seq_read,
  .llseek = seq_lseek,
  .release = single_release,
};

//Inicio
static int __init memoinfo_proc_init(void) {
	proc_create("memo_200614833", 0, NULL, &hello_proc_fops);
	printk(KERN_INFO "Estudiante: Mario Roberto Morales Sitavi\r\n");
  	return 0;
}

//Fin
static void __exit memoinfo_proc_exit(void) {
	remove_proc_entry("memo_200614833", NULL); // ELIMINAR EL ARCHIVO DEL PROC
  	printk(KERN_INFO "Curso: Sistemas Opeativos 1\r\n");
}

MODULE_LICENSE("GPL");
module_init(memoinfo_proc_init);
module_exit(memoinfo_proc_exit);
