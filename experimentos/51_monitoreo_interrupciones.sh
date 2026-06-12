#!/bin/bash
# Proyecto Integrador - Pareja 5
# Script para monitorear las interrupciones del teclado e HID en tiempo real

echo "=== Monitoreando interrupciones de Teclado/HID (Presiona Ctrl+C para salir) ==="
watch -n1 "cat /proc/interrupts | grep -iE 'usb|hid|kbd|i8042'"

