#ifndef PAGE_OTA_H
#define PAGE_OTA_H

const char PAGE_OTA[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<body>
<div style="display:flex;justify-content:space-between;align-items:center">
<strong data-i18n="ota_update">Actualizacion OTA</strong>
<button id="langBtn" onclick="toggleLang()" class="btn btn--s btn--blue">EN</button>
</div>
<hr>
<p data-i18n="ota_enabled">Actualizaciones OTA habilitadas.</p>
<table style="width:310px">
<tr><td style="width:150px">Hostname:</td><td><span id="hostname">-</span></td></tr>
<tr><td data-i18n="ip_address">Direccion IP:</td><td><span id="ip">-</span></td></tr>
<tr><td data-i18n="port">Puerto:</td><td>8266</td></tr>
<tr><td data-i18n="status">Estado:</td><td><span id="status" style="color:green" data-i18n="ready">Listo</span></td></tr>
</table>
<hr>
<strong data-i18n="instructions">Instrucciones</strong>
<ol>
<li data-i18n="ota_step1">Abrir Arduino IDE o PlatformIO</li>
<li data-i18n="ota_step2">Ir a Tools > Port</li>
<li data-i18n="ota_step3">Seleccionar el puerto de red</li>
<li data-i18n="ota_step4">Subir el sketch normalmente</li>
</ol>
<hr>
<a href="/" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="back">Volver</span></a>
</body>
<script>
window.onload = function() {
    load("style.css","css", function() {
        load("microajax.js","js", function() {
            initLang();
            loadOTAInfo();
        });
    });
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

function loadOTAInfo() {
    microAjax("/admin/otavalues", function(res) {
        var data = JSON.parse(res);
        document.getElementById("hostname").innerHTML = data.hostname;
        document.getElementById("ip").innerHTML = data.ip;
    });
}
</script>
)=====";

void send_OTA_html() {
    server.send_P(200, "text/html", PAGE_OTA);
}

void send_OTA_values_html() {
    String response = "{\"hostname\":\"" + config.DeviceName + ".local\",\"ip\":\"" + WiFi.localIP().toString() + "\"}";
    server.send(200, "application/json", response);
}

#endif
