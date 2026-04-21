#ifndef PAGE_AUTOBRIGHTNESS_H
#define PAGE_AUTOBRIGHTNESS_H

const char PAGE_AutoBrightness[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<body>
<strong>Auto Brightness Settings</strong>
<hr>
<form id="autoBrightnessForm">
<table style="width:310px">
<tr><td colspan="2"><input type="checkbox" id="enabled" onchange="toggleFields()"> Enable auto brightness</td></tr>
<tr><td colspan="2"><hr></td></tr>
<tr><td style="width:150px">Day brightness:</td><td><input type="range" id="dayBright" min="10" max="255" value="150" oninput="showVal('dayBright','dayVal')"><span id="dayVal">150</span></td></tr>
<tr><td>Night brightness:</td><td><input type="range" id="nightBright" min="10" max="255" value="30" oninput="showVal('nightBright','nightVal')"><span id="nightVal">30</span></td></tr>
<tr><td colspan="2"><hr></td></tr>
<tr><td>Day starts at:</td><td><select id="dayStart"></select> hours</td></tr>
<tr><td>Night starts at:</td><td><select id="nightStart"></select> hours</td></tr>
<tr><td colspan="2" style="padding-top:10px">
<input type="button" style="width:150px" class="btn btn--m btn--grey" value="Save" onclick="saveSettings()">
</td></tr>
</table>
</form>
<hr>
<strong>Current Status</strong>
<table style="width:310px">
<tr><td style="width:150px">Current hour:</td><td><span id="currentHour">-</span></td></tr>
<tr><td>Current mode:</td><td><span id="currentMode">-</span></td></tr>
<tr><td>Current brightness:</td><td><span id="currentBrightness">-</span></td></tr>
</table>
<hr>
<a href="/" style="width:250px" class="btn btn--m btn--grey">Back</a>
</body>
<script>
window.onload = function() {
    load("style.css","css", function() {
        load("microajax.js","js", function() {
            initSelects();
            loadSettings();
        });
    });
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

function initSelects() {
    var daySelect = document.getElementById("dayStart");
    var nightSelect = document.getElementById("nightStart");
    for (var i = 0; i < 24; i++) {
        var opt1 = document.createElement("option");
        var opt2 = document.createElement("option");
        opt1.value = opt2.value = i;
        opt1.text = opt2.text = (i < 10 ? "0" : "") + i + ":00";
        daySelect.appendChild(opt1);
        nightSelect.appendChild(opt2);
    }
}

function showVal(inputId, spanId) {
    document.getElementById(spanId).innerHTML = document.getElementById(inputId).value;
}

function toggleFields() {
    var enabled = document.getElementById("enabled").checked;
    document.getElementById("dayBright").disabled = !enabled;
    document.getElementById("nightBright").disabled = !enabled;
    document.getElementById("dayStart").disabled = !enabled;
    document.getElementById("nightStart").disabled = !enabled;
}

function loadSettings() {
    microAjax("/admin/autobrightvalues", function(res) {
        var data = JSON.parse(res);
        document.getElementById("enabled").checked = data.enabled == "1";
        document.getElementById("dayBright").value = data.dayBrightness;
        document.getElementById("dayVal").innerHTML = data.dayBrightness;
        document.getElementById("nightBright").value = data.nightBrightness;
        document.getElementById("nightVal").innerHTML = data.nightBrightness;
        document.getElementById("dayStart").value = data.dayStart;
        document.getElementById("nightStart").value = data.nightStart;
        document.getElementById("currentHour").innerHTML = data.currentHour + ":00";
        document.getElementById("currentMode").innerHTML = data.isDay == "1" ? "Day" : "Night";
        document.getElementById("currentBrightness").innerHTML = data.currentBrightness;
        toggleFields();
    });
}

function saveSettings() {
    var enabled = document.getElementById("enabled").checked ? 1 : 0;
    var dayBright = document.getElementById("dayBright").value;
    var nightBright = document.getElementById("nightBright").value;
    var dayStart = document.getElementById("dayStart").value;
    var nightStart = document.getElementById("nightStart").value;

    var params = "enabled=" + enabled +
        "&dayBright=" + dayBright +
        "&nightBright=" + nightBright +
        "&dayStart=" + dayStart +
        "&nightStart=" + nightStart;

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
    bool isDay = false;
    if (autoBrightness.dayStartHour < autoBrightness.nightStartHour) {
        isDay = (DateTime.hour >= autoBrightness.dayStartHour &&
                 DateTime.hour < autoBrightness.nightStartHour);
    } else {
        isDay = (DateTime.hour >= autoBrightness.dayStartHour ||
                 DateTime.hour < autoBrightness.nightStartHour);
    }

    String response = "{";
    response += "\"enabled\":\"" + String(autoBrightness.enabled ? 1 : 0) + "\",";
    response += "\"dayBrightness\":\"" + String(autoBrightness.dayBrightness) + "\",";
    response += "\"nightBrightness\":\"" + String(autoBrightness.nightBrightness) + "\",";
    response += "\"dayStart\":\"" + String(autoBrightness.dayStartHour) + "\",";
    response += "\"nightStart\":\"" + String(autoBrightness.nightStartHour) + "\",";
    response += "\"currentHour\":\"" + String(DateTime.hour) + "\",";
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
    if (server.hasArg("dayStart")) {
        autoBrightness.dayStartHour = server.arg("dayStart").toInt();
    }
    if (server.hasArg("nightStart")) {
        autoBrightness.nightStartHour = server.arg("nightStart").toInt();
    }

    saveAutoBrightnessConfig();

    // Apply immediately
    updateAutoBrightness(DateTime.hour);
    FastLED.show();

    server.send(200, "text/plain", "OK");
}

#endif
