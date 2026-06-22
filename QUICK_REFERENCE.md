# Referencia Rápida

[![English](https://img.shields.io/badge/lang-English-blue.svg)](QUICK_REFERENCE.en.md)
[![Español](https://img.shields.io/badge/lang-Español-green.svg)](QUICK_REFERENCE.md)

Hoja de referencia de un vistazo. Para el detalle, ver el [Manual Técnico](MANUAL.md).

## Conexiones (Pinout)

| ESP8266 | Tira LED | Notas |
|---------|----------|-------|
| GPIO2 (D4) | DIN | A través de resistencia **3 kΩ** a VCC |
| GND | GND | GND común con la fuente |
| 5V (fuente externa) | VCC | Fuente 5V / **3A** mín. |

## Acceso y valores por defecto

| Parámetro | Valor |
|-----------|-------|
| Red de configuración (AP) | SSID `Calendario` · pass `12345678` |
| Portal de configuración | `http://192.168.4.1` |
| Acceso normal (mDNS) | `http://perpetualcalendar.local` |
| Puerto web | 80 |
| Puerto OTA | 8266 |
| Puerto NTP (cliente) | 2390 |
| Serial | 115200 baud |
| Servidor NTP sugerido | `time.cloudflare.com` |

## Páginas web

| URL | Función |
|-----|---------|
| `/` | Menú principal |
| `/config.html` | WiFi (principal + respaldo) |
| `/ntp.html` | NTP y zona horaria |
| `/appsscript.html` | IDs de Google Apps Script |
| `/time.html` | Hora manual |
| `/info.html` | Info del sistema, reinicio, borrar WiFi |
| `/led.html` | Brillo, Test LEDs, Refresh |
| `/autobrightness.html` | Modo día/noche |
| `/colors.html` | Colores + corrección gamma |
| `/ota.html` | Info OTA |

## Mapeo de LEDs (zigzag)

```
Fila 1: LEDs 68-74  (izq→der)
Fila 2: LEDs 61-49  (der→izq)
Fila 3: LEDs 36-48  (izq→der)
Fila 4: LEDs 35-23  (der→izq)
Fila 5: LEDs 10-22  (izq→der)
Fila 6: LEDs  9-0   (der→izq)
Reloj : LEDs 62-67  (HH:MM:SS, solo Color Coded Clock)
```

## Colores del calendario (por defecto)

| Tipo de día | Color | HSV |
|-------------|-------|-----|
| Laborable | Verde | (96, 255, 192) |
| Día actual | Azul | (160, 255, 192) |
| Fin de semana | Rojo | (0, 255, 128) |
| Mes | Gris | (0, 0, 192) |
| Festivos | Púrpura | (192, 255, 255) |
| Aniversarios | Cian | (128, 255, 255) |
| Tareas | Naranja | (32, 255, 192) |

**Prioridad (el último sobrescribe):** base → día actual → festivos → aniversarios → tareas.

## Colores del reloj por dígito

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|---|---|---|---|---|---|---|---|---|---|
| Negro | Rojo | Naranja | Amarillo | Verde | Cian | Azul | Púrpura | Rosa | Blanco |

## Endpoints de API (GET)

| Endpoint | Acción |
|----------|--------|
| `/admin/values` · `/admin/connectionstate` | Config de red · estado WiFi |
| `/admin/infovalues` | Info del sistema |
| `/admin/ntpvalues` · `/admin/appsscript` | NTP · IDs de scripts |
| `/admin/timevalues` | Hora actual |
| `/admin/ledvalues` · `/admin/setbrightness` · `/admin/savebrightness` | Brillo |
| `/admin/autobrightvalues` · `/admin/saveautobrightness` | Auto-brillo |
| `/admin/colorvalues` · `/admin/previewcolors` · `/admin/savecolors` · `/admin/resetcolors` | Colores |
| `/admin/gammavalues` · `/admin/previewgamma` · `/admin/savegamma` | Gamma |
| `/admin/otavalues` | Info OTA |
| `/admin/testleds` · `/admin/refreshcalendar` | Probar LEDs · refrescar |
| `/admin/restart` · `/admin/clearwifi` | Reiniciar · borrar WiFi |

## Direcciones EEPROM clave

| Dirección | Contenido |
|-----------|-----------|
| 0-2 | Firma "CFG" |
| 64-127 | SSID + pass (principal) |
| 128-159 | Servidor NTP |
| 200-487 | IDs de scripts (Holidays / Anniversaries / Todos) |
| 488 | Brillo manual |
| 489-495 | Auto-brillo (día/noche, hora:min) |
| 520-583 | SSID2 + pass2 (respaldo) |
| 584-607 | Colores personalizados (HSV) + gamma |

> EEPROM total: 640 bytes. Mapa completo en el [Manual](MANUAL.md#almacenamiento-en-eeprom).

## Compilar (PlatformIO)

```bash
pio run -e standard            # estándar
pio run -e color_coded_clock   # con reloj
pio run -e standard -t upload  # subir
```

---

*Referencia rápida del proyecto Perpetual Calendar with Google Calendar Connection v2.2*
