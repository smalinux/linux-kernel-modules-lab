
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/skbuff.h>
#include <linux/textsearch.h>

MODULE_AUTHOR("smalinux");
MODULE_LICENSE("GPL");


#define MY_ID "sohaib"
static	struct ts_config *conf;

static unsigned int *hfunc(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	int pos;
	struct ts_state st;

	memset(&st, 0, sizeof(struct ts_state));

	for (pos = skb_find_text(skb, 0, INT_MAX, conf);
		pos != UINT_MAX;
		pos = textsearch_next(conf, &st))
		pr_err("pos at %d\n", pos);

	return NF_ACCEPT;
}


/* Initialize netfilter hook */
static struct nf_hook_ops nfho = {
	.hook 		= (nf_hookfn*)hfunc,
	.hooknum 	= NF_INET_POST_ROUTING,
	.pf 		= PF_INET,
	.priority 	= NF_IP_PRI_FIRST,		/* max hook priority */
};

static int __init LKM_init(void)
{
	conf = textsearch_prepare("kmp", MY_ID, 6, GFP_KERNEL, TS_AUTOLOAD);
	if (IS_ERR(conf))
		return PTR_ERR(conf);
	nf_register_net_hook(&init_net, &nfho);
}

static void __exit LKM_exit(void)
{
	textsearch_destroy(conf);
	nf_unregister_net_hook(&init_net, &nfho);
}

module_init(LKM_init);
module_exit(LKM_exit);
