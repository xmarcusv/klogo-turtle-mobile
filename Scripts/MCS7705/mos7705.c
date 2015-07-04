/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


/*************************************************************************
 ***
 *** Project Name: MosChip
 ***
 *** Module Name: Mos7705
 ***
 *** File: mos7705.c 
 ***		
 *** Purpose	  : It gives an interface between USB to Parallel 
 ***                and serves as a Printer Port Driver for the high 
 ***		        level layers /applications.
 *** 
 *** DBG - Code inserted due to as part of debugging
 *** DPRINTK - Debug Print statement
 ***
 *************************************************************************/

/* all file inclusion goes here */
#include <linux/version.h>

//#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)

//#include <linux/config.h>
//#endif

#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/interrupt.h> 
#include <linux/ioport.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/tty.h>
#include <linux/tty_driver.h>
#include <linux/tty_flip.h>

#include <linux/spinlock.h>
#include <linux/serial.h>

#include <linux/sysctl.h>
#include <linux/ioctl.h>
#include <asm/uaccess.h>
#include <linux/usb.h>

#include <linux/init.h>
#include <linux/parport.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0))
#include <linux/kthread.h>
#endif

//#include <linux/socket.h>


/* all defines goes here */

/*
 * Debug related defines 
 */

/* 1: Enables the debugging -- 0: Disable the debugging */

#define MOS_DEBUG	0

#if MOS_DEBUG
	#define DPRINTK(fmt, args...) printk( "%s: " fmt, __FUNCTION__ , ## args)
#else
	#define DPRINTK(fmt, args...)
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,21)
#include <linux/usb/serial.h>
#else
#include "usb-serial.h"                 /*usb-serial defines*/
#endif

#include "mos7705.h"            /* MCS7705 Defines    */
#include "mos7705_16C50.h"		/* 16C50 UART defines */

/*
 * Version Information
 */
#define DRIVER_VERSION "2.0.0"
#define DRIVER_DESC "Moschip USB Parallel Adapter"

/*
 * Defines used for sending commands to port 
 */

#define WAIT_FOR_EVER   (HZ * 0 ) /* timeout urb is wait for ever*/
#define MOS_WDR_TIMEOUT (HZ * 5 ) /* default urb timeout */
                                                                                                                             
#define MOS_PORT1       0x0200          /* Serial Port 1 */
#define MOS_PORT2       0x0300          /* Serial Port 2 */
#define MOS_VENREG      0x0000          /* Vendor Specific Cmd */
//#define MOS_MAX_PORT    0x02            /* Number of Serial Ports */
#define MOS_MAX_PORT    0x01	          /* Number of Serial Ports */
#define MOS_WRITE       0x0E            /* Write Request */
#define MOS_READ        0x0D            /* Read Request */

//#define MOS_ECR_MODE_INIT	0x45
#define MOS_ECR_MODE_INIT	0x25

/* parport */
/* --------------------------------------------------------------------- */

static int restore_state_thread(void *);
struct semaphore	thread_complete;

wait_queue_head_t restore_state_event;
int ThreadState = 0,thread_cond=0;
unsigned char dcr,ecr;

char flag_ser_par;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0))
	typedef pid_t _thread_hdl_;
#else
	typedef struct task_struct * _thread_hdl_;
#endif

int start_kthread(_thread_hdl_ *t_hdl, int (*threadfn)(void *data), void *data, const char *name)
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0))
	*t_hdl = kernel_thread(threadfn, data, CLONE_FS|CLONE_FILES);
	if(*t_hdl < 0)
#else
	*t_hdl = kthread_run(threadfn, data, name);
	if(IS_ERR(*t_hdl))
#endif
		return 0;
	
	return -1;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0))
	static pid_t thread_pid;
#else
	static struct task_struct * thread_pid;
#endif

/* parport private structure */
struct parport_mos7705_private 
{
	struct usb_device *usbdev;
	void *irqhandle;
	unsigned int irqpipe;
	unsigned char reg[7];  /* USB registers */
};

/****************************************************************************
 * moschip7705_parport_device
 *              Structure defining MCS7705, usb serail device
 ****************************************************************************/
//added
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,15)
static struct usb_serial_driver moschip7705_parport_device = {
     	.driver                 = {
                                       .owner  = THIS_MODULE,
                                        .name   = "moschip7705",
                                },
        .description            = DRIVER_DESC,
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,15)
static struct usb_serial_device_type moschip7705_parport_device = {
        .owner                  = THIS_MODULE,
        .name                   = "Moschip USB Parallel Adapter",
        .short_name             = "moschip7705",
#endif 
	.id_table               = id_table_combined7705,

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,26)
	.num_interrupt_in	= 1,
	.num_bulk_in		= 1, //2,
	.num_bulk_out		= 1, //2,
	.num_ports		= 1, //2,
#endif
	.open			= mos7705_open,
	.close		  	= mos7705_close,
	.throttle		= mos7705_throttle,
	.unthrottle		= mos7705_unthrottle,
	.attach			= mos7705_startup,
	.calc_num_ports		= mos7705_calc_num_ports,
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,30) 
	.shutdown		= mos7705_shutdown,
#else
	.disconnect		= mos7705_shutdown,
#endif
	.ioctl			= mos7705_ioctl,
	.set_termios		= mos7705_set_termios,
	.write			= mos7705_write,
	.write_room		= mos7705_write_room,
	.chars_in_buffer	= mos7705_chars_in_buffer,
	.break_ctl		= mos7705_break, 
	.read_bulk_callback	= mos7705_bulk_in_callback, 
};

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,5,0) 
static struct usb_driver io_driver = {
	#if LINUX_VERSION_CODE <=  KERNEL_VERSION(2,6,14)  
	.owner =	THIS_MODULE,
	#endif
	.name =		"moschip7705",
	.probe =	usb_serial_probe,
	.disconnect =	usb_serial_disconnect,
	.id_table =	id_table_combined,
};
#endif

/************************************************************************/
/************************************************************************/
/*             I N T E R F A C E   F U N C T I O N S			*/
/*             I N T E R F A C E   F U N C T I O N S			*/
/************************************************************************/
/************************************************************************/

static inline void set_serial_private(struct usb_serial *serial, struct moschip_serial *data)
{
		usb_set_serial_data(serial, (void *)data );
}

static inline struct moschip_serial * get_serial_private(struct usb_serial *serial)
{
		return (struct moschip_serial*) usb_get_serial_data(serial);
}

static inline void set_port_private(struct usb_serial_port *port, struct moschip_port *data)
{
		usb_set_serial_port_data(port, (void*)data );
}

static inline struct moschip_port * get_port_private(struct usb_serial_port *port)
{
		return (struct moschip_port*) usb_get_serial_port_data(port);
}

/************************************************************************/
/************************************************************************/
/*            U S B   P A R P O R T   F U N C T I O N S                 */
/*            U S B   P A R P O R T   F U N C T I O N S                 */
/************************************************************************/
/************************************************************************/


/*****************************************************************************
 * ECR modes
 *****************************************************************************/

#define ECR_SPP 00
#define ECR_PS2 01
#define ECR_PPF 02
#define ECR_ECP 03
#define ECR_EPP 04

/*****************************************************************************
 * change_mode_mos
 *	this function will be used to change ECR modes
 * Input : 2 Input
 *			pointer to the parport,
 *		        mode to switch to	
 *****************************************************************************/


static void change_mode_mos(struct parport *pp, int m)
{
	unsigned char data;

	DPRINTK("%s\n","Entering...");

        data = 0x00;
        SendMosCmd(pp,MOS_READ,0,0x0A, &data);

	data = data | (m<<5);	
        SendMosCmd(pp,MOS_WRITE,0,0x0A, &data);

}

/*****************************************************************************
 * clear_epp_timeout
 *	this function will be used to clear TIMEOUT BIT in EPP MODE
 * Input : 1 Input
 *			pointer to the parport,
 *****************************************************************************/
 
static int clear_epp_timeout(struct parport *pp)
{
	unsigned char stat;

	DPRINTK("%s\n","Entering...");

	stat = 0x00;
	SendMosCmd(pp,MOS_READ,0,0x01, &stat);

	return stat & 1;

}

/*****************************************************************************
 * mos7705_irq - Future Use
 *	
 * Input : 4 Input
 *			
 *****************************************************************************/
#if 0
static int mos7705_irq(int usbstatus, void *buffer, int len, void *dev_id)
{
	struct parport *pp = (struct parport *)dev_id;
	struct parport_mos7705_private *priv = pp->private_data;	

	if (usbstatus != USB_ST_NOERROR || len < 4 || !buffer)
		return 1;
	memcpy(priv->reg, buffer, 4);
	/* if nAck interrupts are enabled and we have an interrupt, call the interrupt procedure */
	if (priv->reg[2] & priv->reg[1] & 0x10)
		parport_generic_irq(0, pp, NULL);
	return 1;
}
#endif

/*****************************************************************************
 * parport_mos7705_write_data
 *	this function will be used to write data to Reg0 - Data Reg 
 * Input : 2 Input
 *			pointer to the parport,
 *			character to be written
 *****************************************************************************/

static void parport_mos7705_write_data(struct parport *pp, unsigned char d)
{
	DPRINTK("%s\n","Entering...");

	SendMosCmd(pp,MOS_WRITE,0,0x00, &d);

	DPRINTK("%s\n","Leaving...");

}

/*****************************************************************************
 * parport_mos7705_read_data
 *	this function will be used to read data from Reg0 - Data Reg 
 * Input : 1 Input
 *			pointer to the parport
 *****************************************************************************/

static unsigned char parport_mos7705_read_data(struct parport *pp)
{
	unsigned char ret;

	DPRINTK("%s\n","Entering...");

	SendMosCmd(pp,MOS_READ,0,0x00, &ret);
	
	DPRINTK("%s\n","Leaving...");
	return ret;

}

/*****************************************************************************
 * parport_mos7705_write_control 
 *	this function will be used to write DCR Reg 
 * Input : 2 Input
 *			pointer to the parport,
 *			value to be written
 *****************************************************************************/

static void parport_mos7705_write_control(struct parport *pp, unsigned char d)
{
	struct parport_mos7705_private *priv = pp->private_data;	

	DPRINTK("%s\n","Entering...");

	d = (d & 0xf) | (priv->reg[1] & 0xf0);
        
	SendMosCmd(pp,MOS_WRITE,0,0x02, &d);

	priv->reg[1] = d;

	DPRINTK("%s\n","Leaving...");
}

/*****************************************************************************
 * parport_mos7705_read_control 
 *	this function will be used to read DCR Reg 
 * Input : 1 Input
 *			pointer to the parport
 *****************************************************************************/

static unsigned char parport_mos7705_read_control(struct parport *pp)
{
/*	struct parport_mos7705_private *priv = pp->private_data;	

	DPRINTK("%s\n","Entering...");

	return priv->reg[1] & 0xf; // Use sft cpy 

	DPRINTK("%s\n","Leaving...");
*/
	unsigned char ret;
	unsigned char control_reg;

	DPRINTK("%s\n","Entering...");

	ret = SendMosCmd(pp,MOS_READ,0,0x02, &control_reg);

	DPRINTK("%s\n","Leaving...");

	return control_reg;

}

/*****************************************************************************
 * parport_mos7705_frob_control 
 *	this function will be used to alter ECR Reg 
 * Input : 3 Input
 *			pointer to the parport,
 *			mask value,
 *			value to be written		
 *****************************************************************************/

static unsigned char parport_mos7705_frob_control(struct parport *pp, unsigned char mask, unsigned char val)
{
	unsigned char d;
	struct parport_mos7705_private *priv = pp->private_data;	

	DPRINTK("%s\n","Entering...");

	DPRINTK("%x	%x\n",(unsigned int)pp,(unsigned int)priv);

	mask &= 0x0f;
	val &= 0x0f;
	d = (priv->reg[1] & (~mask)) ^ val;

	SendMosCmd(pp,MOS_WRITE,0,0x02, &d);
	priv->reg[1] = d;

	DPRINTK("%s\n","Leaving...");

	return d & 0xf;

}

/*****************************************************************************
 * parport_mos7705_read_status 
 *	this function will be used to read DSR Reg
 * Input : 1 Input
 *			pointer to the parport
 *****************************************************************************/

static unsigned char parport_mos7705_read_status(struct parport *pp)
{
	unsigned char data;

	DPRINTK("%s\n","Entering...");

        SendMosCmd(pp,MOS_READ,0,0x01, &data);

	DPRINTK("%s\n","Leaving...");

	return data & 0xf8;

}


/*****************************************************************************
 * parport_mos7705_disable_irq - Future Use
 *	this function will be used to disable irq 
 * Input : 1 Input
 *			pointer to the parport
 *****************************************************************************/
static void parport_mos7705_disable_irq(struct parport *pp)
{
 #if 0
	unsigned char d;
	struct parport_mos7705_private *priv = pp->private_data;	

	DPRINTK("%s\n","Entering...");
 #endif
}

/*****************************************************************************
 * parport_mos7705_enable_irq - Future Use
 *	this function will be used to enable irq 
 * Input : 1 Input
 *			pointer to the parport
 *****************************************************************************/

static void parport_mos7705_enable_irq(struct parport *pp)
{
 #if 0
	unsigned char d;
	struct parport_mos7705_private *priv = pp->private_data;	

	DPRINTK("%s\n","Entering...");
#endif
}

/*****************************************************************************
 * parport_mos7705_data_forward
 *	this function will be used to set P[0-7] to O/P mode
 * Input : 1 Input
 *			pointer to the parport
 *****************************************************************************/

static void parport_mos7705_data_forward (struct parport *pp)
{
	unsigned char d;
	struct parport_mos7705_private *priv = pp->private_data;	

	DPRINTK("%s\n","Entering...");

	d = priv->reg[1] & ~0x20;

	SendMosCmd(pp,MOS_WRITE,0,0x02, &d);
	priv->reg[1] = d;

	DPRINTK("%s\n","Leaving...");
}

/*****************************************************************************
 * parport_mos7705_data_reverse
 *	this function will be used to set P[0-7] to I/P mode
 * Input : 1 Input
 *			pointer to the parport
 *****************************************************************************/
static void parport_mos7705_data_reverse (struct parport *pp)
{
	unsigned char d;
	struct parport_mos7705_private *priv = pp->private_data;

	DPRINTK("%s\n","Entering...");

	d = priv->reg[1] | 0x20;

	SendMosCmd(pp,MOS_WRITE,0,0x02, &d);

	priv->reg[1] = d;

	DPRINTK("%s\n","Leaving...");

}

/*****************************************************************************
 * parport_mos7705_init_state
 *	this function will be used to set DCR & ECR to initial values
 * Input : 2 Input
 *			pointer to pardevice
 *			pointer to parport_state
 *****************************************************************************/

static void parport_mos7705_init_state(struct pardevice *dev, struct parport_state *s)
{
	DPRINTK("%s\n","Entering...");

	//s->u.pc.ctr = 0x0c | (dev->irq_func ? 0x10 : 0x0);
	s->u.pc.ctr = 0x0c;
	s->u.pc.ecr = MOS_ECR_MODE_INIT;

	DPRINTK("%s\n","Leaving...");
}

/*****************************************************************************
 * parport_mos7705_save_state
 *	this function will be used to save DCR & ECR values
 * Input : 2 Input
 *			pointer to parport
 *			pointer to parport_state
 *****************************************************************************/

static void parport_mos7705_save_state(struct parport *pp, struct parport_state *s)
{
	unsigned char data;	
	struct parport_mos7705_private *priv = pp->private_data;	

	DPRINTK("%s\n","Entering...");
        
	data = 0x00;
        SendMosCmd(pp,MOS_READ,0,0x02, &data);
	s->u.pc.ctr = data;
	priv->reg[1] = data;

	data = 0x00;
        SendMosCmd(pp,MOS_READ,0,0x0A, &data);
	s->u.pc.ecr = data;
	priv->reg[2] = data; 

	DPRINTK("%s\n","Leaving...");
}

static int restore_state_thread(void *vpp)
{
	struct parport *pp;
	pp=vpp;

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,8,0)
	daemonize("otg_hcd_thread");
#endif

	allow_signal(SIGKILL);

	DPRINTK("%s \n","thread ...\n");
	while ( 1 ) {
		//interruptible_sleep_on(&restore_state_event);
		thread_cond = 0;
		wait_event_interruptible(restore_state_event,(thread_cond==1));
		//wait_event(restore_state_event,(thread_cond==1));

       		DPRINTK("%s \n","restore_state_thread:Thread Wait...\n");
		if(ThreadState) {
			ThreadState = 0;
          	
			DPRINTK("%s \n","Stopping restore_state_thread thread ...");
			up(&thread_complete);
          		break;
       		}

		SendMosCmd(pp,MOS_WRITE,0,0x02, &dcr);
		SendMosCmd(pp,MOS_WRITE,0,0x0A, &ecr);
	}
	return 0;
} 
/*****************************************************************************
 * parport_mos7705_restore_state
 *	this function will be used to restore DCR & ECR values  
 * Input : 2 Input
 *			pointer to parport
 *			pointer to parport_state
 *****************************************************************************/

static void parport_mos7705_restore_state(struct parport *pp, struct parport_state *s)
{
	unsigned char data;	

	DPRINTK("%s\n","Entering...");

	if(s==NULL)
	  return;

	data = s->u.pc.ctr;
//        SendMosCmd(pp,MOS_WRITE,0,0x02, &data);
	DPRINTK("%s %x\n","val restrd DCR=",data);
	dcr = data;

	data = s->u.pc.ecr;
//        SendMosCmd(pp,MOS_WRITE,0,0x0A, &data);
	DPRINTK("%s %x\n","val restrd ECR=",data);
	ecr = data;

	//wake_up_interruptible(&restore_state_event);
	wake_up(&restore_state_event); 
	thread_cond = 1;

	DPRINTK("%s\n","Leaving...");

}

/*****************************************************************************
 * parport_mos7705_epp_read_data
 *	this function will be used to read data in EPP mode
 * Input : 4 Input
 *			pointer to the parport
 *			buffer
 *			length
 *			flags
 *****************************************************************************/

static size_t parport_mos7705_epp_read_data(struct parport *pp, void *buf, size_t length, int flags)
{
	size_t got = 0;
	unsigned char data;

	DPRINTK("%s\n","Entering...");

	change_mode_mos(pp, ECR_EPP);

	for (; got < length; got++) 
	{
	
        	SendMosCmd(pp,MOS_WRITE,0,0x04,(unsigned char *) buf);

		    buf++;

        	SendMosCmd(pp,MOS_READ,0,0x01,&data);

		if (data & 0x01) 
		{
			clear_epp_timeout(pp);
			break;
		}
	}
	
	change_mode_mos(pp, ECR_PS2);
	
	DPRINTK("%s\n","Leaving...");

	return got;

}

/*****************************************************************************
 * parport_mos7705_epp_write_data
 *	this function will be used to write data in EPP mode
 * Input : 4 Input
 *			pointer to the parport
 *			buffer
 *			length
 *			flags
 *****************************************************************************/

static size_t parport_mos7705_epp_write_data(struct parport *pp, const void *buf, size_t length, int flags)
{
	int i;
	int rlen;
	struct parport_mos7705_private *priv = pp->private_data;
	struct usb_device *usbdev = priv->usbdev;

	if (!usbdev)
		return 0;

	DPRINTK("%s\n","Entering...");

	change_mode_mos(pp, ECR_EPP);

	i = usb_bulk_msg(usbdev, usb_sndbulkpipe(usbdev, 2), (void *)buf, length, &rlen, HZ*20);
	if (i)
		printk(KERN_ERR "mos7705: sendbulk ep 2 buf %p len %Zu rlen %u\n", buf, length, rlen);

	change_mode_mos(pp, ECR_PS2); 

	DPRINTK("%s\n","Leaving...");

	return rlen;
}

/*****************************************************************************
 * parport_mos7705_epp_read_addr
 *	this function will be used to read addrs in EPP mode
 * Input : 4 Input
 *			pointer to the parport
 *			buffer
 *			length
 *			flags
 *****************************************************************************/

static size_t parport_mos7705_epp_read_addr(struct parport *pp, void *buf, size_t length, int flags)
{
	size_t got = 0;
	unsigned char data;

	DPRINTK("%s\n","Entering...");

	change_mode_mos(pp, ECR_EPP);

	for (; got < length; got++) 
	{
        	SendMosCmd(pp,MOS_READ,0,0x03, (unsigned char *)buf);

			buf++;

			data = 0x00;
        	SendMosCmd(pp,MOS_READ,0,0x01, &data);

		if (data & 0x01) 
		{
			clear_epp_timeout(pp);
			break;
		}
	}

	change_mode_mos(pp, ECR_PS2);

	DPRINTK("%s\n","Leaving...");
	return got;

}

/*****************************************************************************
 * parport_mos7705_epp_write_addr
 *	this function will be used to write addrs in EPP mode
 * Input : 4 Input
 *			pointer to the parport
 *			buffer
 *			length
 *			flags
 *****************************************************************************/

static size_t parport_mos7705_epp_write_addr(struct parport *pp, const void *buf, size_t length, int flags)
{
	unsigned char data;
	size_t written = 0;

	DPRINTK("%s\n","Entering...");

	change_mode_mos(pp, ECR_EPP);

	for (; written < length; written++) 
	{

        	SendMosCmd(pp,MOS_WRITE,0,0x03,(unsigned char *) buf);

			buf++;

			data = 0x00;
        	SendMosCmd(pp,MOS_READ,0,0x01, &data);

		if (data & 0x01) 
		{
			clear_epp_timeout(pp);
			break;
		}
	}
	change_mode_mos(pp, ECR_PS2);

	DPRINTK("%s\n","Leaving...");
	return written;

}

/*****************************************************************************
 * parport_mos7705_ecp_write_data
 *	this function will be used to write data in ECP mode
 * Input : 4 Input
 *			pointer to the parport
 *			buffer
 *			length
 *			flags
 *****************************************************************************/

static size_t parport_mos7705_ecp_write_data(struct parport *pp, const void *buffer, size_t len, int flags)
{
	int i;
	int rlen;
	struct parport_mos7705_private *priv = pp->private_data;
	struct usb_device *usbdev = priv->usbdev;

	if (!usbdev)
		return 0;

	DPRINTK("%s\n","Entering...");

	change_mode_mos(pp, ECR_ECP);


	i = usb_bulk_msg(usbdev, usb_sndbulkpipe(usbdev, 2), (void *)buffer, len, &rlen, HZ*20);
	if (i)
		printk(KERN_ERR "mos7705: sendbulk ep 2 buf %p len %Zu rlen %u\n", buffer, len, rlen);

	change_mode_mos(pp, ECR_PS2);

	DPRINTK("%s\n","Leaving...");
	return rlen;
}

/*****************************************************************************
 * parport_mos7705_ecp_read_data
 *	this function will be used to read data in ECP mode
 * Input : 4 Input
 *			pointer to the parport
 *			buffer
 *			length
 *			flags
 *****************************************************************************/

static size_t parport_mos7705_ecp_read_data(struct parport *pp, void *buffer, size_t len, int flags)
{
	int i;
	int rlen;
	struct parport_mos7705_private *priv = pp->private_data;
	struct usb_device *usbdev = priv->usbdev;

	if (!usbdev)
		return 0;

	DPRINTK("%s\n","Entering...");

	change_mode_mos(pp, ECR_ECP);

	i = usb_bulk_msg(usbdev, usb_rcvbulkpipe(usbdev, 1), buffer, len, &rlen, HZ*20);
	if (i)
		printk(KERN_ERR "mos7705: recvbulk ep 1 buf %p len %Zu rlen %u\n", buffer, len, rlen);

	change_mode_mos(pp, ECR_PS2);

	DPRINTK("%s\n","Leaving...");
	return rlen;
}

/*****************************************************************************
 * parport_mos7705_ecp_write_addr
 * This function will be used to write addrs in ECP mode
 * Input : 4 Input
 *			pointer to the parport
 *			buffer
 *			length
 *			flags
 *****************************************************************************/

static size_t parport_mos7705_ecp_write_addr(struct parport *pp, const void *buffer, size_t len, int flags)
{
	size_t written = 0;

	DPRINTK("%s\n","Entering...");

	change_mode_mos(pp, ECR_ECP);

	for (; written < len; written++) 
	{

        	SendMosCmd(pp,MOS_WRITE,0,0x08, (void *)buffer);

			buffer++;

	}

	change_mode_mos(pp, ECR_PS2);

	DPRINTK("%s\n","Leaving...");
	return written;

}

/*****************************************************************************
 * parport_mos7705_write_compat
 *	this function will be used to write data in PPF/CB-FIFO mode
 * Input : 4 Input
 *			pointer to the parport
 *			buffer
 *			length
 *			flags
 *****************************************************************************/
static size_t parport_mos7705_write_compat(struct parport *pp, const void *buffer, size_t len, int flags)
{
	int i;
	int rlen;
	struct parport_mos7705_private *priv = pp->private_data;
	struct usb_device *usbdev = priv->usbdev;


	if (!usbdev)
		return 0;

	change_mode_mos(pp, ECR_PPF);

	i = usb_bulk_msg(usbdev, usb_sndbulkpipe(usbdev, 2), (void *)buffer, len, &rlen, HZ*20);

	DPRINTK("%s %x\n","length = ",len);
	if (i)
		printk(KERN_ERR "mos7705: sendbulk ep 2 buf %p len %Zu rlen %u\n", buffer, len, rlen);

	change_mode_mos(pp, ECR_PS2);

	DPRINTK("%s\n","Leaving...");
	return rlen;
}

/****************************************************************************
 * parport_mos7705_ops
 *	this structure holds the function names of all the possible 
 *      operations on parport
****************************************************************************/
static struct parport_operations parport_mos7705_ops = 
{
        .owner =                THIS_MODULE,
        .write_data =           parport_mos7705_write_data,
        .read_data =            parport_mos7705_read_data,

        .write_control =        parport_mos7705_write_control,
        .read_control =         parport_mos7705_read_control,
        .frob_control =         parport_mos7705_frob_control,

        .read_status =          parport_mos7705_read_status,

        .enable_irq =           parport_mos7705_enable_irq,
        .disable_irq =          parport_mos7705_disable_irq,

        .data_forward =         parport_mos7705_data_forward,
        .data_reverse =         parport_mos7705_data_reverse,

        .init_state =           parport_mos7705_init_state,
        .save_state =           parport_mos7705_save_state,
        .restore_state =        parport_mos7705_restore_state,

        .epp_write_data =       parport_mos7705_epp_write_data,
        .epp_read_data =        parport_mos7705_epp_read_data,
        .epp_write_addr =       parport_mos7705_epp_write_addr,
        .epp_read_addr =        parport_mos7705_epp_read_addr,

        .ecp_write_data =       parport_mos7705_ecp_write_data,
        .ecp_read_data =        parport_mos7705_ecp_read_data,
        .ecp_write_addr =       parport_mos7705_ecp_write_addr,

        .compat_write_data =    parport_mos7705_write_compat,
        .nibble_read_data =     parport_ieee1284_read_nibble,
        .byte_read_data =       parport_ieee1284_read_byte,
};

/************************************************************************/
/************************************************************************/
/*            U S B  C A L L B A C K   F U N C T I O N S                */
/*            U S B  C A L L B A C K   F U N C T I O N S                */
/************************************************************************/
/************************************************************************/

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
static void mos7705_bulk_in_callback (struct urb *urb)
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
static void mos7705_bulk_in_callback (struct urb *urb, struct pt_regs *regs)
#endif
{

}

/*****************************************************************************
 * SendMosCmd
 *	this function will be used for sending command to device
 * Input : 5 Input
 *			pointer to the serial device,
 *			request type
 *			value
 *			register index
 *			pointer to data/buffer
 *****************************************************************************/
static int SendMosCmd(void *pp, __u8 request, __u16 value, __u16 index, void *data)
{
        int timeout;
        int status;
        __u8 requesttype;
        __u16 size;
	unsigned int Pipe;
	struct usb_device *usbdev;
	
	if(value) /* value -- 0 == Parport   1: Serail */
	{
			DPRINTK("%s","Serial not supported.... \n");
			return -1;
	}
	else
	{
		struct parport_mos7705_private *priv =(struct parport_mos7705_private *)((struct parport *)pp)->private_data;
		usbdev = priv->usbdev;

	}

        size = 0x00;
        timeout = MOS_WDR_TIMEOUT;

        if(value < MOS_MAX_PORT)
                value = (value+1)*0x100 ;
        else
                value = 0x0000;

        if(request==MOS_WRITE)
    	{
            request = (__u8)MOS_WRITE;
            requesttype = (__u8)0x40;
            value  = value + (__u16)*((unsigned char *)data);
            data = NULL;
	    Pipe = usb_sndctrlpipe(usbdev, 0);
	}
    	else
    	{
            request = (__u8)MOS_READ;
            requesttype = (__u8)0xC0;
     	    size = 0x01;
	    Pipe = usb_rcvctrlpipe(usbdev,0); 
    	}


    status = usb_control_msg(                       \
    usbdev,                                         \
    Pipe,			                    \
    request,                                        \
    requesttype,                                    \
    value,                                          \
    index,                                          \
    data,                                           \
    size,                                           \
    timeout );


    if(status<0)
    {
	DPRINTK("Write fail-Val %x index %x-status:%d\n",value,index,status);
    }

/*        if(request==MOS_READ)
                DPRINTK("responded data %x \n",*((unsigned char*) data));
*/
        return status;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static int mos7705_open (struct usb_serial_port *port, struct file * filp)
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
static int mos7705_open (struct tty_struct *tty,struct usb_serial_port *port, struct file * filp)
#else
static int mos7705_open (struct tty_struct *tty,struct usb_serial_port *port)
#endif
{
	return 0;
}

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27))
static void mos7705_close (struct usb_serial_port *port, struct file * filp)
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30))
static void mos7705_close(struct tty_struct *tty,struct usb_serial_port *port, struct file *filp)
#else
static void mos7705_close(struct usb_serial_port *port)
#endif
{

}   

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static void mos7705_break (struct usb_serial_port *port, int break_state)
#else
static void mos7705_break(struct tty_struct *tty, int break_state)
#endif
{

}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static int mos7705_write_room (struct usb_serial_port *port)
#else
static int  mos7705_write_room(struct tty_struct *tty)
#endif
{
        return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static int mos7705_chars_in_buffer (struct usb_serial_port *port)
#else
static int  mos7705_chars_in_buffer(struct tty_struct *tty)
#endif
{
    	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27) && LINUX_VERSION_CODE > KERNEL_VERSION(2,6,10)
static int mos7705_write (struct usb_serial_port *port, const unsigned char *data, int count)
#elif LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,10)
static int mos7705_write (struct usb_serial_port *port, int from_user, const unsigned char *data, int count)
#else
static int mos7705_write (struct tty_struct *tty,struct usb_serial_port *port, const unsigned char *data, int count)
#endif
{
	return 0;
}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static void mos7705_throttle(struct usb_serial_port *port)
#else
static void mos7705_throttle(struct tty_struct *tty)
#endif
{

}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static void mos7705_unthrottle (struct usb_serial_port *port)
#else
static void mos7705_unthrottle(struct tty_struct *tty)
#endif
{

}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27) && LINUX_VERSION_CODE > KERNEL_VERSION(2,6,19)
static void mos7705_set_termios (struct usb_serial_port *port, struct ktermios *old_termios)
#elif LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,19)
static void mos7705_set_termios (struct usb_serial_port *port, struct termios *old_termios)
#else
static void mos7705_set_termios(struct tty_struct *tty,struct usb_serial_port *port,struct ktermios *old_termios)
#endif
{

}

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static int mos7705_ioctl (struct usb_serial_port *port, struct file *file, unsigned int cmd, unsigned long arg)
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39)
static int mos7705_ioctl(struct tty_struct *tty, struct file *file,unsigned int cmd, unsigned long arg)
#else
static int mos7705_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg)
#endif
{
	return -ENOIOCTLCMD;
}

static int mos7705_calc_num_ports(struct usb_serial *serial)
{
	return 1;
}

/****************************************************************************
 * mos7705_startup
 ****************************************************************************/
static int mos7705_startup (struct usb_serial *serial)
{
	struct moschip_serial *mos7705_serial;
	struct moschip_port *mos7705_port;
	struct usb_device *dev;
	static struct parport *pp;

	int i;

	/* parport */
        unsigned char data;
        struct parport_mos7705_private *priv;

	if(!serial)
	{
		DPRINTK("%s\n","Invalid Handler");
		return -1;
	}

	dev = serial->dev;
	
	DPRINTK("%s\n","Entering..\n.");
	/* create our private serial structure */
        #if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,17)
        mos7705_serial = kzalloc (sizeof(struct moschip_serial), GFP_KERNEL);
        #endif
        #if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,17)
        mos7705_serial = kmalloc (sizeof(struct moschip_serial), GFP_KERNEL);
        #endif

	//mos7705_serial = kmalloc (sizeof(struct moschip_serial), GFP_KERNEL);
	if (mos7705_serial == NULL) 
	{
		printk("%s - Out of memory", __FUNCTION__);
		return -ENOMEM;
	}

	/* resetting the private structure field values to zero */
	memset (mos7705_serial, 0, sizeof(struct moschip_serial));

	mos7705_serial->serial = serial;
	set_serial_private(serial,mos7705_serial);

	/* we set up the pointers to the endpoints in the mos7705_open *
	 * function, as the structures aren't created yet.             */
	printk("StartUp: the num of ports are : %d\n",serial->num_ports);
	/* set up port private structures */
	for (i = 0; i < serial->num_ports; ++i) 
	{
	        mos7705_port = kmalloc(sizeof(struct moschip_port), GFP_KERNEL);
		if (mos7705_port == NULL) 
		{
			printk("%s - Out of memory", __FUNCTION__);
			set_serial_private(serial,NULL);
			kfree(mos7705_serial);
			return -ENOMEM;
		}
		memset (mos7705_port, 0, sizeof(struct moschip_port));


	/* Initialize all port interrupt end point to port 0 int endpoint *
	 * Our device has only one interrupt end point comman to all port */

	//	serial->port[i]->interrupt_in_endpointAddress = serial->port[0]->interrupt_in_endpointAddress;

		mos7705_port->port = serial->port[i];
		set_port_private(serial->port[i],mos7705_port);

	DPRINTK("port number is %d \n",serial->port[i]->number);
	DPRINTK("port is %x \n",(unsigned int)serial->port[i]);
	DPRINTK("serial number is %d \n",serial->minor);

	}


	/* setting configuration feature to one */
	usb_control_msg (serial->dev, usb_sndctrlpipe(serial->dev, 0), (__u8)0x03, 0x00,0x01,0x00, 0x00, 0x00, 5*HZ);


	/* parport */
	if (!(priv = kmalloc(sizeof(struct parport_mos7705_private), GFP_KERNEL)))
                return -ENOMEM;
	
	/* resetting the private structure field values to zero */
        memset (priv, 0, sizeof(struct parport_mos7705_private));


        if (!(pp = parport_register_port(0, PARPORT_IRQ_NONE, PARPORT_DMA_NONE,&parport_mos7705_ops))) 
	{
                printk(KERN_WARNING "Could not register parport\n");
                goto probe_abort;
        }

        pp->private_data = priv;
        priv->usbdev = serial->dev;


        pp->modes = PARPORT_MODE_PCSPP | PARPORT_MODE_TRISTATE | PARPORT_MODE_EPP | PARPORT_MODE_ECP | PARPORT_MODE_COMPAT;

	mos7705_serial->parp = pp;	
	DPRINTK("parport is %x \n",(unsigned int)pp);

	/* Initialize the wait event for restore_state_thread */
	init_waitqueue_head(&restore_state_event);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,37)
        init_MUTEX_LOCKED(&thread_complete);
#else
	sema_init(&thread_complete, 0);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0)
	thread_pid = kernel_thread(restore_state_thread,pp,CLONE_KERNEL);
	if(thread_pid < 0) {
		printk("HCD thread creation failed \n");
		return -ENODEV;
	}
#else 
	if(!start_kthread(&thread_pid, restore_state_thread, pp, "pp_thread"))
 		printk("HCD thread creation failed \n");
#endif



        DPRINTK("%s\n","Writing Registers Started\n");

     #if 0 
        data = 0x30;
        SendMosCmd(serial,MOS_READ,MOS_MAX_PORT,0x04, &data);
        data = 0x80;
        SendMosCmd(serial,MOS_WRITE,MOS_MAX_PORT,0x04, &data);
        data = 0x00;
        SendMosCmd(serial,MOS_WRITE,MOS_MAX_PORT,0x04, &data);
      #endif

        data = 0x00;
        SendMosCmd(pp,MOS_READ,0,0x02, &data); 
        data = 0x0C;
        SendMosCmd(pp,MOS_WRITE,0,0x02, &data);
        priv->reg[1] = data; /* DCR */
        
	data = 0x00;
        SendMosCmd(pp,MOS_READ,0,0x0A, &data);
        data = MOS_ECR_MODE_INIT;
        SendMosCmd(pp,MOS_WRITE,0,0x0A, &data);
        priv->reg[2] = data;
	
        DPRINTK("%s\n","Writing Registers Over\n");

	parport_announce_port(pp);

        DPRINTK("%s\n","Leaving : normal EXIT...\n");
	return 0;

probe_abort:
        kfree(priv);
        return -EPERM;

}


/****************************************************************************
 * mos7705_shutdown
 *	This function is called whenever the device is removed from the usb bus.
 ****************************************************************************/

static void mos7705_shutdown (struct usb_serial *serial)
{
	int i;
	struct parport_mos7705_private *priv;
	struct parport *pp;
	struct moschip_serial *mos7705_serial;
	struct moschip_port	*mos7705_port; 
	
	DPRINTK("%s\n","mos7705_shutdown : Entering ...\n");

	if(!serial)
	{
		DPRINTK("%s","Invalid Handler \n");
		return;
	}
	
	mos7705_serial = get_serial_private(serial);
	pp = mos7705_serial->parp;
	
	priv = pp->private_data;  
	ThreadState = 1;
	//wake_up_interruptible(&restore_state_event);
	wake_up(&restore_state_event); 
	thread_cond = 1;
	//wait here till the kernel thread breaks
	down_interruptible(&thread_complete);

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
	kill_proc(thread_pid, SIGKILL, 1); 
#else
	// Need to invoke the above call	
#endif 

	if (pp)
        {
              //  priv = pp->private_data;
                priv->usbdev = NULL;
              	
		pp->modes = 0x0; 
	        parport_remove_port(pp);
              	parport_put_port(pp);
                kfree(priv);
                DPRINTK("%s\n","FREE, parallel private structure");
        }  
	
	/* free private structure allocated for serial port  * 
	 * stop reads and writes on all ports   	     */

	//DPRINTK("parport is %x \n",(unsigned int)pp);
	for (i=0; i < serial->num_ports; ++i) 
	{
		mos7705_port = get_port_private(serial->port[i]);
                //kfree(mos7705_port->ctrl_buf);
       #if LINUX_VERSION_CODE > KERNEL_VERSION(2,6,10)
                //usb_kill_urb(mos7705_port->control_urb);
        #endif
        #if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,10)
       	       //usb_unlink_urb(mos7705_port->control_urb);
        #endif
                kfree(mos7705_port);
               	set_port_private(serial->port[i],NULL);
        }  



	/* free private structure allocated for serial device */
		
	mos7705_serial->parp=NULL; 

	kfree(get_serial_private(serial)); //free the mos7705_serial context
	set_serial_private(serial,NULL);
	DPRINTK("%s\n","FREE, serial private structure");

	DPRINTK("%s\n","mos7705_shutdown : end ...\n");

}

#if LINUX_VERSION_CODE < KERNEL_VERSION(3,4,0)
/****************************************************************************
 *	This is called by the module subsystem, or on startup to initialize us
 ****************************************************************************/

 int __init moschip7705_init(void)
{
	int retval;

	DPRINTK("%s \n"," mos7705_init :entering..........");

        /* Register with the usb serial */

	retval = usb_serial_register (&moschip7705_parport_device);

	if(retval)
		goto failed_port_device_register;

	info(DRIVER_DESC " " DRIVER_VERSION);


 	/* Register with the usb */
	retval = usb_register(&io_driver);

	if (retval) 
		goto failed_usb_register;

	if(retval == 0) 
	{
		DPRINTK("%s\n","Leaving...");
		return 0;
	}


failed_usb_register:

	usb_serial_deregister(&moschip7705_parport_device);


failed_port_device_register:
	return retval;
}

/****************************************************************************
 * moschip7705_exit
 *	Called when the driver is about to be unloaded.
 ****************************************************************************/
void __exit moschip7705_exit (void)
{

	DPRINTK("%s \n"," mos7705_exit :entering..........");
	usb_deregister (&io_driver);
	usb_serial_deregister (&moschip7705_parport_device);

	DPRINTK("%s\n","Leaving...");
}

module_init(moschip7705_init);
module_exit(moschip7705_exit);
#elif LINUX_VERSION_CODE < KERNEL_VERSION(3,5,0)
static struct usb_serial_driver * const serial_drivers[] = {
	&moschip7705_parport_device, NULL
};

module_usb_serial_driver(io_driver, serial_drivers);
#else
static struct usb_serial_driver * const serial_drivers[] = {
	&moschip7705_parport_device, NULL
};

module_usb_serial_driver(serial_drivers, id_table_combined7705);
#endif

/* Module information */
MODULE_DESCRIPTION( DRIVER_DESC );
MODULE_LICENSE("GPL");

