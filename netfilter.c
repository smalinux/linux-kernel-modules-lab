
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/udp.h>

MODULE_AUTHOR("smalinux");
MODULE_LICENSE("GPL");

static unsigned int *hfunc(void *priv, struct sk_buff *skb, const struct nf_hook_state *state)
{
	struct iphdr *iph;
	struct udphdr *udph;
	if (!skb)
		return NF_ACCEPT;
	pr_err("Yes\n");
	iph = ip_hdr(skb);
	if (iph->protocol == IPPROTO_UDP) {
		udph = udp_hdr(skb);
		if (ntohs(udph->dest) == 53) {
			return NF_ACCEPT;
		}
	}
	else if (iph->protocol == IPPROTO_TCP) {
		return NF_ACCEPT;
	}
	
	return NF_DROP;
}


/* Initialize netfilter hook */
static struct nf_hook_ops nfho = {
	.hook 		= (nf_hookfn*)hfunc,
	.hooknum 	= NF_INET_POST_ROUTING,		/* received packets */
	.pf 		= PF_INET,			/* IPv4 */
	.priority 	= NF_IP_PRI_FIRST,		/* max hook priority */
};

static int __init LKM_init(void)
{
	nf_register_net_hook(&init_net, &nfho);
}

static void __exit LKM_exit(void)
{
	nf_unregister_net_hook(&init_net, &nfho);
}

module_init(LKM_init);
module_exit(LKM_exit);
