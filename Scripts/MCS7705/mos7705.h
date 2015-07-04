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
 *** File: mos7705.h 
 ***		
 *** Purpose	  : It contains defines that will be used by mosc7705.c 
 ***
 ***
 *************************************************************************/
#include <linux/version.h>

#if !defined(_MOS_CIP_H_)
#define	_MOS_CIP_H_


#define MAX_RS232_PORTS		1	/* Max # of RS-232 ports per device */


/* 
 *  All typedef goes here
 */


/* typedefs that the insideout headers need */

#ifndef TRUE
	#define TRUE		(1)
#endif

#ifndef FALSE
	#define FALSE		(0)
#endif

#ifndef LOW8
	#define LOW8(val)	((unsigned char)(val & 0xff))
#endif

#ifndef HIGH8
	#define HIGH8(val)	((unsigned char)((val & 0xff00) >> 8))
#endif

#ifndef NUM_ENTRIES
	#define NUM_ENTRIES(x)	(sizeof(x)/sizeof((x)[0]))
#endif

#ifndef __KERNEL__
	#define __KERNEL__
#endif



#define MAX_SERIALNUMBER_LEN 12

/* The following table is used to map the USBx port number to 
 * the device serial number (or physical USB path), */

#define MAX_MOSPORTS	2
#define MAX_NAME_LEN	64

/* For higher baud Rates use TIOCEXBAUD */
#define TIOCEXBAUD	0x5462  

#define BAUD_1152	0	/* 115200bps  * 1	*/
#define BAUD_2304	1	/* 230400bps  * 2	*/
#define BAUD_4032	2	/* 403200bps  * 3.5	*/
#define BAUD_4608	3	/* 460800bps  * 4	*/
#define BAUD_8064	4	/* 806400bps  * 7	*/
#define BAUD_9216	5	/* 921600bps  * 8	*/

#define CHASE_TIMEOUT		(5*HZ)		/* 5 seconds */
#define OPEN_TIMEOUT		(5*HZ)		/* 5 seconds */
#define COMMAND_TIMEOUT		(5*HZ)		/* 5 seconds */

#ifndef SERIAL_MAGIC
	#define SERIAL_MAGIC	0x6702
#endif

#define PORT_MAGIC		0x7301

/* vendor id and device id defines */

#define USB_VENDOR_ID_MOSCHIP		0x9710
//#define MOSCHIP_DEVICE_ID_7720		0x7720
//#define MOSCHIP_DEVICE_ID_7715		0x7715
#define MOSCHIP_DEVICE_ID_7705		0x7705

/* Product information read from the Moschip. Provided for later upgrade */


/* Interrupt Rotinue Defines	*/
	
#define SERIAL_IIR_RLS      0x06
#define SERIAL_IIR_RDA      0x04
#define SERIAL_IIR_CTI      0x0c
#define SERIAL_IIR_THR      0x02
#define SERIAL_IIR_MS       0x00

/*
 *  Emulation of the bit mask on the LINE STATUS REGISTER.
 */
#define SERIAL_LSR_DR       0x0001
#define SERIAL_LSR_OE       0x0002
#define SERIAL_LSR_PE       0x0004
#define SERIAL_LSR_FE       0x0008
#define SERIAL_LSR_BI       0x0010
#define SERIAL_LSR_THRE     0x0020
#define SERIAL_LSR_TEMT     0x0040
#define SERIAL_LSR_FIFOERR  0x0080

/*
 * URB POOL related defines
 */
#define NUM_URBS                        32     /* URB Count */
#define URB_TRANSFER_BUFFER_SIZE        1024	//32     /* URB Size  */

struct moschip_product_info 
{
	__u16	ProductId;		/* Product Identifier */
	__u8	NumPorts;		/* Number of ports on moschip */
	__u8	ProdInfoVer;		/* What version of structure is this? */

	__u32	IsServer        :1;	/* Set if Server */
	__u32	IsRS232         :1;	/* Set if RS-232 ports exist */
	__u32	IsRS422         :1;	/* Set if RS-422 ports exist */
	__u32	IsRS485         :1;	/* Set if RS-485 ports exist */
	__u32	IsReserved      :28;	/* Reserved for later expansion */

	__u8	CpuRev;			/* CPU revision level (chg only if s/w visible) */
	__u8	BoardRev;		/* PCB revision level (chg only if s/w visible) */

	__u8	ManufactureDescDate[3];	/* MM/DD/YY when descriptor template was compiled */
	__u8	Unused1[1];		/* Available */

};

static struct usb_device_id id_table_combined7705 [] = {
	{ USB_DEVICE(USB_VENDOR_ID_MOSCHIP,MOSCHIP_DEVICE_ID_7705) },
	{ } /* terminating entry */
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,8,0)
static struct usb_device_id id_table_combined [] = {
#else
static __devinitdata struct usb_device_id id_table_combined [] = {
#endif
	{ USB_DEVICE(USB_VENDOR_ID_MOSCHIP,MOSCHIP_DEVICE_ID_7705) },
	{ } /* terminating entry */
};

MODULE_DEVICE_TABLE (usb, id_table_combined);

/* This structure holds all of the local port information */

struct moschip_port 
{
	__u8			bulk_out_endpoint;   		/* the bulk out endpoint handle */
	unsigned char *		bulk_out_buffer;     		/* buffer used for the bulk out endpoint */	
	struct urb    *		write_urb;	     		/* write URB for this port */
	__u8	     		 bulk_in_endpoint;	     		/* the bulk in endpoint handle */
	unsigned char * 	bulk_in_buffer;	     		/* the buffer we use for the bulk in endpoint */
	struct urb    *		read_urb;	     		/* read URB for this port */
	__s16			rxBytesAvail;		/* the number of bytes that we need to read from this device */
	__s16			rxBytesRemaining;	/* the number of port bytes left to read */
	char			write_in_progress;	/* TRUE while a write URB is outstanding */
	__u8			shadowLCR;		/* last LCR value received */
	__u8			shadowMCR;		/* last MCR value received */
	__u8			shadowMSR;		/* last MSR value received */
	__u8			shadowLSR;		/* last LSR value received */
	__u8			shadowXonChar;		/* last value set as XON char in moschip */
	__u8			shadowXoffChar;		/* last value set as XOFF char in moschip */
	__u8			validDataMask;
	__u32			baudRate;
	char			open;
	char			openPending;
	char			commandPending;
	char			closePending;
	char			chaseResponsePending;
	int                     delta_msr_cond;
	wait_queue_head_t	wait_chase;		/* for handling sleeping while waiting for chase to finish */
	wait_queue_head_t	wait_open;		/* for handling sleeping while waiting for open to finish */
	wait_queue_head_t	wait_command;		/* for handling sleeping while waiting for command to finish */
	wait_queue_head_t	delta_msr_wait;		/* for handling sleeping while waiting for msr change to happen */
	struct async_icount	icount;
	struct usb_serial_port	*port;			/* loop back to the owner of this object */

  	//for processing control URBS in interrupt context
        struct urb      *control_urb;
        char            *ctrl_buf;
        int             MsrLsr;

	struct urb       	*write_urb_pool[NUM_URBS];
};


/* This structure holds all of the individual serial device information */

struct moschip_serial 
{
	char	name[MAX_NAME_LEN+1];			/* string name of this device */
	struct moschip_product_info	product_info;	/* Product Info */
	__u8	interrupt_in_endpoint;			/* the interrupt endpoint handle */
	unsigned char * interrupt_in_buffer;		/* the buffer we use for the interrupt endpoint */
	struct urb *	interrupt_read_urb;		/* our interrupt urb */
	__u8		bulk_in_endpoint;		/* the bulk in endpoint handle */
	unsigned char *	bulk_in_buffer;			/* the buffer we use for the bulk in endpoint */
	struct urb *	read_urb;			/* our bulk read urb */
	__u8		bulk_out_endpoint;		/* the bulk out endpoint handle */
	__s16		rxBytesAvail;			/* the number of bytes that we need to read from this device */
	__u8		rxPort;				/* the port that we are currently receiving data for */
	__u8		rxStatusCode;			/* the receive status code */
	__u8		rxStatusParam;			/* the receive status paramater */
	__s16		rxBytesRemaining;		/* the number of port bytes left to read */
	struct usb_serial	*serial;		/* loop back to the owner of this object */
	struct parport		*parp;
};

/* local function prototypes */


/* function prototypes for all URB callbacks */

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
static void mos7705_bulk_in_callback (struct urb *urb);
#else
static void mos7705_bulk_in_callback (struct urb *urb, struct pt_regs *regs);
#endif 

/* function prototypes for the usbserial callbacks */

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static int mos7705_open (struct usb_serial_port *port, struct file * filp);
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2,6,32)
static int mos7705_open (struct tty_struct *tty,struct usb_serial_port *port, struct file * filp);
#else
static int mos7705_open (struct tty_struct *tty,struct usb_serial_port *port);
#endif

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27))
static void mos7705_close (struct usb_serial_port *port, struct file * filp);
#elif (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,30))
static void mos7705_close(struct tty_struct *tty,struct usb_serial_port *port, struct file *filp);
#else
static void mos7705_close(struct usb_serial_port *port);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static void mos7705_throttle (struct usb_serial_port *port);
#else
static void mos7705_throttle(struct tty_struct *tty);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static void mos7705_unthrottle (struct usb_serial_port *port);
#else
static void mos7705_unthrottle(struct tty_struct *tty);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static int mos7705_ioctl (struct usb_serial_port *port, struct file *file, unsigned int cmd, unsigned long arg);
#elif LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39)
static int mos7705_ioctl(struct tty_struct *tty, struct file *file,unsigned int cmd, unsigned long arg);
#else
static int mos7705_ioctl(struct tty_struct *tty, unsigned int cmd, unsigned long arg);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27) && LINUX_VERSION_CODE > KERNEL_VERSION(2,6,10)
static int mos7705_write (struct usb_serial_port *port, const unsigned char *data, int count);
#elif LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,10)
static int mos7705_write (struct usb_serial_port *port, int from_user, const unsigned char *data, int count);
#else
static int mos7705_write (struct tty_struct *tty,struct usb_serial_port *port, const unsigned char *data, int count);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static int mos7705_write_room (struct usb_serial_port *port);
#else
static int  mos7705_write_room(struct tty_struct *tty);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static int mos7705_chars_in_buffer (struct usb_serial_port *port);
#else
static int  mos7705_chars_in_buffer(struct tty_struct *tty);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27) && LINUX_VERSION_CODE > KERNEL_VERSION(2,6,19)
static void mos7705_set_termios (struct usb_serial_port *port, struct ktermios *old_termios);
#elif LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,19)
static void mos7705_set_termios (struct usb_serial_port *port, struct termios *old_termios);
#else
static void mos7705_set_termios(struct tty_struct *tty,struct usb_serial_port *port,struct ktermios *old_termios);
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
static void mos7705_break (struct usb_serial_port *port, int break_state);
#else
static void mos7705_break(struct tty_struct *tty, int break_state);
#endif

static int  mos7705_startup(struct usb_serial *serial);
static void mos7705_shutdown(struct usb_serial *serial);


/* function prototypes for all of our local functions */

static int SendMosCmd(void *pp, __u8 request, __u16 value, __u16 index, void *data);

static int mos7705_calc_num_ports(struct usb_serial *serial);

int __init moschip7705_init(void);
void __exit moschip7705_exit(void);

#endif
