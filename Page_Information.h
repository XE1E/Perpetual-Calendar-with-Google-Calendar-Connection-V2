#ifndef PAGE_INFOMATION_H
#define PAGE_INFOMATION_H

const char PAGE_Information[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>Informacion</title>
<link rel="stylesheet" href="style.css" type="text/css" />
<script src="microajax.js"></script>
<div style="display:flex;justify-content:space-between;align-items:center">
<strong data-i18n="system_info">Informacion del Sistema</strong>
<button id="langBtn" onclick="toggleLang()" class="btn btn--s btn--blue">EN</button>
</div>
<hr>
<strong data-i18n="network_info">Red</strong>
<table border="0" cellspacing="0" cellpadding="3" style="width:310px">
<tr><td align="right">SSID:</td><td><span id="x_ssid"></span></td></tr>
<tr><td align="right">IP:</td><td><span id="x_ip"></span></td></tr>
<tr><td align="right" data-i18n="netmask">Mascara:</td><td><span id="x_netmask"></span></td></tr>
<tr><td align="right" data-i18n="gateway">Gateway:</td><td><span id="x_gateway"></span></td></tr>
<tr><td align="right">MAC:</td><td><span id="x_mac"></span></td></tr>
</table>
<hr>
<strong data-i18n="resources">Recursos</strong>
<table border="0" cellspacing="0" cellpadding="3" style="width:310px">
<tr><td align="right">RAM Libre:</td><td><span id="x_heap"></span></td></tr>
<tr><td align="right">RAM Fragmentada:</td><td><span id="x_frag"></span></td></tr>
<tr><td align="right">Flash:</td><td><span id="x_flash"></span></td></tr>
<tr><td align="right">CPU:</td><td><span id="x_cpu"></span></td></tr>
<tr><td align="right">Uptime:</td><td><span id="x_uptime"></span></td></tr>
</table>
<hr>
<strong data-i18n="actions">Acciones</strong>
<table border="0" cellspacing="0" cellpadding="3" style="width:310px">
<tr><td>
<input type="button" style="width:150px" class="btn btn--m btn--blue" value="Reiniciar ESP" onclick="restartESP()">
</td></tr>
<tr><td style="padding-top:10px">
<input type="button" style="width:150px" class="btn btn--m btn--red" value="Borrar WiFi" onclick="clearWiFi()">
</td></tr>
</table>
<hr>
<a href="/" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="back">Volver</span></a>
<script>

function GetState() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/admin/infovalues", true);
    xhr.onreadystatechange = function() {
        if (xhr.readyState == 4 && xhr.status == 200) {
            var lines = xhr.responseText.split("\n");
            lines.forEach(function(line) {
                var fields = line.split("|");
                if (fields.length >= 2) {
                    var el = document.getElementById(fields[0]);
                    if (el) el.innerHTML = fields[1];
                }
            });
        }
    };
    xhr.send();
}

function restartESP() {
    if (confirm("Reiniciar el dispositivo?")) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/admin/restart", true);
        xhr.send();
        alert("Reiniciando... La pagina se recargara en 10 segundos.");
        setTimeout(function() { location.reload(); }, 10000);
    }
}

function clearWiFi() {
    if (confirm("Borrar configuracion WiFi?\nEl dispositivo se reiniciara en modo AP.")) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/admin/clearwifi", true);
        xhr.send();
        alert("Configuracion WiFi borrada. Reiniciando...");
    }
}

window.onload = function() {
    load("style.css","css", function() {
        load("microajax.js","js", function() {
            initLang();
            GetState();
        });
    });
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

</script>
)=====" ;

void send_information_values_html() {
    String values = "";

    // Network info
    values += "x_ssid|" + (String)WiFi.SSID() + "|div\n";
    values += "x_ip|" + (String)WiFi.localIP()[0] + "." + (String)WiFi.localIP()[1] + "." + (String)WiFi.localIP()[2] + "." + (String)WiFi.localIP()[3] + "|div\n";
    values += "x_gateway|" + (String)WiFi.gatewayIP()[0] + "." + (String)WiFi.gatewayIP()[1] + "." + (String)WiFi.gatewayIP()[2] + "." + (String)WiFi.gatewayIP()[3] + "|div\n";
    values += "x_netmask|" + (String)WiFi.subnetMask()[0] + "." + (String)WiFi.subnetMask()[1] + "." + (String)WiFi.subnetMask()[2] + "." + (String)WiFi.subnetMask()[3] + "|div\n";
    values += "x_mac|" + GetMacAddress() + "|div\n";

    // Resource info
    values += "x_heap|" + String(ESP.getFreeHeap() / 1024.0, 1) + " KB|div\n";
    values += "x_frag|" + String(ESP.getHeapFragmentation()) + "%|div\n";
    values += "x_flash|" + String(ESP.getFlashChipSize() / 1024 / 1024) + " MB|div\n";
    values += "x_cpu|" + String(ESP.getCpuFreqMHz()) + " MHz|div\n";

    // Uptime
    unsigned long uptime = millis() / 1000;
    int days = uptime / 86400;
    int hours = (uptime % 86400) / 3600;
    int mins = (uptime % 3600) / 60;
    int secs = uptime % 60;
    String uptimeStr = "";
    if (days > 0) uptimeStr += String(days) + "d ";
    if (hours > 0 || days > 0) uptimeStr += String(hours) + "h ";
    uptimeStr += String(mins) + "m " + String(secs) + "s";
    values += "x_uptime|" + uptimeStr + "|div\n";

    server.send(200, "text/plain", values);
    AdminTimeOutCounter = 0;
}

void handle_restart() {
    server.send(200, "text/plain", "OK");
    delay(500);
    ESP.restart();
}

void handle_clear_wifi() {
    // Clear WiFi credentials from EEPROM
    WriteStringToEEPROM(64, "");   // Clear SSID
    WriteStringToEEPROM(96, "");   // Clear password
    WriteStringToEEPROM(520, "");  // Clear SSID2
    WriteStringToEEPROM(552, "");  // Clear password2
    EEPROM.commit();

    server.send(200, "text/plain", "OK");
    delay(500);
    ESP.restart();
}

#endif
