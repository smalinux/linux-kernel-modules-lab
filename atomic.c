/*  
 *  sma_atomic1.c - The simplest kernel module.
 */
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */

#include <asm/atomic.h>

static int __init sma_hello(void)
{
	atomic_t v; /* define v */
	atomic_t u = ATOMIC_INIT(0); /* define u and initialize it to zero */

	atomic_set(&v, 4); /* v = 4 (atomically) */
	atomic_add(2, &v); /* v = v + 2 = 6 (atomically) */
	atomic_inc(&v); /* v = v + 1 = 7 (atomically) */

	printk("%d\n", atomic_read(&v)); /* will print “7” */

/*	full list of atomic API here:
	* /include/asm-generic/atomic-instrumented.h
	* /include/asm-generic/atomic.h
	* /tools/arch/x86/include/asm/atomic.h
	* /include/linux/atomic-fallback.h
	* /include/asm-generic/bitops/instrumented-atomic.h
	*
	* elixir: clear_bit
	* bit-ops for non-atomic ops: /arch/arm/include/asm/bitops.h
*/


	return 0;
}

static void __exit sma_bye(void)
{
	printk(KERN_INFO "Goodbye world.\n");
}
module_init(sma_hello);
module_exit(sma_bye);

/** MODULE_
*/
MODULE_AUTHOR("Sohaib (smalinux) <xunilams@gmail.com>");
MODULE_VERSION("1.0");
MODULE_DESCRIPTION("hello Description");
// MODULE_ALIAS();
// MODULE_DEVICE_TABLE();
MODULE_LICENSE("GPL");
