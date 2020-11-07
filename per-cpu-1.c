#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
MODULE_LICENSE("GPL");

// Define my per-cpu variable
DEFINE_PER_CPU(int, x);

static int sma_init(void)
{
	int num_cpus = num_online_cpus();
	int i = 0;
	int val;

	pr_info("Number of cpus available:%d\n", num_cpus);
	for (i = 0; i < num_cpus; i++) {
		int value = per_cpu(x, i);
		pr_info("Value of x is %d at Processor:%d\n", value, i);
	}

	val = get_cpu_var(x);
	get_cpu_var(x) = 10;
	put_cpu_var(x);
	this_cpu_inc(x);

	pr_info("Printing x value of all processor after updating current processor:%d\n",
			smp_processor_id());

	for (i = 0; i < num_cpus; i++) {
		int value = per_cpu(x, i);
		pr_info("Value of x is %d at Processor:%d\n", value, i);
	}

	return 0;
}

static void sma_exit(void)
{
	pr_err("Goodbye, cruel world\n");
}

module_init(sma_init);
module_exit(sma_exit);
