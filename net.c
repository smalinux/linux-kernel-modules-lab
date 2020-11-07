#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/net.h>
#include <net/sock.h>
#include <linux/in.h>
MODULE_LICENSE("GPL");

struct socket *sock;

static int hello_init(void)
{
	int err;
	// Create TCP socket	
	err = sock_create_kern(&init_net, PF_INET, SOCK_STREAM, IPPROTO_TCP, &sock);
	if (err < 0) {
        	/* handle error */
		pr_err("Failed sock_create_kern");
	}
	
	pr_err("Hello, world\n");
	return 0;
}

static void hello_exit(void)
{
	sock_release(sock);
	// Release Socket
	pr_err("Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
