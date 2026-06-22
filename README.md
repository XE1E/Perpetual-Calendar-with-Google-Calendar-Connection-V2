# Calendario Perpetuo con Conexión a Google Calendar

[![English](https://img.shields.io/badge/lang-English-blue.svg)](README.en.md)
[![Español](https://img.shields.io/badge/lang-Español-green.svg)](README.md)

![Calendario Perpetuo](docs/perpetual_calendar.jpg)

Un calendario perpetuo físico con LEDs, basado en ESP8266, que se sincroniza con Google Calendar para mostrar festivos, aniversarios y tareas usando colores.

## [Guía Completa de Construcción](https://xe1e.github.io/Perpetual-Calendar-with-Google-Calendar-Connection-V2/GUIA_COMPLETA.html)

Guía paso a paso con imágenes para construir tu propio calendario perpetuo.

## Instalación Rápida (Web Flasher)

**No requiere instalar software!** Flashea el firmware directamente desde tu navegador:

1. Conecta tu ESP8266 (D1 Mini) por USB
2. Visita el [Web Flasher](https://XE1E.github.io/Perpetual-Calendar-with-Google-Calendar-Connection-V2/)
3. Haz clic en "Install" y selecciona tu dispositivo
4. Listo!

## Características

- 75 LEDs direccionables WS2811
- Integración con Google Calendar (festivos, aniversarios, tareas)
- **Colores personalizables** via interfaz web
- **Reloj con colores** que muestra HH:MM:SS (6 LEDs)
- **Brillo automático** con modo nocturno (puede apagar los LEDs por completo)
- **Panel de información del sistema** (RAM, CPU, Uptime, reinicio/borrar WiFi)
- Configuración WiFi via interfaz web
- Actualizaciones OTA (Over-The-Air)
- Soporte mDNS (acceso via `http://perpetualcalendar.local`)

### Reloj con Colores

Cuando se activa, 6 LEDs muestran la hora usando dígitos codificados por color:

| Dígito | Color |
|--------|-------|
| 0 | Gris oscuro |
| 1 | Rojo |
| 2 | Naranja |
| 3 | Amarillo |
| 4 | Verde |
| 5 | Cian |
| 6 | Azul |
| 7 | Púrpura |
| 8 | Magenta |
| 9 | Gris claro |

### Modo Nocturno

Configura el brillo automático para atenuar o apagar los LEDs por la noche:
- Brillo de día: 5-255
- Brillo de noche: 0-255 (0 = apagado por completo)

## Compilar desde Código Fuente

### Usando Arduino IDE
Ver la [guía en Instructables](https://www.instructables.com/Perpetual-Calendar-With-Google-Calendar-Connection/)

### Usando PlatformIO
```bash
# Versión estándar
pio run -e standard

# Con reloj de colores
pio run -e color_coded_clock

# Subir al dispositivo
pio run -e standard -t upload
```

## Documentación

- [Guía Completa de Construcción](https://xe1e.github.io/Perpetual-Calendar-with-Google-Calendar-Connection-V2/GUIA_COMPLETA.html) - Con imágenes paso a paso
- [Lista de Materiales (BOM)](BILL_OF_MATERIALS.md) - Componentes, piezas 3D y herramientas
- [Guía de Ensamblaje y Cableado](ASSEMBLY.md) - Montaje paso a paso
- [Configuración de Google Calendar](GOOGLE_CALENDAR_SETUP.md)
- [Manual Técnico](MANUAL.md)
- [Referencia Rápida](QUICK_REFERENCE.md) - Pinout, colores, endpoints de un vistazo
- [Resolución de Problemas](TROUBLESHOOTING.md)
- [Changelog](CHANGELOG.md) · [Cómo Contribuir](CONTRIBUTING.md)
- [Guía Original en Instructables](https://www.instructables.com/Perpetual-Calendar-With-Google-Calendar-Connection/)

## Licencia

Este proyecto está licenciado bajo [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/).

[![CC BY-NC-SA 4.0](https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png)](https://creativecommons.org/licenses/by-nc-sa/4.0/)

Basado en el proyecto original de [Instructables](https://www.instructables.com/Perpetual-Calendar-With-Google-Calendar-Connection/).
