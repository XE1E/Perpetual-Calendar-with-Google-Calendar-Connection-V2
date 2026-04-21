#ifndef PAGE_LEDSETTINGS_H
#define PAGE_LEDSETTINGS_H

const char PAGE_LEDSettings[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<body>
<strong>LED Settings</strong>
<hr>
<form action="">
<table style="width:310px">
<tr><td style="width:150px">Brightness:</td><td><input type="range" id="brightness" name="brightness" min="10" max="255" value="100" oninput="updateBrightness(this.value)"><span id="brightnessVal">100</span></td></tr>
<tr><td colspan="2" style="padding-top:10px"><input type="button" style="width:150px" class="btn btn--m btn--grey" value="Save" onclick="saveBrightness()"></td></tr>
</table>
</form>
<hr>
<strong>Preview</strong>
<table style="width:310px">
<tr><td><input type="button" style="width:100px" class="btn btn--s btn--grey" value="Test LEDs" onclick="testLeds()"></td>
<td><input type="button" style="width:100px" class="btn btn--s btn--grey" value="Refresh" onclick="refreshCalendar()"></td></tr>
</table>
<hr>
<a href="/" style="width:250px" class="btn btn--m btn--grey">Back</a>
</body>
<script>
window.onload = function() {
    load("style.css","css", function() {
        load("microajax.js","js", function() {
            loadBrightness();
        });
    });
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

function loadBrightness() {
    microAjax("/admin/ledvalues", function(res) {
        var data = JSON.parse(res);
        document.getElementById("brightness").value = data.brightness;
        document.getElementById("brightnessVal").innerHTML = data.brightness;
    });
}

function updateBrightness(val) {
    document.getElementById("brightnessVal").innerHTML = val;
    microAjax("/admin/setbrightness?val=" + val, function(res) {});
}

function saveBrightness() {
    var val = document.getElementById("brightness").value;
    microAjax("/admin/savebrightness?val=" + val, function(res) {
        alert("Brightness saved!");
    });
}

function testLeds() {
    microAjax("/admin/testleds", function(res) {});
}

function refreshCalendar() {
    microAjax("/admin/refreshcalendar", function(res) {});
}
</script>
)=====";

void send_LED_Settings_html() {
    server.send_P(200, "text/html", PAGE_LEDSettings);
}

void send_LED_Settings_values_html() {
    String response = "{\"brightness\":\"" + String(BRIGHTNESS) + "\"}";
    server.send(200, "application/json", response);
}

void handle_set_brightness() {
    if (server.hasArg("val")) {
        BRIGHTNESS = server.arg("val").toInt();
        if (BRIGHTNESS < 10) BRIGHTNESS = 10;
        if (BRIGHTNESS > 255) BRIGHTNESS = 255;
        FastLED.setBrightness(BRIGHTNESS);
        FastLED.show();
    }
    server.send(200, "text/plain", "OK");
}

void handle_save_brightness() {
    if (server.hasArg("val")) {
        BRIGHTNESS = server.arg("val").toInt();
        if (BRIGHTNESS < 10) BRIGHTNESS = 10;
        if (BRIGHTNESS > 255) BRIGHTNESS = 255;
        EEPROM.write(488, BRIGHTNESS);
        EEPROM.commit();
        FastLED.setBrightness(BRIGHTNESS);
        FastLED.show();
    }
    server.send(200, "text/plain", "OK");
}

void handle_test_leds() {
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::White;
    }
    FastLED.show();
    delay(500);
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Black;
    }
    FastLED.show();
    server.send(200, "text/plain", "OK");
}

void handle_refresh_calendar() {
    temp_hour = -1;
    temp_minute = -1;
    server.send(200, "text/plain", "OK");
}

#endif
