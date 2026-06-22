# Changelog

Todos los cambios notables de este proyecto se documentan en este archivo.

El formato sigue las convenciones de [Keep a Changelog](https://keepachangelog.com/es-ES/1.0.0/).
La numeración corresponde a la **versión del firmware**.

> ℹ️ Las versiones no tienen fecha asociada en el historial original. La etiqueta de repositorio `v1.0.0` corresponde a la primera publicación de este repositorio "V2".

---

## [2.2]

### Añadido
- Corrección gamma y temperatura de color en la página de colores.
- Color personalizable para el indicador de mes.
- Minutos en la configuración de auto-brillo (formato hora:minuto).
- Soporte para Web Flasher con selección de versión.

### Cambiado
- Separación del firmware en dos versiones: **Standard** y **Color Coded Clock**.
- Mejorada la documentación y la guía de construcción con imágenes.

---

## [2.1]

### Añadido
- Página de información del sistema con recursos (RAM, Flash, CPU, Uptime).
- Botón para reiniciar el dispositivo desde la interfaz web.
- Botón para borrar las credenciales WiFi desde la interfaz web.
- Brillo fijo (120) para la animación arcoíris.
- Traducciones para los nuevos elementos de la interfaz.

### Cambiado
- Mejorada la persistencia de los colores personalizados.
- Mejorada la interfaz de configuración de colores (botones más grandes).

---

## [2.0]

### Añadido
- Soporte **mDNS** (acceso vía `http://perpetualcalendar.local`).
- **Actualizaciones OTA** (Over-The-Air).
- Control de brillo vía web.
- Auto-brillo según la hora del día (modo nocturno).
- Personalización de colores vía web.
- Soporte **WiFi multi-red** (red principal + respaldo).
- Reconexión WiFi automática.

### Corregido
- Visualización del reloj con colores (Color Coded Clock).

### Cambiado
- Optimizado el uso de memoria EEPROM (640 bytes).

---

## [1.0]

### Añadido
- Versión inicial.
- Visualización del calendario con LEDs.
- Integración con Google Calendar.
- Configuración vía interfaz web.
- Sincronización NTP.

---

[2.2]: https://github.com/XE1E/Perpetual-Calendar-with-Google-Calendar-Connection-V2
[2.1]: https://github.com/XE1E/Perpetual-Calendar-with-Google-Calendar-Connection-V2
[2.0]: https://github.com/XE1E/Perpetual-Calendar-with-Google-Calendar-Connection-V2
[1.0]: https://github.com/XE1E/Perpetual-Calendar-with-Google-Calendar-Connection-V2
