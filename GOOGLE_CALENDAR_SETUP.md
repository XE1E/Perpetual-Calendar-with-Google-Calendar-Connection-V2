# Guia Completa: Configuracion de Google Calendar

## Tabla de Contenidos

0. [GUIA RAPIDA (10 minutos)](#guia-rapida-10-minutos) - **Empieza aqui**
1. [Introduccion](#introduccion)
2. [Requisitos Previos](#requisitos-previos)
3. [Arquitectura de la Integracion](#arquitectura-de-la-integracion)
4. [Parte 1: Crear los Calendarios en Google](#parte-1-crear-los-calendarios-en-google)
5. [Parte 2: Crear los Google Apps Scripts](#parte-2-crear-los-google-apps-scripts)
6. [Parte 3: Publicar los Scripts](#parte-3-publicar-los-scripts)
7. [Parte 4: Configurar el Dispositivo](#parte-4-configurar-el-dispositivo)
8. [Verificacion y Pruebas](#verificacion-y-pruebas)
9. [Resolucion de Problemas](#resolucion-de-problemas)

---

## GUIA RAPIDA (10 minutos)

> **Para usuarios que quieren configurar todo rapidamente.** Si tienes problemas, consulta las secciones detalladas mas abajo.

> 📷 **Nota sobre las imagenes:** Esta guia incluye marcadores de captura de pantalla (lineas que empiezan con 📷). Aun no hay imagenes; cada marcador indica que capturar y el nombre de archivo sugerido dentro de `docs/images/gcal/`. Para anadir una imagen, guardala con ese nombre y cambia la linea por `![descripcion](docs/images/gcal/nombre.png)`.

### Que necesitas

- Una cuenta de Google (Gmail)
- Tu Calendario Perpetuo conectado a WiFi

### Paso 1: Crea 3 calendarios en Google Calendar

1. Ve a **https://calendar.google.com**
2. En el panel izquierdo, haz clic en **"+" junto a "Otros calendarios"**
3. Selecciona **"Crear calendario"**
4. Crea estos 3 calendarios:
   - `Festivos`
   - `Aniversarios` 
   - `Tareas`

📷 *Captura: dialogo "Crear calendario" de Google Calendar* — `docs/images/gcal/01-crear-calendario.png`

### Paso 2: Obtén los IDs de tus calendarios

Para **cada** calendario que creaste:

1. Haz clic en los **3 puntos** junto al nombre del calendario
2. Selecciona **"Configuracion y uso compartido"**
3. Baja hasta **"Integrar el calendario"**
4. Copia el **"ID del calendario"** (algo como `abc123@group.calendar.google.com`)

📷 *Captura: seccion "Integrar el calendario" mostrando el ID del calendario* — `docs/images/gcal/02-id-calendario.png`

**Guarda los 3 IDs en un bloc de notas.**

### Paso 3: Crea el primer script (Festivos)

1. Ve a **https://script.google.com**
2. Haz clic en **"Nuevo proyecto"**
3. **Borra todo** el codigo y pega esto:

```javascript
function doGet() {
  var calendarId = 'PEGA_AQUI_TU_ID_DE_FESTIVOS';
  
  var calendar = CalendarApp.getCalendarById(calendarId);
  var now = new Date();
  var start = new Date(now.getFullYear(), now.getMonth(), 1);
  var end = new Date(now.getFullYear(), now.getMonth() + 1, 0);
  var events = calendar.getEvents(start, end);
  
  var days = "";
  var added = {};
  for (var i = 0; i < events.length; i++) {
    var d = events[i].getStartTime().getDate();
    if (!added[d]) { days += d + "-"; added[d] = true; }
  }
  return ContentService.createTextOutput(days);
}
```

4. **Reemplaza** `PEGA_AQUI_TU_ID_DE_FESTIVOS` con el ID real de tu calendario de Festivos
5. Presiona **Ctrl+S** para guardar
6. Nombra el proyecto: `Festivos`

📷 *Captura: editor de Apps Script con el codigo pegado* — `docs/images/gcal/03-editor-script.png`

### Paso 4: Publica el script

1. Haz clic en **"Implementar"** (boton azul arriba)
2. Selecciona **"Nueva implementacion"**
3. Haz clic en el **engranaje** > **"Aplicacion web"**
4. Configura:
   - Ejecutar como: **Yo**
   - Quien tiene acceso: **Cualquier usuario**

📷 *Captura: dialogo "Nueva implementacion" con tipo "Aplicacion web" y permisos* — `docs/images/gcal/04-nueva-implementacion.png`

5. Haz clic en **"Implementar"**
6. Haz clic en **"Autorizar acceso"**
7. Selecciona tu cuenta de Google
8. Clic en **"Avanzado"** > **"Ir a Festivos (no seguro)"** > **"Permitir"**

📷 *Captura: pantalla de advertencia "Google no ha verificado esta aplicacion" con el enlace "Avanzado"* — `docs/images/gcal/05-autorizar.png`

9. **COPIA el "ID de implementacion"** (empieza con `AKfycb...`)

📷 *Captura: dialogo final mostrando el "ID de implementacion"* — `docs/images/gcal/06-id-implementacion.png`

**Guarda este ID. Es el primero de 3.**

### Paso 5: Repite para Aniversarios y Tareas

Repite los pasos 3 y 4 dos veces mas:

**Para Aniversarios:**
1. Nuevo proyecto en script.google.com
2. Pega el mismo codigo
3. Cambia el ID por el de tu calendario de Aniversarios
4. Publica y copia el ID de implementacion

**Para Tareas:**
1. Nuevo proyecto en script.google.com
2. Pega el mismo codigo
3. Cambia el ID por el de tu calendario de Tareas
4. Publica y copia el ID de implementacion

**Ahora tienes 3 IDs de implementacion.**

### Paso 6: Configura el Calendario Perpetuo

1. Abre tu navegador
2. Ve a **http://perpetualcalendar.local/appsscript.html**
   (o usa la IP de tu dispositivo: `http://192.168.X.X/appsscript.html`)
3. Pega los IDs:
   - **Holidays:** ID del script de Festivos
   - **Anniversaries:** ID del script de Aniversarios
   - **To-dos:** ID del script de Tareas
4. Haz clic en **"Save"**

📷 *Captura: pagina `appsscript.html` del dispositivo con los 3 IDs pegados* — `docs/images/gcal/07-config-dispositivo.png`

### Paso 7: Prueba que funciona

1. Ve a Google Calendar
2. Agrega un evento para **hoy** en el calendario de "Tareas"
3. Espera 1-2 minutos
4. El LED del dia de hoy deberia cambiar a **naranja**

### Listo!

Tu Calendario Perpetuo ahora muestra:
- **Purpura:** Dias con festivos
- **Cian:** Dias con aniversarios
- **Naranja:** Dias con tareas

> **Problemas?** Consulta la seccion [Resolucion de Problemas](#resolucion-de-problemas) mas abajo.

---

## Introduccion

El Calendario Perpetuo se conecta a Google Calendar mediante **Google Apps Script**, un servicio gratuito de Google que permite crear aplicaciones web simples. La integracion funciona asi:

```
ESP8266 --> Google Apps Script --> Google Calendar API --> Tus Calendarios
```

El dispositivo consulta 3 calendarios diferentes:
- **Holidays (Festivos):** Dias feriados, vacaciones
- **Anniversaries (Aniversarios):** Cumpleanos, fechas especiales
- **To-dos (Tareas):** Recordatorios, eventos del dia a dia

Cada tipo de evento se muestra con un color diferente en el calendario fisico.

---

## Requisitos Previos

Antes de comenzar, asegurate de tener:

- [ ] Una cuenta de Google (Gmail)
- [ ] Acceso a Google Calendar (calendar.google.com)
- [ ] Acceso a Google Apps Script (script.google.com)
- [ ] El Calendario Perpetuo conectado a tu red WiFi
- [ ] Conocer la IP del dispositivo o acceso via `http://perpetualcalendar.local`

---

## Arquitectura de la Integracion

### Como funciona

```
                    INTERNET
                       |
    +------------------+------------------+
    |                  |                  |
    v                  v                  v
+--------+        +--------+        +--------+
|Script 1|        |Script 2|        |Script 3|
|Holidays|        |Anniv.  |        | Todos  |
+---+----+        +---+----+        +---+----+
    |                 |                  |
    v                 v                  v
+--------+        +--------+        +--------+
|Calendar|        |Calendar|        |Calendar|
|Festivos|        |Cumples |        |Tareas  |
+--------+        +--------+        +--------+
```

### Flujo de datos

1. El ESP8266 hace una peticion HTTPS a cada Google Apps Script
2. El Script consulta el calendario correspondiente via Google Calendar API
3. El Script devuelve una lista de dias con eventos: `"5-12-25-"`
4. El ESP8266 interpreta los dias y enciende los LEDs correspondientes

### Frecuencia de actualizacion

| Calendario | Frecuencia | Razon |
|------------|------------|-------|
| Todos | Cada minuto | Las tareas pueden cambiar frecuentemente |
| Holidays | Cada hora | Los festivos son relativamente estaticos |
| Anniversaries | Cada hora | Los aniversarios son estaticos |

---

## Parte 1: Crear los Calendarios en Google

### Paso 1.1: Acceder a Google Calendar

1. Abre tu navegador
2. Ve a: **https://calendar.google.com**
3. Inicia sesion con tu cuenta de Google

### Paso 1.2: Crear el Calendario de Festivos

1. En el panel izquierdo, busca **"Otros calendarios"**
2. Haz clic en el **+** (Agregar otros calendarios)
3. Selecciona **"Crear calendario"**
4. Completa los datos:
   - **Nombre:** `Festivos` (o como prefieras)
   - **Descripcion:** Dias feriados y vacaciones
   - **Zona horaria:** Tu zona horaria
5. Haz clic en **"Crear calendario"**

### Paso 1.3: Crear el Calendario de Aniversarios

Repite el proceso anterior con:
- **Nombre:** `Aniversarios`
- **Descripcion:** Cumpleanos y fechas especiales

### Paso 1.4: Crear el Calendario de Tareas

Repite el proceso anterior con:
- **Nombre:** `Tareas`
- **Descripcion:** Recordatorios y eventos diarios

> **Tip:** Tambien puedes usar calendarios existentes o el calendario principal. Solo necesitas el ID del calendario.

### Paso 1.5: Obtener los IDs de los Calendarios

Para cada calendario creado:

1. Haz clic en los **tres puntos** junto al nombre del calendario
2. Selecciona **"Configuracion y uso compartido"**
3. Desplazate hacia abajo hasta **"Integrar el calendario"**
4. Copia el **"ID del calendario"**

El ID tiene este formato:
```
abc123xyz@group.calendar.google.com
```

O para el calendario principal:
```
tu-email@gmail.com
```

**Guarda estos 3 IDs, los necesitaras mas adelante.**

### Paso 1.6: Agregar eventos de prueba

Para verificar que todo funciona, agrega algunos eventos:

1. En el calendario de **Festivos**, agrega un evento para hoy o manana
2. En el calendario de **Aniversarios**, agrega un cumpleanos este mes
3. En el calendario de **Tareas**, agrega una tarea para hoy

---

## Parte 2: Crear los Google Apps Scripts

### Paso 2.1: Acceder a Google Apps Script

1. Abre tu navegador
2. Ve a: **https://script.google.com**
3. Inicia sesion con la misma cuenta de Google

### Paso 2.2: Crear el Script para Festivos

1. Haz clic en **"Nuevo proyecto"**
2. Se abrira el editor con un archivo `Codigo.gs`
3. **Borra todo el contenido** y pega el siguiente codigo:

```javascript
function doGet() {
  // IMPORTANTE: Reemplaza con el ID de tu calendario de Festivos
  var calendarId = 'TU_CALENDAR_ID_FESTIVOS@group.calendar.google.com';
  
  try {
    var calendar = CalendarApp.getCalendarById(calendarId);
    
    if (!calendar) {
      return ContentService.createTextOutput('Error: Calendario no encontrado');
    }
    
    // Obtener primer y ultimo dia del mes actual
    var now = new Date();
    var firstDay = new Date(now.getFullYear(), now.getMonth(), 1);
    var lastDay = new Date(now.getFullYear(), now.getMonth() + 1, 0);
    
    // Obtener todos los eventos del mes
    var events = calendar.getEvents(firstDay, lastDay);
    
    // Construir string con los dias que tienen eventos
    var days = "";
    var addedDays = {}; // Para evitar duplicados
    
    for (var i = 0; i < events.length; i++) {
      var eventDate = events[i].getStartTime();
      var day = eventDate.getDate();
      
      // Evitar duplicados del mismo dia
      if (!addedDays[day]) {
        days += day + "-";
        addedDays[day] = true;
      }
    }
    
    return ContentService.createTextOutput(days);
    
  } catch (e) {
    return ContentService.createTextOutput('Error: ' + e.toString());
  }
}
```

4. **IMPORTANTE:** Reemplaza `TU_CALENDAR_ID_FESTIVOS@group.calendar.google.com` con el ID real de tu calendario de Festivos (el que copiaste en el Paso 1.5)

5. Haz clic en el icono de disquete o presiona `Ctrl+S` para guardar

6. Cambia el nombre del proyecto:
   - Haz clic en **"Proyecto sin titulo"** arriba a la izquierda
   - Escribe: `Calendario Festivos`
   - Presiona Enter

### Paso 2.3: Crear el Script para Aniversarios

1. Ve a **https://script.google.com** nuevamente
2. Haz clic en **"Nuevo proyecto"**
3. Pega el mismo codigo del Paso 2.2
4. Cambia el `calendarId` por el ID de tu calendario de Aniversarios
5. Guarda y renombra el proyecto a: `Calendario Aniversarios`

### Paso 2.4: Crear el Script para Tareas

1. Ve a **https://script.google.com** nuevamente
2. Haz clic en **"Nuevo proyecto"**
3. Pega el mismo codigo del Paso 2.2
4. Cambia el `calendarId` por el ID de tu calendario de Tareas
5. Guarda y renombra el proyecto a: `Calendario Tareas`

---

## Parte 3: Publicar los Scripts

Cada script debe publicarse como una "aplicacion web" para que el ESP8266 pueda acceder a el.

### Paso 3.1: Publicar el Script de Festivos

1. Abre el proyecto **"Calendario Festivos"** en script.google.com
2. Haz clic en **"Implementar"** (boton azul arriba a la derecha)
3. Selecciona **"Nueva implementacion"**
4. Haz clic en el engranaje junto a "Seleccionar tipo"
5. Selecciona **"Aplicacion web"**
6. Configura:
   - **Descripcion:** `Festivos v1`
   - **Ejecutar como:** `Yo (tu-email@gmail.com)`
   - **Quien tiene acceso:** `Cualquier usuario`
7. Haz clic en **"Implementar"**

### Paso 3.2: Autorizar el Script

La primera vez que publiques, Google te pedira autorizacion:

1. Haz clic en **"Autorizar acceso"**
2. Selecciona tu cuenta de Google
3. Aparecera una advertencia: "Google no ha verificado esta aplicacion"
4. Haz clic en **"Avanzado"** o **"Configuracion avanzada"**
5. Haz clic en **"Ir a Calendario Festivos (no seguro)"**
6. Revisa los permisos y haz clic en **"Permitir"**

### Paso 3.3: Copiar el ID de Implementacion

Despues de autorizar, veras una pantalla con:
- **ID de implementacion:** Un codigo largo como `AKfycbx...`
- **URL de la aplicacion web:** `https://script.google.com/macros/s/AKfycbx.../exec`

**IMPORTANTE:** Copia el **ID de implementacion** (NO la URL completa, solo el ID).

El ID tiene este formato:
```
AKfycbxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
```

**Guarda este ID. Lo necesitaras para configurar el dispositivo.**

### Paso 3.4: Publicar los otros Scripts

Repite los pasos 3.1 a 3.3 para:
- **Calendario Aniversarios**
- **Calendario Tareas**

Al final tendras **3 IDs de implementacion**, uno para cada script.

### Paso 3.5: Verificar los Scripts

Puedes probar cada script en tu navegador:

1. Copia la **URL de la aplicacion web** (la que termina en `/exec`)
2. Pegala en una nueva pestana del navegador
3. Deberia mostrar algo como: `5-12-25-` (los dias con eventos)
4. Si no hay eventos este mes, mostrara una linea vacia

---

## Parte 4: Configurar el Dispositivo

### Paso 4.1: Acceder a la Interfaz Web

1. Asegurate de que el Calendario Perpetuo este encendido y conectado a WiFi
2. Abre tu navegador
3. Ve a: `http://perpetualcalendar.local` o usa la IP del dispositivo

### Paso 4.2: Ir a la Pagina de Apps Script

1. En la pagina principal, busca el enlace **"Apps Script"** o **"Script Settings"**
2. O ve directamente a: `http://perpetualcalendar.local/appsscript.html`

### Paso 4.3: Ingresar los IDs

Veras 3 campos de texto:

```
+------------------------------------------+
|    Apps Script Deployment IDs            |
+------------------------------------------+
| Holidays:     [_________________________]|
| Anniversaries:[_________________________]|
| To-dos:       [_________________________]|
|                                          |
|              [ Save ]                    |
+------------------------------------------+
```

1. En **Holidays**, pega el ID de implementacion del script de Festivos
2. En **Anniversaries**, pega el ID de implementacion del script de Aniversarios
3. En **To-dos**, pega el ID de implementacion del script de Tareas
4. Haz clic en **Save**

### Paso 4.4: Reiniciar el Dispositivo (Opcional)

Para que los cambios surtan efecto inmediatamente:

1. Desconecta la alimentacion del ESP8266
2. Espera 5 segundos
3. Vuelve a conectar

O espera aproximadamente 1 minuto para que se actualice automaticamente.

---

## Verificacion y Pruebas

### Verificar que funciona

1. Agrega un evento en uno de tus calendarios de Google para **hoy**
2. Espera 1-2 minutos
3. El LED correspondiente a hoy deberia cambiar de color:
   - **Naranja** si es una tarea (To-do)
   - **Purpura** si es un festivo (Holiday)
   - **Cian** si es un aniversario (Anniversary)

### Verificar via Serial Monitor

Si tienes el ESP8266 conectado por USB:

1. Abre Arduino IDE
2. Ve a `Herramientas > Monitor Serie`
3. Configura a **115200 baud**
4. Busca mensajes como:
   ```
   Calendar Data---> 5-12-25-
   ```
   Esto indica que el script esta respondiendo correctamente.

### Forzar actualizacion manual

1. Ve a `http://perpetualcalendar.local/led.html`
2. Haz clic en **"Refresh Calendar"**
3. Los eventos se actualizaran inmediatamente

---

## Resolucion de Problemas

### Problema: "Error: Calendario no encontrado"

**Causa:** El ID del calendario es incorrecto.

**Solucion:**
1. Ve a Google Calendar
2. Configuracion del calendario > Integrar el calendario
3. Copia el ID correcto (termina en `@group.calendar.google.com` o `@gmail.com`)
4. Actualiza el script y vuelve a publicar

### Problema: Los eventos no aparecen

**Verificaciones:**
1. El evento esta en el calendario correcto?
2. El evento tiene fecha dentro del mes actual?
3. El ID de implementacion es correcto?
4. Probaste el script directamente en el navegador?

**Solucion:**
1. Abre la URL del script en el navegador
2. Si muestra error, revisa el codigo del script
3. Si muestra datos, verifica el ID en el dispositivo

### Problema: Error 401 o "No autorizado"

**Causa:** El script no tiene los permisos correctos.

**Solucion:**
1. Ve al proyecto en script.google.com
2. Implementar > Administrar implementaciones
3. Elimina la implementacion actual
4. Crea una nueva implementacion
5. Asegurate de seleccionar "Cualquier usuario" en permisos
6. Copia el nuevo ID y actualizalo en el dispositivo

### Problema: Error de conexion HTTPS

**Causa:** El ESP8266 no puede conectar con Google.

**Verificaciones:**
1. El dispositivo tiene conexion a Internet?
2. La animacion arcoiris se detuvo? (indica que NTP respondio)
3. Puedes acceder a la interfaz web del dispositivo?

**Solucion:**
1. Verifica la conexion WiFi
2. Reinicia el dispositivo
3. Revisa que el router tenga acceso a Internet

### Problema: "Could not connect to server"

**Causa:** Google esta rechazando la conexion.

**Solucion:**
1. Los scripts de Google usan HTTPS, verifica que la hora del dispositivo sea correcta
2. Ve a `/ntp.html` y verifica la configuracion NTP
3. El certificado SSL del ESP8266 expira, actualiza el firmware si es muy antiguo

### Problema: Script funciona en navegador pero no en ESP8266

**Verificacion:**
1. Asegurate de copiar solo el ID de implementacion, no la URL completa
2. El ID debe empezar con `AKfycb`
3. No debe incluir `https://` ni `/exec`

**Formato correcto:**
```
AKfycbx1234567890abcdefghijklmnopqrstuvwxyz123456
```

**Formato incorrecto:**
```
https://script.google.com/macros/s/AKfycbx.../exec  <-- NO usar URL
```

---

## Codigo del Script (Referencia Completa)

Este es el codigo completo con comentarios explicativos:

```javascript
/**
 * Google Apps Script para Calendario Perpetuo
 * Este script devuelve los dias del mes actual que tienen eventos
 * 
 * Formato de respuesta: "dia1-dia2-dia3-"
 * Ejemplo: "5-12-25-" = eventos los dias 5, 12 y 25
 */

function doGet() {
  // ============================================================
  // CONFIGURACION - Modifica esta linea con tu ID de calendario
  // ============================================================
  var calendarId = 'TU_CALENDAR_ID@group.calendar.google.com';
  
  // Para usar tu calendario principal, usa tu email:
  // var calendarId = 'tu-email@gmail.com';
  
  try {
    // Obtener referencia al calendario
    var calendar = CalendarApp.getCalendarById(calendarId);
    
    // Verificar que el calendario existe
    if (!calendar) {
      return ContentService.createTextOutput('Error: Calendario no encontrado. Verifica el ID.');
    }
    
    // Calcular rango de fechas del mes actual
    var now = new Date();
    var year = now.getFullYear();
    var month = now.getMonth(); // 0-11
    
    var firstDay = new Date(year, month, 1);                    // Primer dia del mes
    var lastDay = new Date(year, month + 1, 0, 23, 59, 59);    // Ultimo dia del mes
    
    // Obtener todos los eventos en el rango
    var events = calendar.getEvents(firstDay, lastDay);
    
    // Construir respuesta
    var days = "";
    var addedDays = {}; // Objeto para rastrear dias ya agregados
    
    for (var i = 0; i < events.length; i++) {
      var event = events[i];
      var startDate = event.getStartTime();
      var day = startDate.getDate();
      
      // Solo agregar cada dia una vez (evitar duplicados)
      if (!addedDays[day]) {
        days += day + "-";
        addedDays[day] = true;
      }
      
      // Para eventos de varios dias, agregar todos los dias del evento
      var endDate = event.getEndTime();
      if (endDate.getDate() !== day) {
        // Es un evento de varios dias
        var currentDate = new Date(startDate);
        while (currentDate < endDate && currentDate.getMonth() === month) {
          var currentDay = currentDate.getDate();
          if (!addedDays[currentDay]) {
            days += currentDay + "-";
            addedDays[currentDay] = true;
          }
          currentDate.setDate(currentDate.getDate() + 1);
        }
      }
    }
    
    // Devolver los dias como texto plano
    return ContentService.createTextOutput(days);
    
  } catch (e) {
    // En caso de error, devolver mensaje descriptivo
    return ContentService.createTextOutput('Error: ' + e.toString());
  }
}

/**
 * Funcion de prueba - Ejecutar desde el editor para verificar
 */
function testScript() {
  var result = doGet();
  Logger.log(result.getContent());
}
```

---

## Como Funciona el Codigo en el ESP8266

### Conexion HTTPS

El ESP8266 usa la libreria `HTTPSRedirect` para conectar con Google:

```cpp
// Archivo: Perpetual-Calendar-With-Google-Calendar-Connection.ino

const char* host = "script.google.com";
const int httpsPort = 443;

void getCalendar(String GScriptId) {
    // Construir URL: /macros/s/[ID]/exec
    String url = String("/macros/s/") + GScriptId + "/exec";
    
    // Hacer peticion HTTPS GET
    client->GET(url, host);
    
    // Obtener respuesta (ej: "5-12-25-")
    calendarData = client->getResponseBody();
}
```

### Procesamiento de la Respuesta

Los dias se extraen del string y se guardan en arrays:

```cpp
void initDatesArray(int (&Dates)[20], String calendarString) {
    // Ejemplo: "5-12-25-" -> Dates = [5, 12, 25, 0, 0, ...]
    
    while(calendarString.indexOf("-") != -1) {
        // Extraer numero antes del guion
        Dates[index] = calendarString.substring(0, 
                         calendarString.indexOf("-")).toInt();
        // Avanzar al siguiente numero
        calendarString = calendarString.substring(
                         calendarString.indexOf("-") + 1);
    }
}
```

### Visualizacion en LEDs

Los eventos se muestran con colores especificos:

```cpp
void EventsDisplay() {
    for (int i = 1; i <= 31; i++) {
        // Festivos: Purpura
        for (int j = 0; j < 20; j++) {
            if (Holidays[j] == i) 
                leds[Days[i-1]] = holidays_color;  // Purpura
        }
        // Aniversarios: Cian
        for (int j = 0; j < 20; j++) {
            if (Anniversaries[j] == i) 
                leds[Days[i-1]] = anniversaries_color;  // Cian
        }
        // Tareas: Naranja (mayor prioridad visual)
        for (int j = 0; j < 20; j++) {
            if (Todos[j] == i) 
                leds[Days[i-1]] = todos_color;  // Naranja
        }
    }
}
```

### Jerarquia de Colores

Si un dia tiene multiples tipos de eventos, el ultimo sobrescribe:

```
1. Color base (verde laborable / rojo fin de semana)
2. Dia actual (azul)
3. Festivos (purpura)
4. Aniversarios (cian)
5. Tareas (naranja) <-- Mayor prioridad
```

---

## Consejos Adicionales

### Usar calendarios existentes

No necesitas crear calendarios nuevos. Puedes usar:
- Tu calendario principal de Gmail
- Calendarios de festivos de tu pais (Google ofrece estos gratis)
- Calendarios compartidos

### Multiples eventos el mismo dia

El script solo reporta el **dia**, no la cantidad de eventos. Si tienes 5 reuniones el dia 15, el script devuelve `15-` una sola vez.

### Eventos recurrentes

Los eventos recurrentes (cumpleanos anuales, reuniones semanales) funcionan automaticamente. El script detecta todas las ocurrencias dentro del mes.

### Privacidad

- Los scripts solo leen los **dias** con eventos, no los titulos ni detalles
- El ESP8266 no almacena tus datos de calendario
- Todo el trafico es HTTPS (encriptado)

### Limites de Google

- Google Apps Script tiene limites de ejecucion gratuitos
- Para uso personal, no deberias alcanzar estos limites
- Limite aproximado: 90 minutos de ejecucion por dia

---

*Guia creada para el proyecto Perpetual Calendar with Google Calendar Connection v2.2*
