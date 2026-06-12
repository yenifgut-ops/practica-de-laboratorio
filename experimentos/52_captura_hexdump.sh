#!/bin/bash
# Proyecto Integrador - Pareja 5
# Script para volcar eventos binarios en formato hexadecimal

DEVICE="/dev/input/event2"

if [ "$EUID" -ne 0 ]; then
  echo "Por favor, ejecuta este script como root (sudo)."
  exit 1
fi

echo "=== Capturando ráfaga de bytes en crudo desde $DEVICE ==="
echo "Presiona un par de teclas y luego cierra con Ctrl+C..."
cat "$DEVICE" | hexdump -C
