#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x5260efca, "module_layout" },
	{ 0x282fd5f5, "kthread_stop" },
	{ 0xe9587909, "usb_unregister_notify" },
	{ 0xfed11ed1, "usb_mon_deregister" },
	{ 0x54554948, "kobject_put" },
	{ 0xfca528e3, "sysfs_create_group" },
	{ 0xecc3c7d7, "kobject_create_and_add" },
	{ 0xcc2e8c93, "kernel_kobj" },
	{ 0x37013607, "mutex_unlock" },
	{ 0x3be89d3c, "usb_register_notify" },
	{ 0xace5c0fc, "usb_bus_list" },
	{ 0x7da7be30, "mutex_lock" },
	{ 0x98dc13bd, "usb_bus_list_lock" },
	{ 0x407e53f6, "wake_up_process" },
	{ 0xbe79c9cc, "kthread_create_on_node" },
	{ 0x1afe3bc4, "usb_mon_register" },
	{ 0xf9a482f9, "msleep" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x69acdf38, "memcpy" },
	{ 0x27e1a049, "printk" },
	{ 0xe914e41e, "strcpy" },
	{ 0x91715312, "sprintf" },
	{ 0x8f64aa4, "_raw_spin_unlock_irqrestore" },
	{ 0x9327f5ce, "_raw_spin_lock_irqsave" },
	{ 0xadaabe1b, "pv_lock_ops" },
	{ 0xd52bf1ce, "_raw_spin_lock" },
	{ 0xbdfb6dbb, "__fentry__" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "5950E89A9BF824A0718C771");
