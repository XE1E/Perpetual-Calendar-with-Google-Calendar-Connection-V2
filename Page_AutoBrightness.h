#ifndef PAGE_AUTOBRIGHTNESS_H
#define PAGE_AUTOBRIGHTNESS_H

const char PAGE_AutoBrightness[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<body>
<div style="display:flex;justify-content:space-between;align-items:center">
<strong data-i18n="auto_brightness">Brillo Automatico</strong>
<button id="langBtn" onclick="toggleLang()" class="btn btn--s btn--blue">EN</button>
</div>
<hr>
<form id="autoBrightnessForm">
<table style="width:310px">
<tr><td colspan="2"><input type="checkbox" id="enabled" onchange="toggleFields()"> <span data-i18n="enable_auto">Activar brillo automatico</span></td></tr>
<tr><td colspan="2"><hr></td></tr>
<tr><td style="width:150px" data-i18n="day_brightness">Brillo de dia:</td><td><input type="range" id="dayBright" min="5" max="255" value="150" oninput="showVal('dayBright','dayVal')"><span id="dayVal">150</span></td></tr>
<tr><td data-i18n="night_brightness">Brillo de noche:</td><td><input type="range" id="nightBright" min="0" max="255" value="30" oninput="showNightVal()"><span id="nightVal">30</span></td></tr>
<tr><td colspan="2"><hr></td></tr>
<tr><td data-i18n="day_starts">Dia inicia a las:</td><td><select id="dayStartH"></select>:<select id="dayStartM"></select></td></tr>
<tr><td data-i18n="night_starts">Noche inicia a las:</td><td><select id="nightStartH"></select>:<select id="nightStartM"></select></td></tr>
<tr><td colspan="2" style="padding-top:10px">
<input type="button" id="saveBtn" style="width:150px" class="btn btn--m btn--grey" value="Guardar" onclick="saveSettings()">
</td></tr>
</table>
</form>
<hr>
<strong data-i18n="current_status">Estado Actual</strong>
<table style="width:310px">
<tr><td style="width:150px" data-i18n="current_hour">Hora actual:</td><td><span id="currentHour">-</span></td></tr>
<tr><td data-i18n="current_mode">Modo actual:</td><td><span id="currentMode">-</span></td></tr>
<tr><td data-i18n="current_bright">Brillo actual:</td><td><span id="currentBrightness">-</span></td></tr>
</table>
<hr>
<a href="/" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="back">Volver</span></a>
</body>
<script>
window.onload = function() {
    load("style.css","css", function() {
        load("microajax.js","js", function() {
            initLang();
            initSelects();
            loadSettings();
        });
    });
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

function initSelects() {
    var hSelects = [document.getElementById("dayStartH"), document.getElementById("nightStartH")];
    var mSelects = [document.getElementById("dayStartM"), document.getElementById("nightStartM")];
    for (var i = 0; i < 24; i++) {
        hSelects.forEach(function(s) {
            var opt = document.createElement("option");
            opt.value = i;
            opt.text = (i < 10 ? "0" : "") + i;
            s.appendChild(opt);
        });
    }
    for (var i = 0; i < 60; i += 5) {
        mSelects.forEach(function(s) {
            var opt = document.createElement("option");
            opt.value = i;
            opt.text = (i < 10 ? "0" : "") + i;
            s.appendChild(opt);
        });
    }
}

function showVal(inputId, spanId) {
    document.getElementById(spanId).innerHTML = document.getElementById(inputId).value;
}

function showNightVal() {
    var val = document.getElementById("nightBright").value;
    document.getElementById("nightVal").innerHTML = val == 0 ? "OFF" : val;
}

function toggleFields() {
    var enabled = document.getElementById("enabled").checked;
    document.getElementById("dayBright").disabled = !enabled;
    document.getElementById("nightBright").disabled = !enabled;
    document.getElementById("dayStartH").disabled = !enabled;
    document.getElementById("dayStartM").disabled = !enabled;
    document.getElementById("nightStartH").disabled = !enabled;
    document.getElementById("nightStartM").disabled = !enabled;
}

function loadSettings() {
    microAjax("/admin/autobrightvalues", function(res) {
        var data = JSON.parse(res);
        document.getElementById("enabled").checked = data.enabled == "1";
        document.getElementById("dayBright").value = data.dayBrightness;
        document.getElementById("dayVal").innerHTML = data.dayBrightness;
        document.getElementById("nightBright").value = data.nightBrightness;
        document.getElementById("nightVal").innerHTML = data.nightBrightness == 0 ? "OFF" : data.nightBrightness;
        document.getElementById("dayStartH").value = data.dayStartH;
        document.getElementById("dayStartM").value = data.dayStartM;
        document.getElementById("nightStartH").value = data.nightStartH;
        document.getElementById("nightStartM").value = data.nightStartM;
        var h = data.currentHour, m = data.currentMinute;
        document.getElementById("currentHour").innerHTML = (h<10?"0":"")+h+":"+(m<10?"0":"")+m;
        document.getElementById("currentMode").innerHTML = data.isDay == "1" ? t("day") : t("night");
        document.getElementById("currentBrightness").innerHTML = data.currentBrightness;
        document.getElementById("saveBtn").value = t("save");
        toggleFields();
    });
}

function saveSettings() {
    var enabled = document.getElementById("enabled").checked ? 1 : 0;
    var dayBright = document.getElementById("dayBright").value;
    var nightBright = document.getElementById("nightBright").value;
    var dayStartH = document.getElementById("dayStartH").value;
    var dayStartM = document.getElementById("dayStartM").value;
    var nightStartH = document.getElementById("nightStartH").value;
    var nightStartM = document.getElementById("nightStartM").value;

    var params = "enabled=" + enabled +
        "&dayBright=" + dayBright +
        "&nightBright=" + nightBright +
        "&dayStartH=" + dayStartH + "&dayStartM=" + dayStartM +
        "&nightStartH=" + nightStartH + "&nightStartM=" + nightStartM;

    microAjax("/admin/saveautobrightness?" + params, function(res) {
        alert("Settings saved!");
        loadSettings();
    });
}
</script>
)=====";

void send_AutoBrightness_html() {
    server.send_P(200, "text/html", PAGE_AutoBrightness);
}

void send_AutoBrightness_values_html() {
    uint16_t currentTime = DateTime.hour * 60 + DateTime.minute;
    uint16_t dayStart = autoBrightness.dayStartHour * 60 + autoBrightness.dayStartMinute;
    uint16_t nightStart = autoBrightness.nightStartHour * 60 + autoBrightness.nightStartMinute;
    bool isDay = (dayStart < nightStart)
        ? (currentTime >= dayStart && currentTime < nightStart)
        : (currentTime >= dayStart || currentTime < nightStart);

    String response = "{";
    response += "\"enabled\":\"" + String(autoBrightness.enabled ? 1 : 0) + "\",";
    response += "\"dayBrightness\":\"" + String(autoBrightness.dayBrightness) + "\",";
    response += "\"nightBrightness\":\"" + String(autoBrightness.nightBrightness) + "\",";
    response += "\"dayStartH\":\"" + String(autoBrightness.dayStartHour) + "\",";
    response += "\"dayStartM\":\"" + String(autoBrightness.dayStartMinute) + "\",";
    response += "\"nightStartH\":\"" + String(autoBrightness.nightStartHour) + "\",";
    response += "\"nightStartM\":\"" + String(autoBrightness.nightStartMinute) + "\",";
    response += "\"currentHour\":\"" + String(DateTime.hour) + "\",";
    response += "\"currentMinute\":\"" + String(DateTime.minute) + "\",";
    response += "\"isDay\":\"" + String(isDay ? 1 : 0) + "\",";
    response += "\"currentBrightness\":\"" + String(BRIGHTNESS) + "\"";
    response += "}";
    server.send(200, "application/json", response);
}

void handle_save_autobrightness() {
    if (server.hasArg("enabled")) {
        autoBrightness.enabled = server.arg("enabled").toInt() == 1;
    }
    if (server.hasArg("dayBright")) {
        autoBrightness.dayBrightness = server.arg("dayBright").toInt();
    }
    if (server.hasArg("nightBright")) {
        autoBrightness.nightBrightness = server.arg("nightBright").toInt();
    }
    if (server.hasArg("dayStartH")) {
        autoBrightness.dayStartHour = server.arg("dayStartH").toInt();
    }
    if (server.hasArg("dayStartM")) {
        autoBrightness.dayStartMinute = server.arg("dayStartM").toInt();
    }
    if (server.hasArg("nightStartH")) {
        autoBrightness.nightStartHour = server.arg("nightStartH").toInt();
    }
    if (server.hasArg("nightStartM")) {
        autoBrightness.nightStartMinute = server.arg("nightStartM").toInt();
    }

    saveAutoBrightnessConfig();

    // Apply immediately
    updateAutoBrightness(DateTime.hour, DateTime.minute);
    FastLED.show();

    server.send(200, "text/plain", "OK");
}

#endif
