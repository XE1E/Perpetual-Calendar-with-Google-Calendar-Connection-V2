#ifndef PAGE_COLORSETTINGS_H
#define PAGE_COLORSETTINGS_H

// EEPROM addresses for custom colors (494-511)
#define EEPROM_CUSTOM_COLORS_ENABLED 494
#define EEPROM_COLOR_WEEKDAY 495      // 3 bytes (H, S, V)
#define EEPROM_COLOR_WEEKEND 498      // 3 bytes
#define EEPROM_COLOR_TODAY 501        // 3 bytes
#define EEPROM_COLOR_HOLIDAY 504      // 3 bytes
#define EEPROM_COLOR_ANNIVERSARY 507  // 3 bytes
#define EEPROM_COLOR_TODO 510         // 3 bytes (uses 510, 511, and we need one more)

struct CustomColors {
    bool enabled;
    CHSV weekday;
    CHSV weekend;
    CHSV today;
    CHSV holiday;
    CHSV anniversary;
    CHSV todo;
} customColors;

void loadCustomColors() {
    customColors.enabled = EEPROM.read(EEPROM_CUSTOM_COLORS_ENABLED);

    if (customColors.enabled == 1) {
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
                                  192); // Default value for V since we're out of EEPROM space

        // Apply custom colors
        weekday_color = customColors.weekday;
        weekend_color = customColors.weekend;
        actualday_color = customColors.today;
        holidays_color = customColors.holiday;
        anniversaries_color = customColors.anniversary;
        todos_color = customColors.todo;
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
<tr><td data-i18n="holiday_color">Festivos:</td><td><input type="color" id="holiday" value="#ff00ff"></td></tr>
<tr><td data-i18n="anniversary_color">Aniversarios:</td><td><input type="color" id="anniversary" value="#00ffff"></td></tr>
<tr><td data-i18n="todo_color">Tareas:</td><td><input type="color" id="todo" value="#ffa500"></td></tr>
<tr><td colspan="2" style="padding-top:10px">
<input type="button" id="previewBtn" style="width:120px" class="btn btn--m btn--grey" value="Vista Previa" onclick="previewColors()">
<input type="button" id="saveBtn" style="width:100px" class="btn btn--m btn--grey" value="Guardar" onclick="saveColors()">
</td></tr>
<tr><td colspan="2">
<input type="button" id="resetBtn" style="width:210px" class="btn btn--s btn--grey" value="Restablecer" onclick="resetColors()">
</td></tr>
</table>
</form>
<hr>
<a href="/" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="back">Volver</span></a>
</body>
<script>
window.onload = function() {
    load("style.css","css", function() {
        load("microajax.js","js", function() {
            initLang();
            loadColors();
            document.getElementById("previewBtn").value = t("preview");
            document.getElementById("saveBtn").value = t("save");
            document.getElementById("resetBtn").value = t("reset_colors");
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
    microAjax("/admin/colorvalues", function(res) {
        var data = JSON.parse(res);
        document.getElementById("enabled").checked = data.enabled == "1";
        document.getElementById("weekday").value = hsvToHex(parseInt(data.weekday_h), parseInt(data.weekday_s), parseInt(data.weekday_v));
        document.getElementById("weekend").value = hsvToHex(parseInt(data.weekend_h), parseInt(data.weekend_s), parseInt(data.weekend_v));
        document.getElementById("today").value = hsvToHex(parseInt(data.today_h), parseInt(data.today_s), parseInt(data.today_v));
        document.getElementById("holiday").value = hsvToHex(parseInt(data.holiday_h), parseInt(data.holiday_s), parseInt(data.holiday_v));
        document.getElementById("anniversary").value = hsvToHex(parseInt(data.anniversary_h), parseInt(data.anniversary_s), parseInt(data.anniversary_v));
        document.getElementById("todo").value = hsvToHex(parseInt(data.todo_h), parseInt(data.todo_s), parseInt(data.todo_v));
        toggleColors();
    });
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
    var holiday = hexToHsv(document.getElementById("holiday").value);
    var anniversary = hexToHsv(document.getElementById("anniversary").value);
    var todo = hexToHsv(document.getElementById("todo").value);

    return "enabled=" + enabled +
        "&wdh=" + weekday.h + "&wds=" + weekday.s + "&wdv=" + weekday.v +
        "&weh=" + weekend.h + "&wes=" + weekend.s + "&wev=" + weekend.v +
        "&tdh=" + today.h + "&tds=" + today.s + "&tdv=" + today.v +
        "&hoh=" + holiday.h + "&hos=" + holiday.s + "&hov=" + holiday.v +
        "&anh=" + anniversary.h + "&ans=" + anniversary.s + "&anv=" + anniversary.v +
        "&toh=" + todo.h + "&tos=" + todo.s + "&tov=" + todo.v;
}
</script>
)=====";

void send_Color_Settings_html() {
    server.send_P(200, "text/html", PAGE_ColorSettings);
}

void send_Color_Settings_values_html() {
    String response = "{";
    response += "\"enabled\":\"" + String(customColors.enabled ? 1 : 0) + "\",";
    response += "\"weekday_h\":\"" + String(weekday_color.h) + "\",";
    response += "\"weekday_s\":\"" + String(weekday_color.s) + "\",";
    response += "\"weekday_v\":\"" + String(weekday_color.v) + "\",";
    response += "\"weekend_h\":\"" + String(weekend_color.h) + "\",";
    response += "\"weekend_s\":\"" + String(weekend_color.s) + "\",";
    response += "\"weekend_v\":\"" + String(weekend_color.v) + "\",";
    response += "\"today_h\":\"" + String(actualday_color.h) + "\",";
    response += "\"today_s\":\"" + String(actualday_color.s) + "\",";
    response += "\"today_v\":\"" + String(actualday_color.v) + "\",";
    response += "\"holiday_h\":\"" + String(holidays_color.h) + "\",";
    response += "\"holiday_s\":\"" + String(holidays_color.s) + "\",";
    response += "\"holiday_v\":\"" + String(holidays_color.v) + "\",";
    response += "\"anniversary_h\":\"" + String(anniversaries_color.h) + "\",";
    response += "\"anniversary_s\":\"" + String(anniversaries_color.s) + "\",";
    response += "\"anniversary_v\":\"" + String(anniversaries_color.v) + "\",";
    response += "\"todo_h\":\"" + String(todos_color.h) + "\",";
    response += "\"todo_s\":\"" + String(todos_color.s) + "\",";
    response += "\"todo_v\":\"" + String(todos_color.v) + "\"";
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
    // Force calendar redraw
    temp_hour = -1;
    server.send(200, "text/plain", "OK");
}

void handle_save_colors() {
    applyColorsFromArgs();
    saveCustomColors();
    temp_hour = -1;
    server.send(200, "text/plain", "OK");
}

void handle_reset_colors() {
    customColors.enabled = false;
    weekday_color = rainbow_colors[4];
    weekend_color = rainbow_colors[1];
    actualday_color = rainbow_colors[6];
    holidays_color = rainbow_colors[7];
    anniversaries_color = rainbow_colors[5];
    todos_color = rainbow_colors[2];
    saveCustomColors();
    temp_hour = -1;
    server.send(200, "text/plain", "OK");
}

#endif
