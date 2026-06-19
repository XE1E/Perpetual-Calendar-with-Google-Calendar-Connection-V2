# Calendario Perpetuo con Conexión a Google Calendar

[![English](https://img.shields.io/badge/lang-English-blue.svg)](README.en.md)
[![Español](https://img.shields.io/badge/lang-Español-green.svg)](README.md)

![Calendario Perpetuo](docs/perpetual_calendar.jpg)

Un calendario perpetuo físico con LEDs, basado en ESP8266, que se sincroniza con Google Calendar para mostrar festivos, aniversarios y tareas usando colores.

## Instalación Rápida (Web Flasher)

**No requiere instalar software!** Flashea el firmware directamente desde tu navegador:

1. Conecta tu ESP8266 (D1 Mini) por USB
2. Visita el [Web Flasher](https://XE1E.github.io/Perpetual-Calendar-with-Google-Calendar-Connection-V2/)
3. Haz clic en "Install" y selecciona tu dispositivo
4. Listo!

## Características

- 75 LEDs direccionables WS2811
- Integración con Google Calendar (festivos, aniversarios, tareas)
- Brillo automático según hora del día
- Colores personalizables via interfaz web
- Configuración WiFi via interfaz web
- Panel de información del sistema (RAM, CPU, Uptime)
- Actualizaciones OTA (Over-The-Air)
- Reloj con colores opcional

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

- [Guía en Instructables](https://www.instructables.com/Perpetual-Calendar-With-Google-Calendar-Connection/)
- [Configuración de Google Calendar](GOOGLE_CALENDAR_SETUP.md)
- [Manual Completo](MANUAL.md)

## Licencia

Este proyecto está licenciado bajo [Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0)](https://creativecommons.org/licenses/by-nc-sa/4.0/).

[![CC BY-NC-SA 4.0](https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png)](https://creativecommons.org/licenses/by-nc-sa/4.0/)

Basado en el proyecto original de [Instructables](https://www.instructables.com/Perpetual-Calendar-With-Google-Calendar-Connection/).
