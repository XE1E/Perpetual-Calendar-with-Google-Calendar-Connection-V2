# Manual Completo: Calendario Perpetuo con Conexión a Google Calendar

## Tabla de Contenidos

1. [Descripción General](#descripción-general)
2. [Requisitos de Hardware](#requisitos-de-hardware)
3. [Arquitectura del Sistema](#arquitectura-del-sistema)
4. [Estructura de Archivos](#estructura-de-archivos)
5. [Funcionamiento Detallado](#funcionamiento-detallado)
6. [Mapeo de LEDs](#mapeo-de-leds)
7. [Sistema de Colores](#sistema-de-colores)
8. [Reloj con Colores (Color Coded Clock)](#reloj-con-colores-color-coded-clock)
9. [Sincronización NTP](#sincronización-ntp)
10. [Integración con Google Calendar](#integración-con-google-calendar) - Ver tambien: [Guia Completa de Google Calendar](GOOGLE_CALENDAR_SETUP.md)
11. [Interfaz Web de Configuración](#interfaz-web-de-configuración)
12. [Funciones Avanzadas V2](#funciones-avanzadas-v2)
    - [mDNS](#mdns)
    - [Actualizaciones OTA](#actualizaciones-ota)
    - [Control de Brillo](#control-de-brillo)
    - [Auto-Brillo por Hora](#auto-brillo-por-hora)
    - [Colores Personalizables](#colores-personalizables)
    - [WiFi Multi-Red](#wifi-multi-red)
13. [Almacenamiento en EEPROM](#almacenamiento-en-eeprom)
14. [Instalación y Configuración Inicial](#instalación-y-configuración-inicial)
15. [Resolución de Problemas](#resolución-de-problemas)

---

## Descripción General

Este proyecto implementa un **calendario perpetuo físico** utilizando un microcontrolador ESP8266 y una tira de 75 LEDs WS2811. 
El calendario:

- Muestra visualmente todos los días del mes actual
- Se sincroniza automáticamente con la hora vía NTP
- Se conecta a Google Calendar para mostrar eventos
- Diferencia visualmente fines de semana, días festivos, aniversarios y tareas
- Se configura mediante una interfaz web integrada
- **Modo reloj opcional** con colores para mostrar la hora (HH:MM:SS)
- **Acceso fácil** mediante mDNS (http://perpetualcalendar.local)
- **Actualizaciones OTA** sin necesidad de cable USB
- **Brillo ajustable** manualmente o automático según hora del día
- **Colores personalizables** vía interfaz web
- **Soporte multi-red WiFi** con red de respaldo

### Diagrama de Arquitectura

```
┌─────────────────────────────────────────────────────────────────┐
│                        CALENDARIO PERPETUO                       │
├─────────────────────────────────────────────────────────────────┤
│                                                                  │
│   ┌─────────────┐      ┌──────────────┐      ┌──────────────┐   │
│   │   ESP8266   │◀────▶│  Servidor    │      │   Google     │   │
│   │             │      │     NTP      │      │   Calendar   │   │
│   └──────┬──────┘      └──────────────┘      └──────┬───────┘   │
│          │                                          │           │
│          │              ┌──────────────┐            │           │
│          │              │ Google Apps  │◀───────────┘           │
│          │              │   Script     │                        │
│          │              └──────┬───────┘                        │
│          │                     │                                │
│          ▼                     ▼                                │
│   ┌─────────────┐      ┌──────────────┐                        │
│   │  75 LEDs    │      │  Interfaz    │                        │
│   │   WS2811    │      │     Web      │                        │
│   └─────────────┘      └──────────────┘                        │
│                                                                  │
└─────────────────────────────────────────────────────────────────┘
```

---

## Requisitos de Hardware

| Componente | Especificación |
|------------|----------------|
| Microcontrolador | ESP8266 (NodeMCU, Wemos D1 Mini, etc.) |
| Tira LED | WS2811/WS2812B, 75 LEDs |
| Pin de datos LED | GPIO 2 (D4 en NodeMCU) |
| Alimentación | 5V, mínimo 3A (75 LEDs × 60mA máx) |
| Conexión | WiFi 2.4GHz |

### Esquema de Conexión

```
ESP8266          Tira LED WS2811
────────         ───────────────
GPIO 2  ───────▶ DIN (Data In)
GND     ───────▶ GND
5V      ───────▶ VCC (desde fuente externa)
```

> **Nota:** Se recomienda alimentar la tira LED desde una fuente externa de 5V y conectar el GND común con el ESP8266.

---

## Estructura de Archivos

```
Perpetual-Calendar-with-Google-Calendar-Connection/
│
├── Perpetual-Calendar-With-Google-Calendar-Connection.ino  # Archivo principal
├── global.h                    # Variables globales y configuración EEPROM
├── NTP.h                       # Sincronización de tiempo NTP
├── HTTPSRedirect.h/.cpp        # Cliente HTTPS para Google Apps Script
├── ColorCodedClock.h           # Modo reloj con colores (opcional)
├── DebugMacros.h               # Macros de depuración
│
├── WiFiManager.h               # Gestión WiFi multi-red y reconexión automática
├── AutoBrightness.h            # Control de brillo automático por hora
│
├── Page_Admin.h                # Página principal de administración
├── Page_NetworkConfiguration.h # Configuración de red WiFi (principal + respaldo)
├── Page_NTPSettings.h          # Configuración del servidor NTP
├── Page_AppsScriptSettings.h   # Configuración de Google Apps Script
├── Page_SetTime.h              # Configuración manual de hora
├── Page_Information.h          # Información del dispositivo
├── Page_LEDSettings.h          # Control de brillo de LEDs
├── Page_AutoBrightness.h       # Configuración de auto-brillo
├── Page_ColorSettings.h        # Personalización de colores
├── Page_OTA.h                  # Información de actualizaciones OTA
├── Page_Style.css.h            # Estilos CSS de la interfaz web
├── Page_Script.js.h            # JavaScript de la interfaz web
│
├── README.md                   # Documentación básica
└── MANUAL.md                   # Este manual
```

### Descripción de Cada Archivo

| Archivo | Propósito |
|---------|-----------|
| `*.ino` | Contiene `setup()`, `loop()`, funciones de display LED y conexión a Google |
| `global.h` | Define la estructura de configuración, funciones EEPROM, servidor web |
| `NTP.h` | Obtiene hora UTC, calcula zona horaria, horario de verano, día de semana |
| `HTTPSRedirect.*` | Maneja conexiones HTTPS con redirección para Google Apps Script |
| `WiFiManager.h` | Gestión de múltiples redes WiFi con ESP8266WiFiMulti |
| `AutoBrightness.h` | Lógica de brillo automático día/noche |
| `ColorCodedClock.h` | Display de hora usando colores en LEDs |
| `Page_*.h` | Páginas HTML embebidas para la interfaz de configuración web |

---

## Funcionamiento Detallado

### Flujo de Inicialización (setup)

```
1. Iniciar Serial (115200 baud)
2. Leer configuración desde EEPROM
3. Intentar conexión WiFi
   ├── Éxito: Modo Station (cliente WiFi)
   └── Fallo: Modo AP (punto de acceso "Calendario")
4. Iniciar servidor web en puerto 80
5. Configurar FastLED (75 LEDs, WS2811, pin GPIO 2)
6. Conectar a Google Apps Script
7. Iniciar timer de 1 segundo (ISRsecondTick)
```

### Bucle Principal (loop)

```
loop() {
    1. Manejar peticiones web (server.handleClient)
    
    2. Actualizar hora NTP si corresponde
       └── Primera vez: después de 5 segundos
       └── Luego: cada X minutos (configurable)
    
    3. Verificar estado de conexión:
       ├── Sin WiFi → Mostrar LED de advertencia (rojo)
       ├── Sin NTP  → Mostrar animación "pride" (arcoíris)
       └── Con hora → Mostrar calendario
    
    4. Si hay hora válida:
       ├── Cada MINUTO: Actualizar calendario "Todos"
       └── Cada HORA: Actualizar "Holidays" y "Anniversaries"
           └── Redibujar display completo
}
```

### Algoritmo de Cálculo del Día de la Semana

El proyecto utiliza el **algoritmo de Zeller modificado** para calcular el día de la semana:

```cpp
// Archivo: NTP.h, líneas 201-208
int DayOfTheWeek(int year, int month, int day) {
    if (month <= 2) {
        --year;
        month += 12;
    }
    return ((((day + (((month + 1) * 26) / 10) + year + (year / 4)
            + (6 * (year / 100)) + (year / 400)) % 7) + 6) % 7);
}
// Resultado: 0 = Domingo, 1 = Lunes, ..., 6 = Sábado
```

### Algoritmo de Año Bisiesto

```cpp
// Archivo: NTP.h, líneas 167-172
bool isLeapYear(int yr) {
    if (yr % 4 == 0 && yr % 100 != 0 || yr % 400 == 0)
        return true;
    else
        return false;
}
```

Un año es bisiesto si:
- Es divisible por 4 **Y** no es divisible por 100
- **O** es divisible por 400

---

## Mapeo de LEDs

El calendario utiliza 75 LEDs distribuidos en un patrón de zigzag:

### Distribución Física

```
LED 68-74:  [Dom][Lun][Mar][Mié][Jue][Vie][Sáb]  ← Fila 1 (días 1-7)
LED 61-49:  [   ][   ][   ][   ][   ][   ][   ]  ← Fila 2 (zigzag inverso)
LED 36-48:  [   ][   ][   ][   ][   ][   ][   ]  ← Fila 3
LED 35-23:  [   ][   ][   ][   ][   ][   ][   ]  ← Fila 4 (zigzag inverso)
LED 10-22:  [   ][   ][   ][   ][   ][   ][   ]  ← Fila 5
LED 9-0:    [   ][   ][   ][   ][   ][   ][   ]  ← Fila 6 (zigzag inverso)

LED 62: Indicador de advertencia (sin WiFi)
```

### Array de Mapeo

```cpp
// Archivo: *.ino, líneas 33-38
byte calendar_leds[] = {
    68, 69, 70, 71, 72, 73, 74,                    // Fila 1: izq → der
    61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49,  // Fila 2: der → izq
    36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,  // Fila 3: izq → der
    35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23,  // Fila 4: der → izq
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,  // Fila 5: izq → der
    9, 8, 7, 6, 5, 4, 3, 2, 1, 0                   // Fila 6: der → izq
};
```

### LEDs de Meses

```cpp
// Archivo: *.ino, línea 39
byte calendar_months[] = {19, 20, 21, 22, 7, 6, 5, 4, 3, 2, 1, 0};
// Índice:                 Ene Feb Mar Abr May Jun Jul Ago Sep Oct Nov Dic
```

---

## Sistema de Colores

### Paleta de Colores (HSV)

| Color | HSV | Uso |
|-------|-----|-----|
| Gris oscuro | (0, 0, 64) | Índice 0 |
| Rojo | (0, 255, 192) | Fines de semana |
| Naranja | (32, 255, 255) | Tareas (Todos) |
| Amarillo | (64, 255, 255) | Índice 3 |
| Verde | (96, 255, 255) | Días laborables |
| Cian | (138, 255, 255) | Aniversarios |
| Azul | (160, 255, 255) | Día actual |
| Púrpura | (180, 255, 255) | Festivos |
| Magenta | (214, 255, 255) | Índice 8 |
| Gris claro | (0, 0, 192) | Mes actual |

### Asignación de Colores

```cpp
// Archivo: *.ino, líneas 46-59
CHSV weekday_color     = rainbow_colors[4];  // Verde - días laborables
CHSV actualday_color   = rainbow_colors[6];  // Azul - día actual
CHSV weekend_color     = rainbow_colors[1];  // Rojo - fines de semana
CRGB month_color       = rainbow_colors[9];  // Gris - indicador de mes
CHSV holidays_color    = rainbow_colors[7];  // Púrpura - festivos
CHSV anniversaries_color = rainbow_colors[5]; // Cian - aniversarios
CHSV todos_color       = rainbow_colors[2];  // Naranja - tareas
```

### Jerarquía de Prioridad de Colores

Cuando un día tiene múltiples eventos, el color final sigue esta prioridad (el último sobrescribe):

```
1. Color base (laborable/fin de semana)
2. Día actual
3. Festivos (Holidays)
4. Aniversarios (Anniversaries)
5. Tareas (Todos) ← Mayor prioridad visual
```

### Colores de Fin de Semana

El color de fin de semana (`weekend_color`) aplica tanto a **sábados como domingos**. Ambos días usan el mismo color (rojo por defecto) para mantener consistencia visual.

---

## Reloj con Colores (Color Coded Clock)

### Activación

El modo reloj está **desactivado por defecto**. Para activarlo, descomentar la línea 10 del archivo `.ino`:

```cpp
#define COLOR_CODED_CLOCK  // Descomentar para activar
```

### Layout del Reloj (6 LEDs)

```
LED 62   LED 63   LED 64   LED 65   LED 66   LED 67
 ┌──┐    ┌──┐     ┌──┐     ┌──┐     ┌──┐     ┌──┐
 │H1│    │H2│     │M1│     │M2│     │S1│     │S2│
 └──┘    └──┘     └──┘     └──┘     └──┘     └──┘
  ↓       ↓        ↓        ↓        ↓        ↓
Decena  Unidad  Decena   Unidad   Decena   Unidad
 Hora    Hora   Minuto   Minuto   Segundo  Segundo
```

### Codificación de Colores por Dígito

| Dígito | Color | HSV |
|--------|-------|-----|
| 0 | Gris oscuro | (0, 0, 64) |
| 1 | Rojo | (0, 255, 192) |
| 2 | Naranja | (32, 255, 255) |
| 3 | Amarillo | (64, 255, 255) |
| 4 | Verde | (96, 255, 255) |
| 5 | Cian | (138, 255, 255) |
| 6 | Azul | (160, 255, 255) |
| 7 | Púrpura | (180, 255, 255) |
| 8 | Magenta | (214, 255, 255) |
| 9 | Gris claro | (0, 0, 192) |

### Ejemplo: Hora 14:35:27

```
LED 62 = 1 → Rojo      (decena hora)
LED 63 = 4 → Verde     (unidad hora)
LED 64 = 3 → Amarillo  (decena minuto)
LED 65 = 5 → Cian      (unidad minuto)
LED 66 = 2 → Naranja   (decena segundo)
LED 67 = 7 → Púrpura   (unidad segundo)
```

### Separación de LEDs

```
LEDs 62-67: Reloj HH:MM:SS (6 dígitos)
LEDs 68-74: Primera fila del calendario
```

### Indicador de WiFi

Cuando no hay conexión WiFi, los 6 LEDs del reloj **parpadean en rojo** como indicador visual.

> **Nota:** El reloj se actualiza automáticamente cada segundo y se redibuja después de cada actualización del calendario para evitar sobrescrituras.

---

## Sincronización NTP

### Proceso de Sincronización

```
1. Crear cliente UDP en puerto 2390
2. Resolver nombre del servidor NTP a IP
3. Enviar paquete NTP (48 bytes)
4. Esperar respuesta (100ms timeout)
5. Extraer timestamp Unix de bytes 40-43
6. Restar 70 años (diferencia 1900-1970)
7. Almacenar en UnixTimestamp global
```

### Estructura del Paquete NTP

```cpp
// Archivo: NTP.h, líneas 39-47
packetBuffer[0] = 0b11100011;  // LI=3, Version=4, Mode=3 (cliente)
packetBuffer[1] = 0;           // Stratum
packetBuffer[2] = 6;           // Intervalo de polling
packetBuffer[3] = 0xEC;        // Precisión del reloj
// Bytes 12-15: Identificador de referencia
```

### Ajuste de Zona Horaria

```cpp
// Archivo: NTP.h, líneas 157-165
unsigned long adjustTimeZone(unsigned long _timeStamp, int _timeZone,
        bool _isDayLightSavingSaving) {
    _timeStamp += _timeZone * 360;  // Ajustar zona horaria
    if (_isDayLightSavingSaving && summerTime(_timeStamp))
        _timeStamp += 3600;  // Añadir 1 hora si es horario de verano
    return _timeStamp;
}
```

> **Nota:** La zona horaria se almacena en unidades de 6 minutos (360 segundos). Por ejemplo, UTC+1 = timeZone 10.

### Cálculo de Horario de Verano (Europa Central)

El sistema calcula automáticamente el horario de verano para Europa Central:
- **Inicio:** Último domingo de marzo a las 3:00
- **Fin:** Último domingo de octubre a las 3:00

---

## Integración con Google Calendar

### Arquitectura de Conexión

```
ESP8266 ──HTTPS──▶ script.google.com ──redirect──▶ script.googleusercontent.com
                          │
                          ▼
                   Google Apps Script
                          │
                          ▼
                   Google Calendar API
```

### Configuración de Google Apps Script

Se requieren **3 scripts** separados, uno para cada tipo de evento:

1. **HolidaysScriptID** - Calendario de festivos
2. **AnniversariesScriptID** - Calendario de aniversarios  
3. **TodosScriptID** - Calendario de tareas

### Formato de Respuesta del Script

El Google Apps Script debe devolver los días del mes con eventos en formato:

```
día1-día2-día3-día4-
```

Ejemplo para eventos los días 5, 12 y 25:
```
5-12-25-
```

### Procesamiento de Datos del Calendario

```cpp
// Archivo: *.ino, líneas 153-164
void initDatesArray(int (&Dates)[20], String calendarString) {
    // Inicializar array a ceros
    for (index = 0; index < 20; index++) {
        Dates[index] = 0;
    }
    // Parsear string separado por guiones
    while(calendarString.indexOf("-") != -1) {
        Dates[index] = calendarString.substring(0, calendarString.indexOf("-")).toInt();
        index++;
        calendarString = calendarString.substring(calendarString.indexOf("-") + 1);
    }
}
```

### Frecuencia de Actualización

| Calendario | Frecuencia | Razón |
|------------|------------|-------|
| Todos | Cada minuto | Tareas pueden cambiar frecuentemente |
| Holidays | Cada hora | Festivos son estáticos |
| Anniversaries | Cada hora | Aniversarios son estáticos |

---

## Interfaz Web de Configuración

### Acceso a la Interfaz

- **Modo Station (conectado a WiFi):** `http://[IP_asignada]/`
- **Modo AP (sin WiFi configurado):** `http://192.168.4.1/`
  - SSID: `Calendario`
  - Password: `12345678`

### Páginas Disponibles

| URL | Descripción |
|-----|-------------|
| `/` | Página principal de administración |
| `/config.html` | Configuración de red WiFi (principal + respaldo) |
| `/ntp.html` | Configuración del servidor NTP |
| `/appsscript.html` | IDs de Google Apps Script |
| `/time.html` | Configuración manual de hora |
| `/info.html` | Información del sistema (red, recursos, acciones) |
| `/led.html` | Control de brillo de LEDs |
| `/autobrightness.html` | Configuración de auto-brillo |
| `/colors.html` | Personalización de colores |
| `/ota.html` | Información de actualizaciones OTA |

### Endpoints de API

| Endpoint | Método | Descripción |
|----------|--------|-------------|
| `/admin/values` | GET | Valores de configuración de red |
| `/admin/connectionstate` | GET | Estado de conexión WiFi |
| `/admin/infovalues` | GET | Información del sistema |
| `/admin/ntpvalues` | GET | Configuración NTP |
| `/admin/appsscript` | GET | IDs de scripts |
| `/admin/timevalues` | GET | Hora actual |
| `/admin/ledvalues` | GET | Valor actual de brillo |
| `/admin/setbrightness` | GET | Ajustar brillo (temporal) |
| `/admin/savebrightness` | GET | Guardar brillo en EEPROM |
| `/admin/autobrightvalues` | GET | Configuración auto-brillo |
| `/admin/saveautobrightness` | GET | Guardar auto-brillo |
| `/admin/colorvalues` | GET | Colores actuales |
| `/admin/previewcolors` | GET | Preview de colores |
| `/admin/savecolors` | GET | Guardar colores |
| `/admin/resetcolors` | GET | Restaurar colores por defecto |
| `/admin/otavalues` | GET | Información OTA |
| `/admin/testleds` | GET | Probar todos los LEDs |
| `/admin/refreshcalendar` | GET | Forzar actualización calendario |
| `/admin/restart` | GET | Reiniciar el dispositivo |
| `/admin/clearwifi` | GET | Borrar credenciales WiFi y reiniciar |

---

## Funciones Avanzadas V2

### Página de Información del Sistema

**Página:** `/info.html`

Muestra información completa del dispositivo y permite realizar acciones de mantenimiento.

**Secciones:**

| Sección | Contenido |
|---------|-----------|
| **Red** | SSID conectado, IP, Máscara, Gateway, MAC |
| **Recursos** | RAM libre, Fragmentación, Flash, CPU, Uptime |
| **Acciones** | Botones de reinicio y borrado WiFi |

**Acciones disponibles:**

- **Reiniciar ESP:** Reinicia el dispositivo (con confirmación)
- **Borrar WiFi:** Borra las credenciales WiFi guardadas y reinicia en modo AP (con confirmación)

> **Nota:** Al borrar WiFi, el dispositivo creará nuevamente la red `Calendario` para reconfigurar.

---

### mDNS

El dispositivo es accesible mediante un nombre de dominio local, sin necesidad de conocer la IP.

**Acceso:** `http://perpetualcalendar.local`

```cpp
// El nombre se deriva de config.DeviceName
// "Perpetual Calendar" → "perpetualcalendar.local"
```

**Funcionamiento:**
- Se activa automáticamente al conectar a WiFi
- Se reinicia automáticamente tras reconexión WiFi
- Compatible con Windows, macOS, Linux e iOS
- Android requiere app adicional (Bonjour Browser)

---

### Actualizaciones OTA

Permite actualizar el firmware sin conexión USB, directamente desde Arduino IDE.

**Puerto:** 8266

**Indicadores LED durante actualización:**
| Estado | Color LEDs |
|--------|------------|
| Iniciando | Azul |
| Progreso | Púrpura (barra de progreso) |
| Completado | Verde |
| Error | Rojo |

**Pasos para actualizar:**
1. Abrir Arduino IDE
2. Ir a `Herramientas > Puerto`
3. Seleccionar el puerto de red: `perpetualcalendar at [IP]`
4. Subir el sketch normalmente

> **Nota:** La primera carga debe hacerse por USB. Las siguientes pueden ser OTA.

---

### Control de Brillo

**Página:** `/led.html`

**Características:**
- Slider de brillo: 5-255
- Cambio en tiempo real (preview)
- Guardado permanente en EEPROM
- Botón "Test LEDs" para verificar funcionamiento
- Botón "Refresh" para forzar actualización del calendario

**EEPROM:** Dirección 488

---

### Auto-Brillo por Hora (Modo Nocturno)

**Página:** `/autobrightness.html`

Ajusta automáticamente el brillo según la hora del día. Permite configurar un modo nocturno con brillo reducido o LEDs completamente apagados.

**Configuración:**

| Parámetro | Descripción | Rango |
|-----------|-------------|-------|
| Brillo día | Brillo durante horas diurnas | 5-255 |
| Brillo noche | Brillo durante horas nocturnas | 0-255 (0 = OFF) |
| Hora inicio día | Hora a la que comienza modo día | 0-23 |
| Hora inicio noche | Hora a la que comienza modo noche | 0-23 |

**Ejemplo de configuración:**
```
Brillo día: 150
Brillo noche: 15 (o 0 para apagar completamente)
Inicio día: 07:00
Inicio noche: 22:00
```

> **Tip:** Configura brillo noche en **0** para apagar completamente los LEDs durante la noche.

**EEPROM:** Direcciones 489-493

---

### Colores Personalizables

**Página:** `/colors.html`

Permite cambiar los colores de cada tipo de día mediante selectores visuales.

**Colores configurables:**

| Elemento | Color por defecto | Descripción |
|----------|-------------------|-------------|
| Días laborables | Verde | Lunes a viernes |
| Fines de semana | Rojo | Sábados y domingos |
| Día actual | Azul | Día de hoy |
| Festivos | Púrpura | Eventos del calendario Holidays |
| Aniversarios | Cian | Eventos del calendario Anniversaries |
| Tareas | Naranja | Eventos del calendario Todos |

**Funciones:**
- **Preview:** Ver cambios en tiempo real sin guardar
- **Save:** Guardar colores permanentemente
- **Reset to Defaults:** Restaurar colores originales

**EEPROM:** Direcciones 494-511

**Formato de almacenamiento:** HSV (Hue, Saturation, Value) - 3 bytes por color

---

### WiFi Multi-Red

**Página:** `/config.html`

Permite configurar dos redes WiFi. El dispositivo se conecta automáticamente a la mejor señal disponible.

**Configuración:**

```
┌─────────────────────────────────────┐
│ Red Principal (obligatoria)         │
│ SSID:     [___________________]     │
│ Password: [___________________]     │
├─────────────────────────────────────┤
│ Red de Respaldo (opcional)          │
│ SSID 2:     [___________________]   │
│ Password 2: [___________________]   │
└─────────────────────────────────────┘
```

**Comportamiento:**
1. Al iniciar, intenta conectar a cualquiera de las dos redes
2. Selecciona automáticamente la de mejor señal
3. Si pierde conexión, intenta reconectar cada 30 segundos
4. Alterna entre ambas redes buscando conexión
5. Después de 3 intentos fallidos, espera 5 minutos

**Estado de conexión:**
- La página muestra a qué red está conectado actualmente
- Ejemplo: `CONNECTED to MiRedWiFi`

**EEPROM:** Direcciones 520-583 (SSID2 + Password2)

---

### Reconexión WiFi Automática

El sistema monitorea constantemente la conexión WiFi y reconecta automáticamente si se pierde.

**Parámetros:**

| Parámetro | Valor | Descripción |
|-----------|-------|-------------|
| Intervalo de verificación | 30 seg | Tiempo entre verificaciones |
| Máximo reintentos | 3 | Intentos antes de pausa larga |
| Pausa después de fallos | 5 min | Tiempo de espera tras 3 fallos |
| Timeout de conexión | 10 seg | Tiempo máximo por intento |

**Acciones automáticas tras reconexión:**
- Reiniciar servicio mDNS
- Continuar actualización NTP
- Reanudar consultas a Google Calendar

---

## Almacenamiento en EEPROM

### Mapa de Memoria EEPROM (640 bytes)

| Dirección | Tamaño | Contenido |
|-----------|--------|-----------|
| 0-2 | 3 bytes | Firma "CFG" (validación) |
| 16 | 1 byte | DHCP habilitado |
| 17 | 1 byte | Horario de verano |
| 18-21 | 4 bytes | Intervalo actualización NTP |
| 22-25 | 4 bytes | Zona horaria |
| 32-35 | 4 bytes | Dirección IP |
| 36-39 | 4 bytes | Máscara de red |
| 40-43 | 4 bytes | Gateway |
| 64-95 | 32 bytes | SSID WiFi (red principal) |
| 96-127 | 32 bytes | Contraseña WiFi (red principal) |
| 128-159 | 32 bytes | Servidor NTP |
| 160-191 | 32 bytes | Nombre del dispositivo |
| 192-199 | 8 bytes | Primer día de la semana |
| 200-295 | 96 bytes | HolidaysScriptID |
| 296-391 | 96 bytes | AnniversariesScriptID |
| 392-487 | 96 bytes | TodosScriptID |
| **488** | **1 byte** | **Brillo manual** |
| **489** | **1 byte** | **Auto-brillo habilitado** |
| **490** | **1 byte** | **Brillo día** |
| **491** | **1 byte** | **Brillo noche** |
| **492** | **1 byte** | **Hora inicio día** |
| **493** | **1 byte** | **Hora inicio noche** |
| **494** | **1 byte** | **Colores personalizados habilitado** |
| **495-497** | **3 bytes** | **Color días laborables (HSV)** |
| **498-500** | **3 bytes** | **Color fines de semana (HSV)** |
| **501-503** | **3 bytes** | **Color día actual (HSV)** |
| **504-506** | **3 bytes** | **Color festivos (HSV)** |
| **507-509** | **3 bytes** | **Color aniversarios (HSV)** |
| **510-511** | **2 bytes** | **Color tareas (HS)** |
| **520-551** | **32 bytes** | **SSID WiFi (red respaldo)** |
| **552-583** | **32 bytes** | **Contraseña WiFi (red respaldo)** |

### Funciones de EEPROM

```cpp
// Escritura de configuración
void WriteConfig();

// Lectura de configuración (retorna true si existe)
boolean ReadConfig();

// Funciones auxiliares
void EEPROMWritelong(int address, long value);
long EEPROMReadlong(long address);
void WriteStringToEEPROM(int beginaddress, String string);
String ReadStringFromEEPROM(int beginaddress);      // Máx 32 chars
String ReadLongStringFromEEPROM(int beginaddress);  // Máx 96 chars
```

---

## Instalación y Configuración Inicial

### Opción A: Web Flasher (Recomendado)

La forma más fácil de instalar el firmware es usando el **Web Flasher** directamente desde el navegador. No requiere instalar ningún software.

**Requisitos:**
- Navegador Chrome, Edge u Opera (Firefox y Safari no soportan Web Serial)
- Cable USB para conectar el ESP8266
- Ningún otro programa usando el puerto serial (cerrar Arduino IDE si está abierto)

**Pasos:**

1. Conectar el ESP8266 (D1 Mini o compatible) al computador via USB
2. Abrir el Web Flasher: **https://XE1E.github.io/Perpetual-Calendar-with-Google-Calendar-Connection-V2/**
3. Seleccionar la versión a instalar:
   - **Standard** - Calendario básico
   - **Color Coded Clock** - Incluye reloj con colores en LEDs 62-67 (HH:MM:SS)
4. Hacer clic en "Install"
5. Seleccionar el puerto serial cuando el navegador lo solicite
6. Esperar ~30 segundos a que termine la instalación
7. Continuar con el **Paso 3: Configuración Inicial** (más abajo)

> **Nota:** Si el botón "Install" no aparece o muestra error, verificar que el navegador sea compatible y que el cable USB soporte datos (no solo carga).

---

### Opción B: Arduino IDE (Manual)

Si prefieres compilar el código tú mismo o necesitas hacer modificaciones:

#### Paso 1: Preparar el Entorno Arduino

1. Instalar Arduino IDE (1.8.x o 2.x)
2. Añadir soporte para ESP8266:
   - Ir a `Archivo > Preferencias`
   - En "URLs adicionales de gestor de placas" añadir:
     ```
     http://arduino.esp8266.com/stable/package_esp8266com_index.json
     ```
3. Instalar placa ESP8266 desde el gestor de placas
4. Instalar librerías requeridas desde el gestor de librerías:
   - **FastLED** - Control de LEDs WS2811/WS2812

**Librerías incluidas en ESP8266 Core (no requieren instalación):**
- ESP8266WiFi
- ESP8266WebServer
- ESP8266mDNS
- ESP8266WiFiMulti
- ArduinoOTA
- EEPROM
- Ticker

#### Paso 2: Cargar el Firmware

1. Seleccionar placa: `Herramientas > Placa > NodeMCU 1.0`
2. Seleccionar puerto COM correcto
3. Cargar el sketch

---

### Paso 3: Configuración Inicial (Ambas opciones)

1. El ESP8266 creará una red WiFi: `Calendario`
2. Conectarse con contraseña: `12345678`
3. Abrir navegador en: `http://192.168.4.1`
4. Configurar en orden:
   - **Red WiFi principal** (SSID y contraseña)
   - **Red WiFi respaldo** (opcional, recomendado)
   - **Servidor NTP** (ej: `pool.ntp.org`)
   - **Zona horaria**
   - **IDs de Google Apps Script**
5. Configuración opcional avanzada:
   - **Brillo de LEDs** (`/led.html`)
   - **Auto-brillo** (`/autobrightness.html`)
   - **Colores personalizados** (`/colors.html`)

### Paso 4: Acceso Normal

Una vez configurado, acceder mediante:
- **mDNS:** `http://perpetualcalendar.local`
- **IP directa:** `http://[IP_asignada]/`

> **Tip:** La IP se muestra en el Serial Monitor (115200 baud) al iniciar.

### Paso 5: Crear Google Apps Scripts

Para cada calendario (Holidays, Anniversaries, Todos), crear un script que:

1. Acceda al calendario correspondiente
2. Obtenga eventos del mes actual
3. Retorne los días como string: `"día1-día2-día3-"`

Ejemplo básico de script:
```javascript
function doGet() {
  var calendar = CalendarApp.getCalendarById('calendario@gmail.com');
  var now = new Date();
  var start = new Date(now.getFullYear(), now.getMonth(), 1);
  var end = new Date(now.getFullYear(), now.getMonth() + 1, 0);
  var events = calendar.getEvents(start, end);
  
  var days = "";
  for (var i = 0; i < events.length; i++) {
    days += events[i].getStartTime().getDate() + "-";
  }
  return ContentService.createTextOutput(days);
}
```

---

## Resolución de Problemas

### LED 62 Encendido en Rojo

**Problema:** No hay conexión WiFi

**Soluciones:**
1. Verificar SSID y contraseña
2. Verificar que el router esté encendido
3. Acercar el dispositivo al router
4. Reiniciar el ESP8266

### Animación Arcoíris Continua

**Problema:** No se recibe respuesta NTP

**Soluciones:**
1. Verificar conexión a Internet
2. Cambiar servidor NTP (probar `pool.ntp.org`)
3. Verificar que el puerto UDP 123 no esté bloqueado

### No se Muestran Eventos del Calendario

**Problema:** Los eventos de Google Calendar no aparecen

**Soluciones:**
1. Verificar los IDs de los scripts
2. Verificar que los scripts estén publicados como "web app"
3. Verificar permisos del script ("Cualquiera, incluso anónimo")
4. Probar el script directamente en el navegador

### Hora Incorrecta

**Problema:** La hora mostrada no coincide

**Soluciones:**
1. Verificar zona horaria configurada
2. Verificar configuración de horario de verano
3. Usar configuración manual de hora temporalmente

### Reinicio Constante

**Problema:** El ESP8266 se reinicia cada 30 segundos

**Causa:** El watchdog detecta que el loop está bloqueado

**Soluciones:**
1. Verificar conexión a Google (puede estar tardando demasiado)
2. Verificar estabilidad del WiFi
3. Revisar logs por Serial (115200 baud)

### No Funciona mDNS

**Problema:** No se puede acceder via `http://perpetualcalendar.local`

**Soluciones:**
1. Verificar que el dispositivo esté conectado a WiFi
2. En Windows: instalar Bonjour (viene con iTunes)
3. En Android: usar la IP directamente o instalar app Bonjour Browser
4. Esperar 30 segundos después de reinicio
5. Probar con otro navegador

### OTA No Aparece en Arduino IDE

**Problema:** El puerto de red no aparece en Herramientas > Puerto

**Soluciones:**
1. Verificar que Arduino IDE y ESP8266 estén en la misma red
2. Desactivar firewall temporalmente
3. Reiniciar Arduino IDE
4. Verificar que mDNS funcione (probar acceso web)

### Brillo No Se Guarda

**Problema:** El brillo vuelve al valor anterior tras reinicio

**Soluciones:**
1. Usar botón "Save" en lugar de solo mover el slider
2. Verificar que EEPROM no esté llena
3. Reiniciar el dispositivo para verificar

### WiFi Se Desconecta Frecuentemente

**Problema:** El dispositivo pierde conexión WiFi constantemente

**Soluciones:**
1. Configurar red de respaldo en `/config.html`
2. Acercar el dispositivo al router
3. Verificar que no haya interferencias en canal WiFi
4. Revisar logs por Serial para ver patrón de desconexiones

### Reloj No Se Actualiza

**Problema:** El reloj (Color Coded Clock) no cambia o parpadea incorrectamente

**Soluciones:**
1. Verificar que `#define COLOR_CODED_CLOCK` esté descomentado
2. Verificar que NTP esté funcionando (hora correcta)
3. Los LEDs del reloj (62-67) no deben estar dañados
4. Reiniciar el dispositivo

---

## Especificaciones Técnicas

| Parámetro | Valor |
|-----------|-------|
| Microcontrolador | ESP8266 |
| Frecuencia CPU | 80/160 MHz |
| Memoria Flash | 4 MB |
| EEPROM emulada | 640 bytes |
| LEDs soportados | 75 (WS2811/WS2812) |
| LEDs calendario | 68 (días) + 12 (meses) |
| LEDs reloj | 6 (HH:MM:SS) |
| Indicador WiFi | Reloj parpadea rojo |
| Brillo por defecto | 100/255 |
| Brillo mínimo | 5/255 (0 = apagado en modo noche) |
| Puerto web | 80 |
| Puerto OTA | 8266 |
| Puerto NTP | 2390 (cliente) |
| Watchdog timeout | 30 segundos |
| WiFi reconexión | 30 segundos |
| Velocidad Serial | 115200 baud |
| mDNS | perpetualcalendar.local |
| Redes WiFi soportadas | 2 (principal + respaldo) |

---

## Referencias

- [Web Flasher - Instalación desde navegador](https://XE1E.github.io/Perpetual-Calendar-with-Google-Calendar-Connection-V2/)
- [Instructables - Perpetual Calendar With Google Calendar Connection](https://www.instructables.com/Perpetual-Calendar-With-Google-Calendar-Connection/)
- [FastLED Library](https://github.com/FastLED/FastLED)
- [FastLED HSV Colors](https://github.com/FastLED/FastLED/wiki/FastLED-HSV-Colors)
- [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)
- [ESP8266 WiFiMulti](https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/station-class.html)
- [ArduinoOTA](https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html)
- [mDNS/Bonjour](https://arduino-esp8266.readthedocs.io/en/latest/esp8266mdns.html)
- [Google Apps Script](https://developers.google.com/apps-script)

---

## Historial de Versiones

### Versión 2.1
- Añadida página de información del sistema con recursos (RAM, Flash, CPU, Uptime)
- Añadido botón para reiniciar el dispositivo desde la web
- Añadido botón para borrar credenciales WiFi desde la web
- Mejorada persistencia de colores personalizados
- Mejorada interfaz de configuración de colores (botones más grandes)
- Añadido brillo fijo (120) para animación arcoíris
- Añadidas traducciones para nuevos elementos

### Versión 2.0
- Añadido soporte mDNS (acceso via .local)
- Añadido actualizaciones OTA
- Añadido control de brillo via web
- Añadido auto-brillo por hora del día
- Añadido personalización de colores via web
- Añadido soporte WiFi multi-red (principal + respaldo)
- Añadido reconexión WiFi automática
- Corregido display del reloj con colores
- Optimizado uso de memoria EEPROM (640 bytes)

### Versión 1.0
- Versión inicial
- Display de calendario con LEDs
- Integración con Google Calendar
- Configuración via interfaz web
- Sincronización NTP

---

*Manual generado para el proyecto Perpetual Calendar with Google Calendar Connection v2.1*
