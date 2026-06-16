#ifndef PAGE_APPSSCRIPTSETTINGS_H_
#define PAGE_APPSSCRIPTSETTINGS_H_

const char PAGE_AppsScriptSettings[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<div style="display:flex;justify-content:space-between;align-items:center">
<strong data-i18n="apps_script">Apps Script Config</strong>
<button id="langBtn" onclick="toggleLang()" class="btn btn--s btn--blue">EN</button>
</div>
<hr>
<form action="" method="get">
<table border="0" cellspacing="0" cellpadding="3">
<tr><td align="right" data-i18n="deployment_ids">IDs de Despliegue</td><td></td></tr>
<tr><td align="right" data-i18n="holidays_id">Festivos:</td><td><input type="text" id="hscriptid" name="hscriptid" value=""></td></tr>
<tr><td align="right" data-i18n="anniversaries_id">Aniversarios:</td><td><input type="text" id="ascriptid" name="ascriptid" value=""></td></tr>
<tr><td align="right" data-i18n="todos_id">Tareas:</td><td><input type="text" id="escriptid" name="escriptid" value=""></td></tr>
<tr><td colspan="2" align="center"><input type="submit" id="saveBtn" style="width:150px" class="btn btn--m btn--grey" value="Guardar"></td></tr>
</table>
</form>
<hr>
<a href="/" style="width:250px" class="btn btn--m btn--grey"><span data-i18n="back">Volver</span></a>
<script>

window.onload = function ()
{
  load("style.css","css", function()
  {
    load("microajax.js","js", function()
    {
        initLang();
        setValues("/admin/appsscript");
        document.getElementById("saveBtn").value = t("save");
    });
  });
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";


void send_Apps_Script_Settings_html()
{
  if (server.args() > 0 )  // Save Settings
  {
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "hscriptid") config.HolidaysScriptID =  server.arg(i);
      if (server.argName(i) == "ascriptid") config.AnniversariesScriptID =  server.arg(i);
      if (server.argName(i) == "escriptid") config.TodosScriptID =  server.arg(i);
    }
    WriteConfig();
  }
  server.send_P ( 200, "text/html", PAGE_AppsScriptSettings );
  Serial.println(__FUNCTION__);

}


void send_Apps_Script_Settings_values_html()
{
  String values ="";
  values += "hscriptid|" +  (String) config.HolidaysScriptID + "|input\n";
  values += "ascriptid|" +  (String) config.AnniversariesScriptID + "|input\n";
  values += "escriptid|" +  (String) config.TodosScriptID + "|input\n";
  server.send ( 200, "text/plain", values);
  Serial.println(__FUNCTION__);
  AdminTimeOutCounter=0;
}





#endif
