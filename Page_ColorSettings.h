#ifndef PAGE_COLORSETTINGS_H
#define PAGE_COLORSETTINGS_H

// EEPROM addresses for custom colors (584-607)
// Note: 489-495 used by AutoBrightness, 520-583 used by backup WiFi
#define EEPROM_CUSTOM_COLORS_ENABLED 584
#define EEPROM_COLOR_WEEKDAY 585      // 3 bytes (H, S, V)
#define EEPROM_COLOR_WEEKEND 588      // 3 bytes
#define EEPROM_COLOR_TODAY 591        // 3 bytes
#define EEPROM_COLOR_HOLIDAY 594      // 3 bytes
#define EEPROM_COLOR_ANNIVERSARY 597  // 3 bytes
#define EEPROM_COLOR_TODO 600         // 3 bytes
#define EEPROM_COLOR_MONTH 603        // 3 bytes
#define EEPROM_COLOR_CORRECTION 606   // 1 byte (0=none, 1=TypicalLEDStrip, 2=TypicalSMD5050)
#define EEPROM_COLOR_TEMPERATURE 607  // 1 byte (0=none, 1=DirectSunlight, 2=Tungsten100W, 3=ClearBlueSky)

// Color correction settings
uint8_t colorCorrectionMode = 1;  // Default: TypicalLEDStrip
uint8_t colorTemperatureMode = 1; // Default: DirectSunlight

struct CustomColors {
    bool enabled;
    CHSV weekday;
    CHSV weekend;
    CHSV today;
    CHSV holiday;
    CHSV anniversary;
    CHSV todo;
    CHSV month;
} customColors;

void loadCustomColors() {
    // 1. Leer enabled correctamente (solo true si es exactamente 1)
    uint8_t enabledByte = EEPROM.read(EEPROM_CUSTOM_COLORS_ENABLED);
    customColors.enabled = (enabledByte == 1);

    // 2. SIEMPRE cargar colores de EEPROM al struct (para mostrar en UI)
    customColors.weekday = CHSV(EEPROM.read(EEPROM_COLOR_WEEKDAY),
                                 EEPROM.read(EEPROM_COLOR_WEEKDAY + 1),
                                 EEPROM.read(EEPROM_COLOR_WEEKDAY + 2));
    customColors.weekend = CHSV(EEPROM.read(EEPROM_COLOR_WEEKEND),
                                 EEPROM.read(EEPROM_COLOR_WEEKEND + 1),
                                 EEPROM.read(EEPROM_COLOR_WEEKEND + 2));
    customColors.today = CHSV(EEPROM.read(EEPROM_COLOR_TODAY),
                               EEPROM.read(EEPROM_COLOR_TODAY + 1),
                               EEPROM.read(EEPROM_COLOR_TODAY + 2));
    customColors.holiday = CHSV(EEPROM.read(EEPROM_COLOR_HOLIDAY),
                                 EEPROM.read(EEPROM_COLOR_HOLIDAY + 1),
                                 EEPROM.read(EEPROM_COLOR_HOLIDAY + 2));
    customColors.anniversary = CHSV(EEPROM.read(EEPROM_COLOR_ANNIVERSARY),
                                     EEPROM.read(EEPROM_COLOR_ANNIVERSARY + 1),
                                     EEPROM.read(EEPROM_COLOR_ANNIVERSARY + 2));
    customColors.todo = CHSV(EEPROM.read(EEPROM_COLOR_TODO),
                              EEPROM.read(EEPROM_COLOR_TODO + 1),
                              EEPROM.read(EEPROM_COLOR_TODO + 2));
    customColors.month = CHSV(EEPROM.read(EEPROM_COLOR_MONTH),
                               EEPROM.read(EEPROM_COLOR_MONTH + 1),
                               EEPROM.read(EEPROM_COLOR_MONTH + 2));

    // 3. Validar EEPROM no inicializada (255 = no escrita)
    if (customColors.weekday.h == 255 && customColors.weekday.s == 255) {
        // EEPROM vacía, usar defaults
        customColors.weekday = CHSV(96, 255, 192);
        customColors.weekend = CHSV(0, 255, 128);
        customColors.today = CHSV(160, 255, 192);
        customColors.holiday = CHSV(192, 255, 255);
        customColors.anniversary = CHSV(128, 255, 255);
        customColors.todo = CHSV(32, 255, 192);
        customColors.month = CHSV(0, 0, 192);
    }

    // 4. SOLO aplicar a LEDs si está habilitado
    if (customColors.enabled) {
        weekday_color = customColors.weekday;
        weekend_color = customColors.weekend;
        actualday_color = customColors.today;
        holidays_color = customColors.holiday;
        anniversaries_color = customColors.anniversary;
        todos_color = customColors.todo;
        month_color = customColors.month;
    }
}

void saveCustomColors() {
    EEPROM.write(EEPROM_CUSTOM_COLORS_ENABLED, customColors.enabled ? 1 : 0);

    EEPROM.write(EEPROM_COLOR_WEEKDAY, customColors.weekday.h);
    EEPROM.write(EEPROM_COLOR_WEEKDAY + 1, customColors.weekday.s);
    EEPROM.write(EEPROM_COLOR_WEEKDAY + 2, customColors.weekday.v);

    EEPROM.write(EEPROM_COLOR_WEEKEND, customColors.weekend.h);
    EEPROM.write(EEPROM_COLOR_WEEKEND + 1, customColors.weekend.s);
    EEPROM.write(EEPROM_COLOR_WEEKEND + 2, customColors.weekend.v);

    EEPROM.write(EEPROM_COLOR_TODAY, customColors.today.h);
    EEPROM.write(EEPROM_COLOR_TODAY + 1, customColors.today.s);
    EEPROM.write(EEPROM_COLOR_TODAY + 2, customColors.today.v);

    EEPROM.write(EEPROM_COLOR_HOLIDAY, customColors.holiday.h);
    EEPROM.write(EEPROM_COLOR_HOLIDAY + 1, customColors.holiday.s);
    EEPROM.write(EEPROM_COLOR_HOLIDAY + 2, customColors.holiday.v);

    EEPROM.write(EEPROM_COLOR_ANNIVERSARY, customColors.anniversary.h);
    EEPROM.write(EEPROM_COLOR_ANNIVERSARY + 1, customColors.anniversary.s);
    EEPROM.write(EEPROM_COLOR_ANNIVERSARY + 2, customColors.anniversary.v);

    EEPROM.write(EEPROM_COLOR_TODO, customColors.todo.h);
    EEPROM.write(EEPROM_COLOR_TODO + 1, customColors.todo.s);
    EEPROM.write(EEPROM_COLOR_TODO + 2, customColors.todo.v);

    EEPROM.write(EEPROM_COLOR_MONTH, customColors.month.h);
    EEPROM.write(EEPROM_COLOR_MONTH + 1, customColors.month.s);
    EEPROM.write(EEPROM_COLOR_MONTH + 2, customColors.month.v);

    EEPROM.commit();
}

void applyColorCorrection() {
    // Apply color correction
    switch (colorCorrectionMode) {
        case 0: FastLED.setCorrection(UncorrectedColor); break;
        case 1: FastLED.setCorrection(TypicalLEDStrip); break;
        case 2: FastLED.setCorrection(TypicalSMD5050); break;
        default: FastLED.setCorrection(TypicalLEDStrip); break;
    }

    // Apply color temperature
    switch (colorTemperatureMode) {
        case 0: FastLED.setTemperature(UncorrectedTemperature); break;
        case 1: FastLED.setTemperature(DirectSunlight); break;
        case 2: FastLED.setTemperature(Tungsten100W); break;
        case 3: FastLED.setTemperature(ClearBlueSky); break;
        default: FastLED.setTemperature(DirectSunlight); break;
    }
}

void loadColorCorrection() {
    uint8_t corr = EEPROM.read(EEPROM_COLOR_CORRECTION);
    uint8_t temp = EEPROM.read(EEPROM_COLOR_TEMPERATURE);

    // Validate values (255 = uninitialized EEPROM)
    colorCorrectionMode = (corr <= 2) ? corr : 1;
    colorTemperatureMode = (temp <= 3) ? temp : 1;

    applyColorCorrection();
}

void saveColorCorrection() {
    EEPROM.write(EEPROM_COLOR_CORRECTION, colorCorrectionMode);
    EEPROM.write(EEPROM_COLOR_TEMPERATURE, colorTemperatureMode);
    EEPROM.commit();
}

const char PAGE_ColorSettings[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<body>
<div style="display:flex;justify-content:space-between;align-items:center">
<strong data-i18n="color_settings">Configuracion Colores</strong>
<button id="langBtn" onclick="toggleLang()" class="btn btn--s btn--blue">EN</button>
</div>
<hr>
<form id="colorForm">
<table style="width:310px">
<tr><td colspan="2"><input type="checkbox" id="enabled" onchange="toggleColors()"> <span data-i18n="custom_colors">Activar colores personalizados</span></td></tr>
<tr><td style="width:120px" data-i18n="weekday_color">Dias laborales:</td><td><input type="color" id="weekday" value="#00ff00"></td></tr>
<tr><td data-i18n="weekend_color">Fin de semana:</td><td><input type="color" id="weekend" value="#ff0000"></td></tr>
<tr><td data-i18n="today_color">Dia actual:</td><td><input type="color" id="today" value="#0000ff"></td></tr>
<tr><td data-i18n="month_color">Mes actual:</td><td><input type="color" id="month" value="#c0c0c0"></td></tr>
<tr><td data-i18n="holiday_color">Festivos:</td><td><input type="color" id="holiday" value="#ff00ff"></td></tr>
<tr><td data-i18n="anniversary_color">Aniversarios:</td><td><input type="color" id="anniversary" value="#00ffff"></td></tr>
<tr><td data-i18n="todo_color">Tareas:</td><td><input type="color" id="todo" value="#ffa500"></td></tr>
<tr><td colspan="2" style="padding-top:10px">
<input type="button" id="previewBtn" style="width:100px" class="btn btn--s btn--grey" value="Vista Previa" onclick="previewColors()">
<input type="button" id="saveBtn" style="width:80px" class="btn btn--s btn--grey" value="Guardar" onclick="saveColors()">
</td></tr>
<tr><td colspan="2">
<input type="button" id="resetBtn" style="width:210px" class="btn btn--s btn--grey" value="Restablecer" onclick="resetColors()">
</td></tr>
</table>
</form>
<hr>
<strong data-i18n="gamma_correction">Correccion Gamma</strong>
<table style="width:310px">
<tr><td style="width:120px" data-i18n="color_profile">Perfil color:</td><td>
<select id="correction" onchange="previewGamma()">
<option value="0" data-i18n="no_correction">Sin correccion</option>
<option value="1" data-i18n="typical_led">TypicalLEDStrip</option>
<option value="2" data-i18n="typical_smd">TypicalSMD5050</option>
</select>
</td></tr>
<tr><td data-i18n="color_temp">Temperatura:</td><td>
<select id="temperature" onchange="previewGamma()">
<option value="0" data-i18n="no_adjustment">Sin ajuste</option>
<option value="1" data-i18n="direct_sun">DirectSunlight</option>
<option value="2" data-i18n="tungsten">Tungsten100W</option>
<option value="3" data-i18n="clear_sky">ClearBlueSky</option>
</select>
</td></tr>
<tr><td colspan="2" style="padding-top:10px">
<input type="button" id="saveGammaBtn" style="width:210px" class="btn btn--s btn--grey" value="Guardar Gamma" onclick="saveGamma()">
</td></tr>
</table>
<hr>
<a href="/" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="back">Volver</span></a>
</body>
<script>
window.onload = function() {
    load("style.css","css", function() {
        load("microajax.js","js", function() {
            initLang();
            loadColors();
            loadGamma();
            document.getElementById("previewBtn").value = t("preview");
            document.getElementById("saveBtn").value = t("save");
            document.getElementById("resetBtn").value = t("reset_colors");
            document.getElementById("saveGammaBtn").value = t("save_gamma");
        });
    });
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

function hsvToHex(h, s, v) {
    h = h / 255 * 360;
    s = s / 255;
    v = v / 255;
    let c = v * s;
    let x = c * (1 - Math.abs((h / 60) % 2 - 1));
    let m = v - c;
    let r, g, b;
    if (h < 60) { r = c; g = x; b = 0; }
    else if (h < 120) { r = x; g = c; b = 0; }
    else if (h < 180) { r = 0; g = c; b = x; }
    else if (h < 240) { r = 0; g = x; b = c; }
    else if (h < 300) { r = x; g = 0; b = c; }
    else { r = c; g = 0; b = x; }
    r = Math.round((r + m) * 255);
    g = Math.round((g + m) * 255);
    b = Math.round((b + m) * 255);
    return "#" + ((1 << 24) + (r << 16) + (g << 8) + b).toString(16).slice(1);
}

function hexToHsv(hex) {
    let r = parseInt(hex.slice(1, 3), 16) / 255;
    let g = parseInt(hex.slice(3, 5), 16) / 255;
    let b = parseInt(hex.slice(5, 7), 16) / 255;
    let max = Math.max(r, g, b), min = Math.min(r, g, b);
    let h, s, v = max;
    let d = max - min;
    s = max == 0 ? 0 : d / max;
    if (max == min) { h = 0; }
    else {
        switch (max) {
            case r: h = (g - b) / d + (g < b ? 6 : 0); break;
            case g: h = (b - r) / d + 2; break;
            case b: h = (r - g) / d + 4; break;
        }
        h /= 6;
    }
    return { h: Math.round(h * 255), s: Math.round(s * 255), v: Math.round(v * 255) };
}

function loadColors() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/admin/colorvalues", true);
    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
            processColors(xhr.responseText);
        }
    };
    xhr.send();
}

function processColors(res) {
    var data = JSON.parse(res);
    document.getElementById("enabled").checked = (data.enabled == "1");
    document.getElementById("weekday").value = hsvToHex(parseInt(data.weekday_h), parseInt(data.weekday_s), parseInt(data.weekday_v));
    document.getElementById("weekend").value = hsvToHex(parseInt(data.weekend_h), parseInt(data.weekend_s), parseInt(data.weekend_v));
    document.getElementById("today").value = hsvToHex(parseInt(data.today_h), parseInt(data.today_s), parseInt(data.today_v));
    document.getElementById("month").value = hsvToHex(parseInt(data.month_h), parseInt(data.month_s), parseInt(data.month_v));
    document.getElementById("holiday").value = hsvToHex(parseInt(data.holiday_h), parseInt(data.holiday_s), parseInt(data.holiday_v));
    document.getElementById("anniversary").value = hsvToHex(parseInt(data.anniversary_h), parseInt(data.anniversary_s), parseInt(data.anniversary_v));
    document.getElementById("todo").value = hsvToHex(parseInt(data.todo_h), parseInt(data.todo_s), parseInt(data.todo_v));
    toggleColors();
}

function toggleColors() {
    var enabled = document.getElementById("enabled").checked;
    var inputs = document.querySelectorAll("input[type=color]");
    for (var i = 0; i < inputs.length; i++) {
        inputs[i].disabled = !enabled;
    }
}

function previewColors() {
    var params = buildColorParams();
    microAjax("/admin/previewcolors?" + params, function(res) {});
}

function saveColors() {
    var params = buildColorParams();
    microAjax("/admin/savecolors?" + params, function(res) {
        alert("Colors saved!");
    });
}

function resetColors() {
    document.getElementById("enabled").checked = false;
    document.getElementById("weekday").value = "#00ff00";
    document.getElementById("weekend").value = "#ff0000";
    document.getElementById("today").value = "#0000ff";
    document.getElementById("month").value = "#c0c0c0";
    document.getElementById("holiday").value = "#ff00ff";
    document.getElementById("anniversary").value = "#00ffff";
    document.getElementById("todo").value = "#ffa500";
    toggleColors();
    microAjax("/admin/resetcolors", function(res) {
        alert("Colors reset to defaults!");
    });
}

function buildColorParams() {
    var enabled = document.getElementById("enabled").checked ? 1 : 0;
    var weekday = hexToHsv(document.getElementById("weekday").value);
    var weekend = hexToHsv(document.getElementById("weekend").value);
    var today = hexToHsv(document.getElementById("today").value);
    var month = hexToHsv(document.getElementById("month").value);
    var holiday = hexToHsv(document.getElementById("holiday").value);
    var anniversary = hexToHsv(document.getElementById("anniversary").value);
    var todo = hexToHsv(document.getElementById("todo").value);

    return "enabled=" + enabled +
        "&wdh=" + weekday.h + "&wds=" + weekday.s + "&wdv=" + weekday.v +
        "&weh=" + weekend.h + "&wes=" + weekend.s + "&wev=" + weekend.v +
        "&tdh=" + today.h + "&tds=" + today.s + "&tdv=" + today.v +
        "&moh=" + month.h + "&mos=" + month.s + "&mov=" + month.v +
        "&hoh=" + holiday.h + "&hos=" + holiday.s + "&hov=" + holiday.v +
        "&anh=" + anniversary.h + "&ans=" + anniversary.s + "&anv=" + anniversary.v +
        "&toh=" + todo.h + "&tos=" + todo.s + "&tov=" + todo.v;
}

function loadGamma() {
    microAjax("/admin/gammavalues", function(res) {
        var data = JSON.parse(res);
        document.getElementById("correction").value = data.correction;
        document.getElementById("temperature").value = data.temperature;
    });
}

function previewGamma() {
    var corr = document.getElementById("correction").value;
    var temp = document.getElementById("temperature").value;
    microAjax("/admin/previewgamma?corr=" + corr + "&temp=" + temp, function(res) {});
}

function saveGamma() {
    var corr = document.getElementById("correction").value;
    var temp = document.getElementById("temperature").value;
    microAjax("/admin/savegamma?corr=" + corr + "&temp=" + temp, function(res) {
        alert("Gamma saved!");
    });
}
</script>
)=====";

void send_Color_Settings_html() {
    server.send_P(200, "text/html", PAGE_ColorSettings);
}

void send_Color_Settings_values_html() {
    String response = "{";
    response += "\"enabled\":\"" + String(customColors.enabled ? 1 : 0) + "\",";
    response += "\"weekday_h\":\"" + String(customColors.weekday.h) + "\",";
    response += "\"weekday_s\":\"" + String(customColors.weekday.s) + "\",";
    response += "\"weekday_v\":\"" + String(customColors.weekday.v) + "\",";
    response += "\"weekend_h\":\"" + String(customColors.weekend.h) + "\",";
    response += "\"weekend_s\":\"" + String(customColors.weekend.s) + "\",";
    response += "\"weekend_v\":\"" + String(customColors.weekend.v) + "\",";
    response += "\"today_h\":\"" + String(customColors.today.h) + "\",";
    response += "\"today_s\":\"" + String(customColors.today.s) + "\",";
    response += "\"today_v\":\"" + String(customColors.today.v) + "\",";
    response += "\"month_h\":\"" + String(customColors.month.h) + "\",";
    response += "\"month_s\":\"" + String(customColors.month.s) + "\",";
    response += "\"month_v\":\"" + String(customColors.month.v) + "\",";
    response += "\"holiday_h\":\"" + String(customColors.holiday.h) + "\",";
    response += "\"holiday_s\":\"" + String(customColors.holiday.s) + "\",";
    response += "\"holiday_v\":\"" + String(customColors.holiday.v) + "\",";
    response += "\"anniversary_h\":\"" + String(customColors.anniversary.h) + "\",";
    response += "\"anniversary_s\":\"" + String(customColors.anniversary.s) + "\",";
    response += "\"anniversary_v\":\"" + String(customColors.anniversary.v) + "\",";
    response += "\"todo_h\":\"" + String(customColors.todo.h) + "\",";
    response += "\"todo_s\":\"" + String(customColors.todo.s) + "\",";
    response += "\"todo_v\":\"" + String(customColors.todo.v) + "\"";
    response += "}";
    server.send(200, "application/json", response);
}

void applyColorsFromArgs() {
    if (server.hasArg("enabled")) {
        customColors.enabled = server.arg("enabled").toInt() == 1;
    }
    if (server.hasArg("wdh")) {
        weekday_color = CHSV(server.arg("wdh").toInt(), server.arg("wds").toInt(), server.arg("wdv").toInt());
        customColors.weekday = weekday_color;
    }
    if (server.hasArg("weh")) {
        weekend_color = CHSV(server.arg("weh").toInt(), server.arg("wes").toInt(), server.arg("wev").toInt());
        customColors.weekend = weekend_color;
    }
    if (server.hasArg("tdh")) {
        actualday_color = CHSV(server.arg("tdh").toInt(), server.arg("tds").toInt(), server.arg("tdv").toInt());
        customColors.today = actualday_color;
    }
    if (server.hasArg("moh")) {
        month_color = CHSV(server.arg("moh").toInt(), server.arg("mos").toInt(), server.arg("mov").toInt());
        customColors.month = month_color;
    }
    if (server.hasArg("hoh")) {
        holidays_color = CHSV(server.arg("hoh").toInt(), server.arg("hos").toInt(), server.arg("hov").toInt());
        customColors.holiday = holidays_color;
    }
    if (server.hasArg("anh")) {
        anniversaries_color = CHSV(server.arg("anh").toInt(), server.arg("ans").toInt(), server.arg("anv").toInt());
        customColors.anniversary = anniversaries_color;
    }
    if (server.hasArg("toh")) {
        todos_color = CHSV(server.arg("toh").toInt(), server.arg("tos").toInt(), server.arg("tov").toInt());
        customColors.todo = todos_color;
    }
}

void handle_preview_colors() {
    applyColorsFromArgs();
    // Colors applied, display updates on next loop iteration
    server.send(200, "text/plain", "OK");
}

void handle_save_colors() {
    applyColorsFromArgs();
    saveCustomColors();
    server.send(200, "text/plain", "OK");
}

void handle_reset_colors() {
    // Desactivar colores personalizados
    customColors.enabled = false;

    // Restaurar defaults en el struct
    customColors.weekday = CHSV(96, 255, 192);
    customColors.weekend = CHSV(0, 255, 128);
    customColors.today = CHSV(160, 255, 192);
    customColors.holiday = CHSV(192, 255, 255);
    customColors.anniversary = CHSV(128, 255, 255);
    customColors.todo = CHSV(32, 255, 192);
    customColors.month = CHSV(0, 0, 192);

    // Aplicar a colores activos
    weekday_color = customColors.weekday;
    weekend_color = customColors.weekend;
    actualday_color = customColors.today;
    holidays_color = customColors.holiday;
    anniversaries_color = customColors.anniversary;
    todos_color = customColors.todo;
    month_color = customColors.month;

    // Guardar en EEPROM
    saveCustomColors();
    server.send(200, "text/plain", "OK");
}

void send_gamma_values_html() {
    String response = "{";
    response += "\"correction\":\"" + String(colorCorrectionMode) + "\",";
    response += "\"temperature\":\"" + String(colorTemperatureMode) + "\"";
    response += "}";
    server.send(200, "application/json", response);
}

void handle_preview_gamma() {
    if (server.hasArg("corr")) {
        colorCorrectionMode = server.arg("corr").toInt();
    }
    if (server.hasArg("temp")) {
        colorTemperatureMode = server.arg("temp").toInt();
    }
    applyColorCorrection();
    temp_hour = -1;
    server.send(200, "text/plain", "OK");
}

void handle_save_gamma() {
    if (server.hasArg("corr")) {
        colorCorrectionMode = server.arg("corr").toInt();
    }
    if (server.hasArg("temp")) {
        colorTemperatureMode = server.arg("temp").toInt();
    }
    applyColorCorrection();
    saveColorCorrection();
    temp_hour = -1;
    server.send(200, "text/plain", "OK");
}

#endif
