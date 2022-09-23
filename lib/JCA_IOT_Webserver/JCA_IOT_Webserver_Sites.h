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

#ifndef _JCA_IOT_WEBSERVER_SITES_
  #define _JCA_IOT_WEBSERVER_SITES_

  #include <Arduino.h>

/**
 * @brief Static Web Frame for Famework Sites
 */
const char PageFrame[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<title>%TITLE%</title>
<link rel="stylesheet" type="text/css" href="/style.css">
<link rel="stylesheet" type="text/css" href="/styleMobile.css">
<link rel="stylesheet" type="text/css" href="/styleAddon.css">
<style>:root{--ColorHome:var(--secondary);--ColorConfig:var(--secondary);--ColorWiFi:var(--secondary);--ColorSystem:var(--secondary)}</style>
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
</article>
<article>
<header>Save Config</header>
<a href="/config.json" download="config.json"><button>Download Config</button></a>
</article>
<article>
<header>Upload Web-Content</header>
<form method="POST" action="/upload" enctype="multipart/form-data">
<label for="jsonUpload">Choose a config file or web content:<input type="file" id="jsonUpload" name="jsonUpload" accept=".json, .htm, .html, .js, .css"></label>
<button type="submit">Upload</button>
</form>
</article>
<article>
<header>Firmware</header>
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