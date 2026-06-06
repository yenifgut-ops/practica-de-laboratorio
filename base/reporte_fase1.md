# Reporte de Laboratorio: Fase 1 (Temas 5.1 y 6.1)

**Integrantes:** [Yenifer Gutierrez] y [Jose Zerpa]  
**Fecha:** 5 de Junio, 2026

**Identificación de la VM:** vm-gutierrez-zerpa  

---

## A. Preparación de la VM

La máquina virtual ha sido configurada y verificada con los parámetros técnicos solicitados para el correcto desarrollo de las fases posteriores del laboratorio.

### 1. Parámetros de Sistema (Memoria y Sistema Operativo)
- **Sistema Operativo:** Ubuntu 24.04.4 LTS Desktop (64-bit)
- **Memoria RAM:** 3072 MB (3 GB asignados, superando el mínimo requerido de 2 GB para mitigar problemas de *thrashing*).

![Configuración de Memoria Base](../capturas/configuracion-vm1.jpg)

---

### 2. Parámetros de Procesador (CPU)
- **Procesador:** 2 núcleos asignados en la pestaña *Processor*.
- **Capacidad de Ejecución:** 100% con soporte de virtualización por hardware habilitado por defecto en la pestaña de aceleración.

![Configuración de Procesores](../capturas/configuracion-vm2.jpg)

---

### 3. Configuración de Red

Para garantizar tanto el acceso a internet de la máquina virtual como la administración remota segura desde el sistema operativo host, se estructuraron dos adaptadores independientes:

- **Adaptador 1 (Interfaces de salida):** Configurado en modo **NAT** para permitir la descarga de paquetes, dependencias y actualizaciones del sistema operativo.
- **Adaptador 2 (Interfaz de administración):** Configurado en modo **Adaptador sólo anfitrión (Host-Only)** utilizando el dispositivo virtual *VirtualBox Host-Only Ethernet Adapter*. Este entorno aislado proporcionará el direccionamiento IP estático o dinámico privado necesario para interactuar mediante SSH desde la terminal de nuestro anfitrión.

#### Evidencia del Adaptador 1 (NAT):
![Configuración Adaptador 1 NAT](../capturas/configuracion-vm3.jpg)

#### Evidencia del Adaptador 2 (Host-Only):
![Configuración Adaptador 2 Host-Only](../capturas/configuracion-vm4.jpg)

## Tema 6.1: Clasificación del Hypervisor

En esta sección se analiza y clasifica el hipervisor utilizado para el desarrollo del laboratorio, aportando evidencias tanto desde el sistema operativo invitado (VM) como desde el sistema operativo anfitrión (Windows 10 Pro 22H2).

### 1. Evidencia desde el interior de la VM (Sistema Invitado)

Al ejecutar comandos de auditoría de hardware en la terminal de la máquina virtual (Ubuntu 24.04 LTS), se obtuvieron los siguientes resultados:

* **Comando:** `systemd-detect-virt`
  * **Resultado obtenido:** `oracle`
  * **Análisis:** El sistema operativo invitado detecta explícitamente que no se está ejecutando sobre hardware real, sino que está aislado dentro del entorno de virtualización de **Oracle VM VirtualBox**.

* **Comando:** `lscpu | grep -i hypervisor`
  * **Resultado obtenido:** * Presencia de la flag `hypervisor` en las características de la CPU.
    * `Hypervisor vendor: KVM`
  * **Análisis:** Confirma que el procesador virtualizado tiene activas las extensiones de aceleración por hardware (Intel VT-x / AMD-V) heredadas del procesador físico del anfitrión, permitiendo una emulación eficiente.

![Salida de systemd y lscpu](../capturas/systemd-y-lscpu.jpg)
---

### 2. Evidencia desde el ANFITRIÓN (Windows 10 Pro 22H2)

Para evaluar el comportamiento de los módulos y controladores del hipervisor, se ejecutaron pruebas de control dentro y fuera del entorno:

* **Comando:** `lsmod | grep vbox`
  * **Resultado obtenido:** `vboxguest  57344  0`
  * **Análisis:** El módulo `vboxguest` está correctamente cargado dentro de la VM. Este componente representa las *Guest Additions* de VirtualBox, encargadas de la integración de periféricos, carpetas compartidas y optimización de pantalla entre el anfitrión y el invitado.

* **Comando:** `modinfo vboxdrv`
  * **Resultado obtenido:** `ERROR: Module vboxdrv not found.`
  * **Análisis y función del módulo:** Este error es **correcto y esperado** dentro de la VM. El módulo `vboxdrv` (VirtualBox Support Driver) es el controlador principal del hipervisor que **debe ejecutarse exclusivamente en el sistema operativo anfitrión (Windows 10)**. 

![Ejecución de lsmod y error de modinfo](../capturas/k.jpg)
  
  **¿Qué hace el módulo `vboxdrv`?** Es el encargado de interactuar directamente con el núcleo (kernel) del sistema anfitrión para reservar memoria física, conmutar el contexto de la CPU hacia el modo de ejecución virtual y coordinar el acceso al hardware real. Al estar dentro de la máquina virtual, el sistema no tiene acceso a este módulo del host, lo que demuestra el aislamiento del entorno.

---

### 3. Conclusión Justificada: Clasificación de VirtualBox

**Conclusión:** Oracle VM VirtualBox es un **Hipervisor de Tipo 2 (o Alojado / Hosted)**.

**Justificación con base en las evidencias:**
1. **Dependencia de un Sistema Operativo Base:** VirtualBox no se instala directamente sobre el hardware desnudo (*bare-metal*). Como se demostró en el entorno de trabajo, requiere que el sistema operativo **Windows 10 Pro** esté completamente cargado y en ejecución para poder iniciar.
2. **Arquitectura de Módulos:** La ausencia del módulo de control `vboxdrv` dentro de la VM y su ejecución obligatoria en el espacio de kernel del Host confirman que el hipervisor funciona como una aplicación avanzada dentro de Windows, abstrayendo los recursos a través de las APIs del sistema operativo anfitrión en lugar de controlar el hardware de manera nativa.
