/*                                                     
 * LDD3 P: 16
 */                                                    
#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");


/**************************
 * Macros to help debugging
 */

//When you do not want to print the information, just comment out the #define information, and then debug again and uncomment the #define information.
#define DEBUG_A
#ifdef DEBUG_A
#define SMA_DEBUG(fmt, args...) printk( KERN_DEBUG "SMA: " fmt, ## args)
#endif

/*
 * End: Macros to help debugging 
 ********************************/

static int hello_init(void)
{
	SMA_DEBUG("Hello, world\n");
	return 0;
}

static void hello_exit(void)
{
	SMA_DEBUG("Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
