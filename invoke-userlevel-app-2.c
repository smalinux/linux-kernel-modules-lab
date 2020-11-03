#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
//# include <linux/config.h>
#include <linux/kernel.h> /* printk () */
#include<linux/sched.h>

MODULE_LICENSE ( "GPL" );

static __init int testDriver1_init ( void )
{
	int result = 0;
	char cmdPath [] = "/sbin/shutdown" ;
	char * cmdArgv [] = {cmdPath, "now" , NULL};
	char * cmdEnvp [] = { "HOME =/" , "PATH =/sbin :/bin:/usr/bin" , NULL};
	
	result = call_usermodehelper (cmdPath, cmdArgv, cmdEnvp, UMH_WAIT_PROC);
	return result;
}

static __exit void testDriver1_exit ( void )
{
}

module_init (testDriver1_init);
module_exit (testDriver1_exit);
