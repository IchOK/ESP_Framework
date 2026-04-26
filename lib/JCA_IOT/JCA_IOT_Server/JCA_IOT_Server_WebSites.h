/**
 * @file JCA_IOT_Webserver_Sites.h
 * @author JCA (https://github.com/ichok)
 * @brief Default Web Content for WebSite Frame
 * @version 0.1
 * @date 2022-09-04
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_IOT_SERVER_SITES_
#define _JCA_IOT_SERVER_SITES_

#include <Arduino.h>

/**
 * @brief Static Web Frame for Famework Sites
 */
const char PageFrame[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<title>%TITLE%</title>
<link rel="icon" type="image/svg+xml" href="/images/favicon.svg">
<link rel="icon" type="image/png" href="/images/favicon.png">
<link rel="stylesheet" type="text/css" href="/style/style.css">
<link rel="stylesheet" type="text/css" href="/style/styleMobile.css">
<link rel="stylesheet" type="text/css" href="/style/styleAddon.css">
<style>:root{--ColorHome:var(--secondary);--ColorConfig:var(--secondary);--ColorSetup:var(--secondary);--ColorWiFi:var(--secondary);--ColorSystem:var(--secondary)}</style>
<style>%STYLE%</style>
</head>
<body>
<nav class="container-fluid">
<ul>
<li>%SVG_LOGO%</li>
<li>%NAME%</li>
</ul><ul>
<li><a href="/home.htm">%SVG_HOME%</a></li>
<li><a href="/config.htm">%SVG_CONFIG%</a></li>
<li><a href="/setup.htm">%SVG_SETUP%</a></li>
<li><a href="/connect">%SVG_WIFI%</a></li>
<li><a href="/sys">%SVG_SYSTEM%</a></li>
</ul>
</nav>
<main class="container">
<section>
%SECTION%
</section>
</main>
</body>
</html>
)rawliteral";

/**
 * @brief static Web Content for WiFi Connect Site
 */
const char SectionConnect[] PROGMEM = R"rawliteral(
<article>
<form action="/connect" method="POST">
<label for="ssid">SSID<input type="text" id ="ssid" name="ssid" value="%SSID%"></label>
<label for="pass">Password<input type="password" id ="pass" name="pass"></label>
<label for="dhcp"><input type="checkbox" id ="dhcp" name="dhcp" %DHCP%>DHCP</label>
<label for="ip">IP Address<input type="text" id ="ip" name="ip" value="%IP%"></label>
<label for="gateway">Gateway Address<input type="text" id ="gateway" name="gateway" value="%GATEWAY%"></label>
<label for="subnet">Subnet Address<input type="text" id ="subnet" name="subnet" value="%SUBNET%"></label>
<button type ="submit">Connect</button>
</form>
</article>
)rawliteral";

/**
 * @brief  static Web Content for System Site
 */
const char SectionSys[] PROGMEM = R"rawliteral(
<article>
<header>System Information</header>
Firmware: %FW_VERSION%<br/>
Board Name: %BOARD_NAME%<br/>
Board Version: %BOARD_VERSION%<br/>
Board Variant: %BOARD_VARIANT%<br/>
Board MCU: %BOARD_MCU%<br/>
%SYSTEMCONFIG_LINK%
%WIFICONFIG_LINK%
<a href="/api/setup">Function Setup</a><br/>
<a href="/api/tags">Function-Tag Listing</a><br/>
%VALUES_LINK%
%LOG_LINK%
</article>
<article>
<header>Systemkonfiguration</header>
<div name="hostname" style="align-items:baseline ;" class="grid">
    <p id="name">Hostname</p>
    <div style="display:flex;align-items:baseline;">
        <input id="sysCfg_hostname" name="value" style="padding-right:54px;text-align:right;" type="text">
    </div>
</div>
<div name="port" style="align-items:baseline ;" class="grid">
    <p id="name">Web-Port</p>
    <div style="display:flex;align-items:baseline;">
        <input id="sysCfg_port" name="value" style="padding-right:54px;text-align:right;" step="1" type="number" min="1" max="65535">
    </div>
</div>
<div name="udtPort" style="align-items:baseline ;" class="grid">
    <p id="name">UDP-Port</p>
    <div style="display:flex;align-items:baseline;">
        <input id="sysCfg_udtPort" name="value" style="padding-right:54px;text-align:right;" step="1" type="number" min="0" max="65535">
    </div>
</div>
<div name="localTimeZone" style="align-items:baseline ;" class="grid">
    <p id="name">Zeitzonen-Offset</p>
    <div style="display:flex;align-items:baseline;">
        <input id="sysCfg_localTimeZone" name="value" style="padding-right:54px;text-align:right;" step="1" type="number" min="0" max="86400">
        <span name="unit" style="margin-left:-50px;text-align:left;">s</span>
    </div>
</div>
<div name="wsUpdate" style="align-items:baseline ;" class="grid">
    <p id="name">WebSocket-Update-Intervall</p>
    <div style="display:flex;align-items:baseline;">
        <input id="sysCfg_wsUpdate" name="value" style="padding-right:54px;text-align:right;" step="1" type="number" min="0">
        <span name="unit" style="margin-left:-50px;text-align:left;">ms</span>
    </div>
</div>
<div name="dayLightSaving" style="align-items:baseline ;" class="grid">
    <p id="name">Sommerzeit</p>
    <div style="display:flex;align-items:baseline;">
        <input id="sysCfg_dayLightSaving" name="value" type="button" class="primary outline" data-dls="0" value="AUS" title="Klick: umschalten" />
    </div>
</div>
<div name="rebootCounter" style="align-items:baseline ;" class="grid">
    <p id="name">Reboot-Zähler</p>
    <div style="display:flex;align-items:baseline;">
        <input id="sysCfg_rebootCounter" name="value" style="padding-right:54px;text-align:right;" step="any" type="number" readonly>
    </div>
</div>
<button type="button" id="sysConfigSave" class="primary">Speichern</button>
<script>
(function () {
  function el(id) { return document.getElementById(id); }
  function showMsg(t, isErr) {
    var m = el("sysConfigMsg");
    m.style.display = "block";
    m.textContent = t;
    m.style.color = isErr ? "crimson" : "";
  }
  function setDayLS(on) {
    var b = el("sysCfg_dayLightSaving");
    if (!b) { return; }
    b.setAttribute("data-dls", on ? "1" : "0");
    b.value = on ? "EIN" : "AUS";
    b.className = on ? "primary" : "primary outline";
  }
  function applyJson(j) {
    if (!el("sysCfg_hostname")) { return; }
    el("sysCfg_hostname").value = (j && j.hostname != null) ? j.hostname : "";
    el("sysCfg_port").value = (j && j.port != null) ? j.port : 80;
    el("sysCfg_udtPort").value = (j && j.udtPort != null) ? j.udtPort : 0;
    el("sysCfg_localTimeZone").value = (j && j.localTimeZone != null) ? j.localTimeZone : 0;
    el("sysCfg_wsUpdate").value = (j && j.wsUpdate != null) ? j.wsUpdate : 0;
    setDayLS(!!(j && j.dayLightSaving));
    el("sysCfg_rebootCounter").value = (j && j.rebootCounter != null) ? j.rebootCounter : 0;
  }
  function loadCfg() {
    fetch("/api/sysConfig", { credentials: "same-origin", cache: "no-store" })
      .then(function (r) {
        if (!r.ok) throw new Error("HTTP " + r.status);
        return r.json();
      })
      .then(applyJson)
      .catch(function (e) { showMsg("Laden: " + e, true); });
  }
  function saveCfg() {
    var body = {
      hostname: el("sysCfg_hostname").value,
      port: parseInt(el("sysCfg_port").value, 10) || 0,
      udtPort: parseInt(el("sysCfg_udtPort").value, 10) || 0,
      localTimeZone: parseInt(el("sysCfg_localTimeZone").value, 10) || 0,
      wsUpdate: parseInt(el("sysCfg_wsUpdate").value, 10) || 0,
      dayLightSaving: el("sysCfg_dayLightSaving").getAttribute("data-dls") === "1",
      rebootCounter: parseInt(el("sysCfg_rebootCounter").value, 10) || 0
    };
    fetch("/api/sysConfig", {
      method: "PUT",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(body),
      credentials: "same-origin"
    })
      .then(function (r) { return r.json().then(function (j) { return { ok: r.ok, j: j }; }); })
      .then(function (x) {
        if (x.j && x.j.error) { showMsg(x.j.error, true); return; }
        showMsg("Gespeichert.", false);
        loadCfg();
      })
      .catch(function (e) { showMsg("Speichern: " + e, true); });
  }
  function boot() {
    if (el("sysConfigSave")) { el("sysConfigSave").addEventListener("click", saveCfg); }
    var dls = el("sysCfg_dayLightSaving");
    if (dls && dls.getAttribute("data-dls") !== null) {
      dls.addEventListener("click", function () {
        var on = dls.getAttribute("data-dls") !== "1";
        setDayLS(on);
      });
    }
    loadCfg();
  }
  if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", boot);
  } else { boot(); }
})();
</script>
</article>
<article>
<header>Upload Files</header>
<form method="POST" action="/upload" enctype="multipart/form-data">
<label for="jsonUpload">Choose a config file or web content:<input type="file" id="jsonUpload" name="jsonUpload" accept=".json, .htm, .html, .js, .css"></label>
<button type="submit">Upload</button>
</form>
<form method="POST" action="/update" enctype="multipart/form-data">
<label for="update">Choose a Firmware file:<input type="file" id="update" name="update" accept=".bin"></label>
<button type="submit">Update</button>
</form>
</article>
<article>
<header>Reset Controller</header>
<form action="/reset" method="POST">
<button type="submit">Reboot</button>
</form>
</article>
)rawliteral";

#endif