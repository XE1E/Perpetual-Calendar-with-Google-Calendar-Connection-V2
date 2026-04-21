#ifndef PAGE_OTA_H
#define PAGE_OTA_H

const char PAGE_OTA[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<body>
<strong>OTA Update</strong>
<hr>
<p>Over-The-Air updates are enabled.</p>
<table style="width:310px">
<tr><td style="width:150px">Hostname:</td><td><span id="hostname">-</span></td></tr>
<tr><td>IP Address:</td><td><span id="ip">-</span></td></tr>
<tr><td>Port:</td><td>8266</td></tr>
<tr><td>Status:</td><td><span id="status" style="color:green">Ready</span></td></tr>
</table>
<hr>
<strong>Instructions</strong>
<ol>
<li>Open Arduino IDE</li>
<li>Go to Tools > Port</li>
<li>Select the network port (calendario.local)</li>
<li>Upload your sketch normally</li>
</ol>
<hr>
<a href="/" style="width:250px" class="btn btn--m btn--grey">Back</a>
</body>
<script>
window.onload = function() {
    load("style.css","css", function() {
        load("microajax.js","js", function() {
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
