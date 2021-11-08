#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/gpio.h>                       // for the GPIO functions 
#include <linux/interrupt.h>                  // for the IRQ code

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Derek Molloy"); 
MODULE_DESCRIPTION("A Button/LED test driver for the RPi"); 
MODULE_VERSION("0.1");

// INIT ENCODER PINS
static unsigned int encoderPlus = 20;
static unsigned int encoderMinus = 21;

// INIT IRQ AND COUNTER
static unsigned int irqNumber;                
static unsigned int encoderCount = 0;           

// SET THE ENCCODER COUNT AS A PARAMETER
module_param(encoderCount, int, S_IRUGO);
MODULE_PARM_DESC(encoderCount, "The amount of presses");

// prototype for the custom IRQ handler function, function below 
static irq_handler_t  erpi_gpio_irq_handler(unsigned int irq, 
                                            void *dev_id, struct pt_regs *regs);


static int __init erpi_gpio_init(void) 
{
    int result = 0;
    printk(KERN_INFO "GPIO_TEST: Initializing the GPIO_TEST LKM\n");

    // SETUP THE ENCODER PINS AS INPUTS
    gpio_request(encoderPlus, "sysfs");
    gpio_direction_input(encoderPlus);
    gpio_export(encoderPlus, false); 

    gpio_request(encoderMinus, "sysfs");
    gpio_direction_input(encoderMinus);
    gpio_export(encoderMinus, false);  

    printk(KERN_INFO "Encoder value is: %d\n", gpio_get_value(encoderPlus));

    // MAP ENCODER PIN TO IRQ
    irqNumber = gpio_to_irq(encoderPlus);
    printk(KERN_INFO "GPIO_TEST: button mapped to IRQ: %d\n", irqNumber);

    // This next call requests an interrupt line   
    result = request_irq(irqNumber,          // interrupt number requested            
        (irq_handler_t) erpi_gpio_irq_handler,   // handler function            
        IRQF_TRIGGER_RISING,                     // on rising edge (press, not release)            
        "erpi_gpio_handler",                     // used in /proc/interrupts
        NULL);                                   // *dev_id for shared interrupt lines
    printk(KERN_INFO "GPIO_TEST: IRQ request result is: %d\n", result);
    return result;
}

static void __exit erpi_gpio_exit(void) 
{   
    printk(KERN_INFO "Exiting kernel module \n");

    // NULL THE VALUES
    gpio_set_value(encoderPlus, 0);
    gpio_set_value(encoderMinus, 0);
    // UNEXPORT THE PINS
    gpio_unexport(encoderPlus);
    gpio_unexport(encoderMinus);
    // FREE THE UNITS
    free_irq(irqNumber, NULL);
    gpio_free(encoderPlus);
    gpio_free(encoderMinus);
}

static irq_handler_t erpi_gpio_irq_handler(unsigned int irq, 
                                           void *dev_id, struct pt_regs *regs) 
{
    if (gpio_get_value(encoderMinus) == 1){
        encoderCount--;
    }
    else{
        encoderCount++;
    }
    printk(KERN_INFO "Encoder count is: %d\n", encoderCount);
    return (irq_handler_t) IRQ_HANDLED;
}

module_init(erpi_gpio_init);
module_exit(erpi_gpio_exit);