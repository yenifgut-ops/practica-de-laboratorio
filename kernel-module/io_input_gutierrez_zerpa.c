#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gutierrez & Zerpa - Pareja 5");
MODULE_DESCRIPTION("Kernel Module para contar dispositivos de entrada HID");
MODULE_VERSION("1.0");

#define BUFFER_SIZE 8192
#define FILE_PATH "/proc/bus/input/devices"

static int __init io_input_init(void) {
    struct file *file;
    char *buffer;
    loff_t pos = 0;
    ssize_t bytes_read;
    int device_count = 0;
    int i;

    printk(KERN_INFO "io_input_gutierrez_zerpa: Iniciando lectura de dispositivos...\n");

    // 1. Asignar buffer en espacio de Kernel de forma segura
    buffer = kzalloc(BUFFER_SIZE, GFP_KERNEL);
    if (!buffer) {
        printk(KERN_ERR "io_input_gutierrez_zerpa: Error crítico al asignar memoria en Kernel.\n");
        return -ENOMEM;
    }

    // 2. Apertura del archivo virtual /proc usando Kernel File I/O
    file = filp_open(FILE_PATH, O_RDONLY, 0);
    if (IS_ERR(file)) {
        printk(KERN_ERR "io_input_gutierrez_zerpa: Falló la apertura de %s\n", FILE_PATH);
        kfree(buffer);
        return PTR_ERR(file);
    }

    // 3. Lectura del contenido hacia el buffer del kernel
    bytes_read = kernel_read(file, buffer, BUFFER_SIZE - 1, &pos);
    if (bytes_read < 0) {
        printk(KERN_ERR "io_input_gutierrez_zerpa: Falló la lectura del archivo descriptor.\n");
        filp_close(file, NULL);
        kfree(buffer);
        return bytes_read;
    }
    buffer[bytes_read] = '\0'; // Asegurar el cierre de la cadena

    // 4. Algoritmo de conteo basándose en el prefijo estandarizado 'N:'
    // Caso base: La primera línea del archivo comienza directamente con 'N:'
    if (bytes_read > 2 && buffer[0] == 'N' && buffer[1] == ':') {
        device_count++;
    }

    // Escaneo exhaustivo del buffer buscando transiciones de nueva línea '\n' seguidas de 'N:'
    for (i = 0; i < bytes_read - 2; i++) {
        if (buffer[i] == '\n' && buffer[i+1] == 'N' && buffer[i+2] == ':') {
            device_count++;
        }
    }

    // 5. Reportar el conteo final al búfer de dmesg
    printk(KERN_INFO "io_input_gutierrez_zerpa: [PROCESAMIENTO EXITOSO] Se detectaron %d dispositivos de entrada registrados.\n", device_count);

    // Liberar recursos asignados
    filp_close(file, NULL);
    kfree(buffer);
    return 0;
}

static void __exit io_input_exit(void) {
    printk(KERN_INFO "io_input_gutierrez_zerpa: Modulo removido del Kernel de forma segura.\n");
}

module_init(io_input_init);
module_exit(io_input_exit);
