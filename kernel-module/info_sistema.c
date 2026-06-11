#include <linux/module.h>  // Requerido por todos los módulos del kernel 
#include <linux/kernel.h>  // Requerido por las macros de logs como KERN_INFO
#include <linux/init.h>    // Requerido por las macros __init y __exit
#include <linux/utsname.h> // Requerido para obtener datos del sistema (uname) 

// Definición de metadatos del módulo (Evita alertas de kernel "tainted") 
MODULE_AUTHOR("Yenifer Gutierrez y Jose Zerpa");
MODULE_DESCRIPTION("Modulo de informacion del subsistema HID y dispositivos de entrada");
MODULE_LICENSE("GPL");

/**
 * init_module - Función de inicialización del módulo del kernel.
 * Se ejecuta automáticamente de forma privilegiada al invocar 'insmod'.
 * Retorna 0 si el módulo se cargó correctamente o un código de error negativo.
 */
int init_module(void)
{
	printk(KERN_INFO "=== MODULO HID CARGADO ===\n");
	printk(KERN_INFO "Subsistema: HID (Human Interface Device)\n");
	printk(KERN_INFO "Dispositivos HID comunes: teclado, mouse, touchpad, joystick\n");
	printk(KERN_INFO "Kernel actual: %s\n", utsname()->release);
	return 0;
}

/**
 * cleanup_module - Función de descarga y limpieza del módulo.
 * Se ejecuta automáticamente al invocar 'rmmod' para liberar la memoria
 * ocupada por el objeto en el espacio del núcleo.
 */
void cleanup_module(void)
{
	printk(KERN_INFO "Modulo HID descargado por Yenifer Gutierrez y Jose Zerpa\n");
}


