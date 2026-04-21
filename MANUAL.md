# Manual Completo: Calendario Perpetuo con Conexión a Google Calendar

## Tabla de Contenidos

1. [Descripción General](#descripción-general)
2. [Requisitos de Hardware](#requisitos-de-hardware)
3. [Arquitectura del Sistema](#arquitectura-del-sistema)
4. [Estructura de Archivos](#estructura-de-archivos)
5. [Funcionamiento Detallado](#funcionamiento-detallado)
6. [Mapeo de LEDs](#mapeo-de-leds)
7. [Sistema de Colores](#sistema-de-colores)
8. [Sincronización NTP](#sincronización-ntp)
9. [Integración con Google Calendar](#integración-con-google-calendar)
10. [Interfaz Web de Configuración](#interfaz-web-de-configuración)
11. [Almacenamiento en EEPROM](#almacenamiento-en-eeprom)
12. [Instalación y Configuración Inicial](#instalación-y-configuración-inicial)
13. [Resolución de Problemas](#resolución-de-problemas)

---

## Descripción General

Este proyecto implementa un **calendario perpetuo físico** utilizando un microcontrolador ESP8266 y una tira de 75 LEDs WS2811. El calendario:

- Muestra visualmente todos los días del mes actual
- Se sincroniza automáticamente con la hora vía NTP
- Se conecta a Google Calendar para mostrar eventos
- Diferencia visualmente fines de semana, días festivos, aniversarios y tareas
- Se configura mediante una interfaz web integrada

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
├── Page_Admin.h                # Página principal de administración
├── Page_NetworkConfiguration.h # Configuración de red WiFi
├── Page_NTPSettings.h          # Configuración del servidor NTP
├── Page_AppsScriptSettings.h   # Configuración de Google Apps Script
├── Page_SetTime.h              # Configuración manual de hora
├── Page_Information.h          # Información del dispositivo
├── Page_Style.css.h            # Estilos CSS de la interfaz web
├── Page_Script.js.h            # JavaScript de la interfaz web
│
└── README.md                   # Documentación básica
```

### Descripción de Cada Archivo

| Archivo | Propósito |
|---------|-----------|
| `*.ino` | Contiene `setup()`, `loop()`, funciones de display LED y conexión a Google |
| `global.h` | Define la estructura de configuración, funciones EEPROM, servidor web |
| `NTP.h` | Obtiene hora UTC, calcula zona horaria, horario de verano, día de semana |
| `HTTPSRedirect.*` | Maneja conexiones HTTPS con redirección para Google Apps Script |
| `Page_*.h` | Páginas HTML embebidas para la interfaz de configuración web |

---

## Funcionamiento Detallado

### Flujo de Inicialización (setup)

```
1. Iniciar Serial (115200 baud)
2. Leer configuración desde EEPROM
3. Intentar conexión WiFi
   ├── Éxito: Modo Station (cliente WiFi)
   └── Fallo: Modo AP (punto de acceso "PerpetualCalendar-XXXX")
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
  - SSID: `PerpetualCalendar-XXXX` (XXXX = ID del chip)
  - Password: `admin1234`

### Páginas Disponibles

| URL | Descripción |
|-----|-------------|
| `/` | Página principal de administración |
| `/config.html` | Configuración de red WiFi |
| `/ntp.html` | Configuración del servidor NTP |
| `/appsscript.html` | IDs de Google Apps Script |
| `/time.html` | Configuración manual de hora |
| `/info.html` | Información del dispositivo |

### Endpoints de API

| Endpoint | Método | Descripción |
|----------|--------|-------------|
| `/admin/values` | GET | Valores de configuración de red |
| `/admin/connectionstate` | GET | Estado de conexión WiFi |
| `/admin/infovalues` | GET | Información del sistema |
| `/admin/ntpvalues` | GET | Configuración NTP |
| `/admin/appsscript` | GET | IDs de scripts |
| `/admin/timevalues` | GET | Hora actual |

---

## Almacenamiento en EEPROM

### Mapa de Memoria EEPROM (512 bytes)

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
| 64-95 | 32 bytes | SSID WiFi |
| 96-127 | 32 bytes | Contraseña WiFi |
| 128-159 | 32 bytes | Servidor NTP |
| 160-191 | 32 bytes | Nombre del dispositivo |
| 192-199 | 8 bytes | Primer día de la semana |
| 200-295 | 96 bytes | HolidaysScriptID |
| 296-391 | 96 bytes | AnniversariesScriptID |
| 392-487 | 96 bytes | TodosScriptID |

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

### Paso 1: Preparar el Entorno Arduino

1. Instalar Arduino IDE
2. Añadir soporte para ESP8266:
   - Ir a `Archivo > Preferencias`
   - En "URLs adicionales de gestor de placas" añadir:
     ```
     http://arduino.esp8266.com/stable/package_esp8266com_index.json
     ```
3. Instalar placa ESP8266 desde el gestor de placas
4. Instalar librería FastLED desde el gestor de librerías

### Paso 2: Cargar el Firmware

1. Seleccionar placa: `Herramientas > Placa > NodeMCU 1.0`
2. Seleccionar puerto COM correcto
3. Cargar el sketch

### Paso 3: Configuración Inicial

1. El ESP8266 creará una red WiFi: `PerpetualCalendar-XXXX`
2. Conectarse con contraseña: `admin1234`
3. Abrir navegador en: `http://192.168.4.1`
4. Configurar:
   - Red WiFi (SSID y contraseña)
   - Servidor NTP (ej: `pool.ntp.org`)
   - Zona horaria
   - IDs de Google Apps Script

### Paso 4: Crear Google Apps Scripts

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

---

## Especificaciones Técnicas

| Parámetro | Valor |
|-----------|-------|
| Microcontrolador | ESP8266 |
| Frecuencia CPU | 80/160 MHz |
| Memoria Flash | 4 MB |
| EEPROM emulada | 512 bytes |
| LEDs soportados | 75 (WS2811/WS2812) |
| Brillo por defecto | 100/255 |
| Puerto web | 80 |
| Puerto NTP | 2390 (cliente) |
| Watchdog timeout | 30 segundos |
| Velocidad Serial | 115200 baud |

---

## Referencias

- [Instructables - Perpetual Calendar With Google Calendar Connection](https://www.instructables.com/Perpetual-Calendar-With-Google-Calendar-Connection/)
- [FastLED Library](https://github.com/FastLED/FastLED)
- [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)
- [Google Apps Script](https://developers.google.com/apps-script)

---

*Manual generado para el proyecto Perpetual Calendar with Google Calendar Connection*
