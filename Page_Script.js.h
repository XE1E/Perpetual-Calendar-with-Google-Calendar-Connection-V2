#ifndef PAGE_SCRIPT_JS_H
#define PAGE_SCRIPT_JS_H


const char PAGE_microajax_js[] PROGMEM = R"=====(
function microAjax(B,A){this.bindFunction=function(E,D){return function(){return E.apply(D,[D])}};this.stateChange=function(D){if(this.request.readyState==4){this.callbackFunction(this.request.responseText)}};this.getRequest=function(){if(window.ActiveXObject){return new ActiveXObject("Microsoft.XMLHTTP")}else{if(window.XMLHttpRequest){return new XMLHttpRequest()}}return false};this.postBody=(arguments[2]||"");this.callbackFunction=A;this.url=B;this.request=this.getRequest();if(this.request){var C=this.request;C.onreadystatechange=this.bindFunction(this.stateChange,this);if(this.postBody!==""){C.open("POST",B,true);C.setRequestHeader("X-Requested-With","XMLHttpRequest");C.setRequestHeader("Content-type","application/x-www-form-urlencoded");C.setRequestHeader("Connection","close")}else{C.open("GET",B,true)}C.send(this.postBody)}};

function setValues(url)
{
	microAjax(url, function (res)
	{
		res.split(String.fromCharCode(10)).forEach(function(entry) {
		fields = entry.split("|");
		if(fields[2] == "input")
		{
				document.getElementById(fields[0]).value = fields[1];
		}
		else if(fields[2] == "div")
		{
				document.getElementById(fields[0]).innerHTML  = fields[1];
		}
		else if(fields[2] == "chk")
		{
				document.getElementById(fields[0]).checked  = fields[1];
		}
	  });
	});
}

// Translation system
var i18n = {
	en: {
		// Main menu
		"title": "Perpetual Calendar",
		"network_config": "Network Configuration",
		"network_info": "Network Information",
		"ntp_settings": "NTP Settings",
		"apps_script": "Apps Script Settings",
		"manual_time": "Manual Time Setting",
		"led_settings": "LED Settings",
		"auto_brightness": "Auto Brightness",
		"color_settings": "Color Settings",
		"ota_update": "OTA Update",
		"back": "Back",
		"save": "Save",
		"refresh": "Refresh",
		// Network config
		"ssid": "SSID",
		"password": "Password",
		"dhcp": "DHCP",
		"ip": "IP Address",
		"netmask": "Netmask",
		"gateway": "Gateway",
		"connect": "Connect",
		"primary_network": "Primary Network:",
		"backup_network": "Backup Network (optional):",
		"ip_settings": "IP Settings:",
		"connection_state": "Connection State:",
		"networks": "Networks:",
		// LED settings
		"brightness": "Brightness",
		"test_leds": "Test LEDs",
		// Auto brightness
		"enable_auto": "Enable auto brightness",
		"day_brightness": "Day brightness",
		"night_brightness": "Night brightness",
		"day_starts": "Day starts at",
		"night_starts": "Night starts at",
		"hours": "hours",
		"current_status": "Current Status",
		"current_hour": "Current hour",
		"current_mode": "Current mode",
		"current_bright": "Current brightness",
		"day": "Day",
		"night": "Night",
		// NTP
		"ntp_server": "NTP Server",
		"timezone": "Timezone",
		"update_interval": "Update (min)",
		"daylight_saving": "Daylight Saving",
		"first_weekday": "First day of week",
		// Apps Script
		"holidays_id": "Holidays:",
		"anniversaries_id": "Anniversaries:",
		"todos_id": "To-Dos:",
		"deployment_ids": "Deployment IDs",
		// Time
		"year": "Year",
		"month": "Month",
		"day_label": "Day",
		"hour": "Hour",
		"minute": "Minute",
		"second": "Second",
		// Info
		"ssid_name": "SSID",
		"ip_address": "IP Address",
		"mac_address": "MAC Address",
		"device_name": "Device Name",
		// OTA
		"ota_enabled": "Over-The-Air updates enabled.",
		"port": "Port",
		"status": "Status",
		"ready": "Ready",
		"instructions": "Instructions",
		"ota_step1": "Open Arduino IDE or PlatformIO",
		"ota_step2": "Go to Tools > Port",
		"ota_step3": "Select the network port",
		"ota_step4": "Upload your sketch normally",
		// Colors
		"custom_colors": "Enable custom colors",
		"weekday_color": "Weekdays",
		"weekend_color": "Weekends",
		"today_color": "Today",
		"holiday_color": "Holidays",
		"anniversary_color": "Anniversaries",
		"todo_color": "To-Dos",
		"reset_colors": "Reset to Default",
		"preview": "Preview",
		"save_gamma": "Save Gamma",
		// System info
		"system_info": "System Information",
		"resources": "Resources",
		"actions": "Actions"
	},
	es: {
		// Menu principal
		"title": "Calendario Perpetuo",
		"network_config": "Configuracion de Red",
		"network_info": "Informacion de Red",
		"ntp_settings": "Configuracion NTP",
		"apps_script": "Apps Script Config",
		"manual_time": "Ajuste Manual de Hora",
		"led_settings": "Configuracion LEDs",
		"auto_brightness": "Brillo Automatico",
		"color_settings": "Configuracion Colores",
		"ota_update": "Actualizacion OTA",
		"back": "Volver",
		"save": "Guardar",
		"refresh": "Actualizar",
		// Config red
		"ssid": "SSID",
		"password": "Contrasena",
		"dhcp": "DHCP",
		"ip": "Direccion IP",
		"netmask": "Mascara",
		"gateway": "Puerta de Enlace",
		"connect": "Conectar",
		"primary_network": "Red Principal:",
		"backup_network": "Red de Respaldo (opcional):",
		"ip_settings": "Config IP:",
		"connection_state": "Estado Conexion:",
		"networks": "Redes:",
		// LED settings
		"brightness": "Brillo",
		"test_leds": "Probar LEDs",
		// Auto brightness
		"enable_auto": "Activar brillo automatico",
		"day_brightness": "Brillo de dia",
		"night_brightness": "Brillo de noche",
		"day_starts": "Dia inicia a las",
		"night_starts": "Noche inicia a las",
		"hours": "horas",
		"current_status": "Estado Actual",
		"current_hour": "Hora actual",
		"current_mode": "Modo actual",
		"current_bright": "Brillo actual",
		"day": "Dia",
		"night": "Noche",
		// NTP
		"ntp_server": "Servidor NTP",
		"timezone": "Zona Horaria",
		"update_interval": "Intervalo (min)",
		"daylight_saving": "Horario de Verano",
		"first_weekday": "Primer dia semana",
		// Apps Script
		"holidays_id": "Festivos:",
		"anniversaries_id": "Aniversarios:",
		"todos_id": "Tareas:",
		"deployment_ids": "IDs de Despliegue",
		// Time
		"year": "Ano",
		"month": "Mes",
		"day_label": "Dia",
		"hour": "Hora",
		"minute": "Minuto",
		"second": "Segundo",
		// Info
		"ssid_name": "SSID",
		"ip_address": "Direccion IP",
		"mac_address": "Direccion MAC",
		"device_name": "Nombre Dispositivo",
		// OTA
		"ota_enabled": "Actualizaciones OTA habilitadas.",
		"port": "Puerto",
		"status": "Estado",
		"ready": "Listo",
		"instructions": "Instrucciones",
		"ota_step1": "Abrir Arduino IDE o PlatformIO",
		"ota_step2": "Ir a Tools > Port",
		"ota_step3": "Seleccionar el puerto de red",
		"ota_step4": "Subir el sketch normalmente",
		// Colors
		"custom_colors": "Activar colores personalizados",
		"weekday_color": "Dias laborales",
		"weekend_color": "Fin de semana",
		"today_color": "Color dia actual",
		"holiday_color": "Color festivos",
		"anniversary_color": "Color aniversarios",
		"todo_color": "Color tareas",
		"reset_colors": "Restablecer",
		"preview": "Vista Previa",
		"save_gamma": "Guardar Gamma",
		// System info
		"system_info": "Informacion",
		"resources": "Recursos",
		"actions": "Acciones"
	}
};

function getLang() {
	return localStorage.getItem('lang') || 'es';
}

function setLang(lang) {
	localStorage.setItem('lang', lang);
	applyTranslations();
	updateLangBtn();
}

function toggleLang() {
	var current = getLang();
	setLang(current === 'en' ? 'es' : 'en');
}

function t(key) {
	var lang = getLang();
	return i18n[lang][key] || i18n['en'][key] || key;
}

function applyTranslations() {
	var lang = getLang();
	document.querySelectorAll('[data-i18n]').forEach(function(el) {
		var key = el.getAttribute('data-i18n');
		if (i18n[lang][key]) {
			el.textContent = i18n[lang][key];
		}
	});
}

function updateLangBtn() {
	var btn = document.getElementById('langBtn');
	if (btn) {
		btn.textContent = getLang() === 'es' ? 'EN' : 'ES';
	}
}

function initLang() {
	applyTranslations();
	updateLangBtn();
}

)=====";

#endif
