#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/usb.h>
#include <linux/usb/hcd.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/notifier.h>
#include <linux/mutex.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <asm/i387.h>

struct task_struct *init_thread;
static int in;
static int out;
static int in_size;
static int out_size;
static int in_res;
static int out_res;
static int in_size_res;
static int out_size_res;
static DEFINE_SPINLOCK(my_lock);
static DEFINE_SPINLOCK(my_lock_1);
static unsigned long precision = 10000;

static int mon_notify(struct notifier_block *self, unsigned long action,
		      void *dev)
{
  struct usb_bus *ubus = (struct usb_bus*)dev;
	if (action == USB_BUS_ADD)
	{
	  ubus->monitored = 1;
	  mb();
	}
	return NOTIFY_OK;
}

static struct notifier_block mon_nb = {
	.notifier_call = 	mon_notify,
};

static void mon_submit (struct usb_bus *ubus, struct urb *urb)
{
	//spin_lock(&my_lock);
	//++x;
	//spin_unlock(&my_lock); 
	
}

static void mon_submit_error(struct usb_bus *ubus, struct urb *urb, int error)
{

  //spin_lock(&my_lock);
  //++x;
  //spin_unlock(&my_lock); 
	
}
static void mon_complete(struct usb_bus *ubus, struct urb *urb, int status)
{
 
  if (urb->transfer_flags == URB_DIR_IN)
  {
    spin_lock(&my_lock);
    ++in;
    in_size += urb->actual_length;
    spin_unlock(&my_lock);
  } 
    if (urb->transfer_flags == URB_DIR_OUT)
  {
    spin_lock(&my_lock);
    ++out;
    out_size+=urb->actual_length;
    
    spin_unlock(&my_lock);
  }
	
}
void count_speed(int a)
{
  unsigned long speed = a* precision / 10;
  char buf [40]; 
  sprintf(buf, "%lu", speed);
  if (speed == 0) 
  {
    pr_notice("Speed is : 0\n");
  }
  else
  {
    char x [20]; char y [20];
    if(strlen(buf) <= 4)
    {
      strcpy(x,"0");
      strcpy(y,buf);
    }
    else
    {
      int l =  strlen(buf)-4;
      int i;
      for (i =0; i < l; ++i)
        x[i] = buf[i];
      x[l] = '\0';  
  
      strcpy(y,buf+l);
    }
      
    pr_notice("Speed is : %s.%s\n",x,y);
      
      
  }
}


static int my_print(void * a)
{
  unsigned long flags;  
  while(!kthread_should_stop())
  {
    spin_lock_irqsave(&my_lock,flags);
    int a = in; int b = out; int c = in_size; int d = out_size;
    in = 0; out = 0; in_size = 0; out_size = 0;
    spin_unlock_irqrestore(&my_lock,flags);
     
    spin_lock_irqsave(&my_lock_1,flags);
    in_res = a; in_size_res = c; out_res = b; out_size_res = d;
    spin_unlock_irqrestore(&my_lock_1,flags);  
    
    pr_notice(" In \n size: %i , number : %i\n", a, c);
    count_speed(c);
    pr_notice(" Out \n size: %i , number : %i\n", b, d);
    count_speed(d);
    
    msleep(10000);
    
  }
  
return 0;
}

static struct usb_mon_operations mon_ops_0 = {
	.urb_submit =	mon_submit,
	.urb_submit_error = mon_submit_error,
	.urb_complete =	mon_complete,
};

///////////////////////////////////////////////////
static ssize_t out_show(struct kobject *kobj, struct kobj_attribute *attr,
                          char *buf)
{
  unsigned long flags;
  spin_lock_irqsave(&my_lock_1,flags);
  int a = out_res; int b = out_size_res;
  spin_unlock_irqrestore(&my_lock_1,flags); 
   
  return sprintf(buf, "Packets out: %i; size: %i\n", a,b);
}
 
static ssize_t in_show(struct kobject *kobj, struct kobj_attribute *attr,
                          char *buf)
{
  unsigned long flags;
  spin_lock_irqsave(&my_lock_1,flags);
  int a = in_res; int b = in_size_res;
  spin_unlock_irqrestore(&my_lock_1,flags); 
  return sprintf(buf, "Packets in: %i; size: %i\n", a,b);
}
  
static struct kobj_attribute in_attribute =
          __ATTR(in, 0444, in_show, NULL);
   
static struct kobj_attribute out_attribute =
          __ATTR(out, 0444, out_show, NULL);
          
          
static struct attribute *attrs[] = {
    &in_attribute.attr,
    &out_attribute.attr,
    NULL,   /* need to NULL terminate the list of attributes */
  };
  
  static struct attribute_group attr_group = {
          .attrs = attrs,
 };
 
/////////////////////////////////////////////
////////
 static struct kobject *my_kobj;

static int __init mon_init(void)
{
  int retval;
  struct usb_bus *ubus;
  if (usb_mon_register(&mon_ops_0) != 0) 
	  pr_notice( ": unable to register with the core\n");
		
  else 
    {
      pr_notice("Usb monitoring module was initialized\n");	
      init_thread = kthread_run(&my_print, NULL, "pcsgu250_init");
  	
  	  mutex_lock(&usb_bus_list_lock);
	    list_for_each_entry (ubus, &usb_bus_list, bus_list) {
		  ubus->monitored = 1;
	    }
	    
	    usb_register_notify(&mon_nb);
	    mutex_unlock(&usb_bus_list_lock);
	  }
	  
	
  my_kobj = kobject_create_and_add("my_kobject", kernel_kobj);
  if (!my_kobj)
    return -ENOMEM;
 
  /* Create the files associated with this kobject */
  retval = sysfs_create_group(my_kobj, &attr_group);
  if (retval)
  kobject_put(my_kobj);
  return 0;

}

static void __exit mon_exit(void)
{
  struct usb_bus *ubus;

	usb_mon_deregister();
	
	mutex_lock(&usb_bus_list_lock);
	usb_unregister_notify(&mon_nb);
	list_for_each_entry (ubus, &usb_bus_list, bus_list) {
  	ubus->monitored = 0;
	}
	
	mutex_unlock(&usb_bus_list_lock);
	kthread_stop(init_thread);
	pr_notice("Usb monitoring module was deinitialized\n");	
	kobject_put(my_kobj);

}

module_init(mon_init);
module_exit(mon_exit);

MODULE_LICENSE("GPL");
