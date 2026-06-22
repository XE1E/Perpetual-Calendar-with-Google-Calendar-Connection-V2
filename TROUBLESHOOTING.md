# Resolución de Problemas

[![English](https://img.shields.io/badge/lang-English-blue.svg)](TROUBLESHOOTING.en.md)
[![Español](https://img.shields.io/badge/lang-Español-green.svg)](TROUBLESHOOTING.md)

Referencia única de problemas comunes y sus soluciones, organizada por categoría. Para el detalle técnico de cada subsistema, consulta el [Manual Técnico](MANUAL.md) y la [Guía de Google Calendar](GOOGLE_CALENDAR_SETUP.md).

## Índice

- [Hardware y LEDs](#hardware-y-leds)
- [Arranque y reinicios](#arranque-y-reinicios)
- [WiFi y red](#wifi-y-red)
- [Hora y NTP](#hora-y-ntp)
- [Google Calendar](#google-calendar)
- [Reloj de colores](#reloj-de-colores)
- [mDNS y OTA](#mdns-y-ota)
- [Brillo y colores](#brillo-y-colores)

---

## Hardware y LEDs

### Los LEDs no encienden
1. Verifica la conexión del pin de datos (**GPIO2 / D4**) al **DIN** de la tira.
2. Verifica la alimentación de **5V** y que la fuente entregue corriente suficiente (3A mín).
3. Verifica la **resistencia de 3 kΩ** entre GPIO2 y VCC.
4. Comprueba la dirección de las **flechas** de la tira (datos en el sentido correcto).
5. Usa el botón **"Test LEDs"** en `/led.html`.

### Parte de los LEDs no responde
- Revisa las soldaduras del zigzag y que cada fila vaya en sentido alterno. Ver [Ensamblaje](ASSEMBLY.md#paso-2--montar-los-leds-en-el-soporte).

### Colores o brillo erráticos
- Asegúrate de un **GND común** entre fuente, tira y ESP.
- Acorta el cable de datos hacia DIN.

---

## Arranque y reinicios

### El ESP no inicia o se reinicia al encender
**Causa:** falta la resistencia de 3 kΩ o la fuente es insuficiente.
1. Añade la **resistencia de 3 kΩ** entre GPIO2 y VCC.
2. Usa una fuente de **5V / 3A** mínimo.
3. Conecta todos los **GND** juntos.

### Reinicio constante cada ~30 segundos
**Causa:** el watchdog detecta el loop bloqueado.
1. La conexión a Google puede estar tardando demasiado — verifica Internet.
2. Verifica la estabilidad del WiFi.
3. Revisa los logs por Serial (115200 baud).

---

## WiFi y red

### No hay conexión WiFi
1. Verifica SSID y contraseña en `/config.html`.
2. Verifica que el router esté encendido y acerca el dispositivo.
3. Configura una **red de respaldo**.
4. Reinicia el ESP8266.

### El WiFi se desconecta con frecuencia
1. Configura una red de respaldo en `/config.html`.
2. Acerca el dispositivo al router.
3. Verifica interferencias en el canal WiFi.
4. Revisa los logs por Serial para ver el patrón de desconexiones.

> El dispositivo reintenta cada 30 s y, tras 3 fallos, espera 5 minutos.

---

## Hora y NTP

### Animación arcoíris continua
**Causa:** no se recibe respuesta del servidor NTP.
1. Verifica la conexión a Internet.
2. Prueba otro servidor (`time.cloudflare.com`, `time.google.com`).
3. Verifica que el puerto **UDP 123** no esté bloqueado.

### Hora incorrecta
1. Verifica la **zona horaria** en `/ntp.html`.
2. Activa/desactiva el **horario de verano** según tu región.
3. Usa la configuración manual de hora temporalmente (`/time.html`).

---

## Google Calendar

### No se muestran eventos
1. Verifica los **IDs de implementación** de los scripts.
2. Asegúrate de que los scripts estén publicados como **"Cualquier usuario"**.
3. El evento debe tener fecha dentro del **mes actual**.
4. Prueba el script en el navegador: `https://script.google.com/macros/s/TU_ID/exec`.

### "Error: Calendario no encontrado"
- El ID del calendario es incorrecto. Cópialo de nuevo desde *Configuración del calendario → Integrar el calendario* y vuelve a publicar el script.

### Error 401 / "No autorizado"
- Elimina la implementación actual y crea una nueva con permiso **"Cualquier usuario"**. Copia el nuevo ID al dispositivo.

### El script funciona en el navegador pero no en el ESP8266
- Pega **solo el ID de implementación** (empieza con `AKfycb`), **no** la URL completa ni `/exec`.

Más detalle en la [Guía de Google Calendar](GOOGLE_CALENDAR_SETUP.md#resolucion-de-problemas).

---

## Reloj de colores

### El reloj no se actualiza o parpadea mal
1. Verifica que instalaste la versión **"Color Coded Clock"** (no la Standard).
2. Verifica que NTP funcione (hora correcta).
3. Los LEDs del reloj (**62-67**) no deben estar dañados.
4. Reinicia el dispositivo.

---

## mDNS y OTA

### No funciona `http://perpetualcalendar.local`
1. Verifica que el dispositivo esté conectado a WiFi.
2. En **Windows:** instala Bonjour (viene con iTunes).
3. En **Android:** usa la IP directa o una app de Bonjour Browser.
4. Espera 30 s tras el reinicio y prueba otro navegador.

### OTA no aparece en Arduino IDE
1. El IDE y el ESP8266 deben estar en la **misma red**.
2. Desactiva el firewall temporalmente.
3. Reinicia Arduino IDE.
4. Verifica que mDNS funcione (prueba el acceso web).

> La primera carga debe ser por USB; las siguientes pueden ser OTA (puerto 8266).

---

## Brillo y colores

### El brillo no se guarda
1. Usa el botón **"Save"**, no solo el slider.
2. Reinicia el dispositivo para verificar.

### Los LEDs se ven apagados de noche
- Es el **modo nocturno**: revisa `/autobrightness.html`. Brillo noche en `0` apaga los LEDs por completo.

---

*Guía de resolución de problemas del proyecto Perpetual Calendar with Google Calendar Connection v2.2*
