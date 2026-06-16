#ifndef PAGE_ADMIN_H
#define PAGE_ADMIN_H

//<a href="color.html"   style="width:250px"  class="btn btn--m btn--grey" >Colour settings</a><br>
//
//  HTML PAGE
//

const char PAGE_AdminMainPage[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<body>
<div style="display:flex;justify-content:space-between;align-items:center">
<strong data-i18n="title">Calendario Perpetuo</strong>
<button id="langBtn" onclick="toggleLang()" class="btn btn--s btn--blue">EN</button>
</div>
<hr>
<a href="config.html" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="network_config">Configuracion de Red</span></a><br>
<a href="info.html" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="network_info">Informacion de Red</span></a><br>
<a href="ntp.html" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="ntp_settings">Configuracion NTP</span></a><br>
<a href="appsscript.html" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="apps_script">Apps Script Config</span></a><br>
<a href="time.html" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="manual_time">Ajuste Manual de Hora</span></a><br>
<a href="led.html" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="led_settings">Configuracion LEDs</span></a><br>
<a href="autobrightness.html" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="auto_brightness">Brillo Automatico</span></a><br>
<a href="colors.html" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="color_settings">Configuracion Colores</span></a><br>
<a href="ota.html" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="ota_update">Actualizacion OTA</span></a><br>
</body>
<script>
window.onload = function ()
{
	load("style.css","css", function()
	{
		load("microajax.js","js", function()
		{
			initLang();
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

</script>

)=====";

#endif

