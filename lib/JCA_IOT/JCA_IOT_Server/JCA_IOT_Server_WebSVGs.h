/**
 * @file JCA_IOT_Webserver_SVGs.h
 * @author JCA (https://github.com/ichok)
 * @brief Contains SVG's to include in Websites
 * @version 0.1
 * @date 2022-09-04
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */
#ifndef _JCA_IOT_SERVER_SVGS_
#define _JCA_IOT_SERVER_SVGS_

#include <Arduino.h>

/**
 * @brief Home Button
 *
 */
const char SvgHome[] PROGMEM = R"rawliteral(
<svg xmlns="http://www.w3.org/2000/svg" width="50" height="50" viewBox="0 0 200 200">
  <g id="Home"
    style="display:inline;fill:var(--ColorHome);stroke:var(--ColorHome);stroke-width:4;stroke-linecap:round;stroke-linejoin:round">
    <path d="M 100,20 15,85 H 35 V 180 H 75 V 115 H 125 v 65 h 40 V 85 h 20 z" />
  </g>
</svg>
)rawliteral";

/**
 * @brief Config Button
 *
 */
const char SvgConfig[] PROGMEM = R"rawliteral(
<svg xmlns="http://www.w3.org/2000/svg" width="50" height="50" viewBox="0 0 200 200">
  <g id="Config"
    style="display:inline;fill:var(--ColorConfig);stroke:var(--ColorConfig);stroke-width:0;stroke-linecap:round;stroke-linejoin:round">
    <rect width="85" height="20" x="15" y="90" />
    <rect width="45" height="20" x="15" y="30" />
    <rect width="45" height="20" x="140" y="90" />
    <rect width="65" height="20" x="15" y="150" />
    <rect style="stroke-width:5" width="30" height="30" x="65" y="25" />
    <rect style="stroke-width:5" width="30" height="30" x="105" y="85" />
    <rect style="stroke-width:5" width="30" height="30" x="85" y="145" />
    <rect width="85" height="20" x="100" y="30" />
    <rect width="65" height="20" x="120" y="150" />
  </g>
</svg>
)rawliteral";

/**
 * @brief WiFi Connect Button
 *
 */
const char SvgWiFi[] PROGMEM = R"rawliteral(
<svg xmlns="http://www.w3.org/2000/svg" width="50" height="50" viewBox="0 0 200 200">
  <g id="Config"
    style="display:inline;fill:none;stroke:var(--ColorWiFi);stroke-width:10;stroke-linecap:round;stroke-linejoin:round">
    <path d="M 60,170 A 80,80 0 0 1 60,30" />
    <path d="M 70,152 A 60,60 0 0 1 70,48" />
    <path d="M 80,135 A 40,40 0 0 1 80,65" />
    <circle style="fill:var(--ColorWiFi);stroke:none" cx="100" cy="100" r="20" />
    <path d="M 140,30 A 80,80 0 0 1 140,170" />
    <path d="M 130,48 A 60,60 0 0 1 130,152" />
    <path d="M 120,65 A 40,40 0 0 1 120,135" />
  </g>
</svg>
)rawliteral";

/**
 * @brief Setup button
 *
 */
const char SvgSetup[] PROGMEM = R"rawliteral(
<svg xmlns="http://www.w3.org/2000/svg" width="50" height="50" viewBox="0 0 200 200">
  <g id="Setup"
    style="display:inline;fill:var(--ColorSetup);stroke:var(--ColorSetup);stroke-width:0;stroke-linecap:round;stroke-linejoin:round">
    <path d="m40 22-2 4 21 12c0 10-4 18-13 23l-21-12-1 2c5 15 15 25 35 25 5 0 20 15 28 23l16-16c-8-8-26-26-26-31 0-18-5-30-37-30zm125 5-20 14-2 8-58 58-8-5-4 4c0 12-14 18-20 18l-33 33c0 10 11 21 21 21l33-33c0-6 6-20 18-20l4-4-5-8 58-58 8-2 14-20zm-50 68-16 16c12 12 28 28 28 33 0 18 5 30 37 30l2-4-21-12c0-10 4-18 13-23l21 12 1-2c-5-15-15-25-35-25-5 0-18-13-30-25z"/>
  </g>
</svg>
)rawliteral";

/**
 * @brief System button
 *
 */
const char SvgSystem[] PROGMEM = R"rawliteral(
<svg xmlns="http://www.w3.org/2000/svg" width="50" height="50" viewBox="0 0 200 200">
  <g id="Config"
    style="display:inline;fill:none;stroke:var(--ColorSystem);stroke-width:10;stroke-linecap:round;stroke-linejoin:round">
    <path d="M 108,20 A 80,80 0 0 1 132,27 L 130,48 A 60,60 0 0 1 152,70 L 173,67 A 80,80 0 0 1 180,92 L 160,100 A 60,60 0 0 1 152,130 L 165,147 A 80,80 0 0 1 147,165 L 130,152 A 60,60 0 0 1 100,160 L 92,180 A 80,80 0 0 1 67,173 L 70,152 A 60,60 0 0 1 48,130 L 27,133 A 80,80 0 0 1 20,108 L 40,100 A 60,60 0 0 1 48,70 L 35,53 A 80,80 0 0 1 53,35 L 70,48 A 60,60 0 0 1 100,40 L 108,20" />
    <circle style="fill:var(--ColorSystem);stroke:none" cx="100" cy="100" r="30" />
  </g>
</svg>
)rawliteral";

/**
 * @brief JCA Logo
 *
 */
const char SvgLogo[] PROGMEM = R"rawliteral(
<svg width="50" height="50" version="1.1" viewBox="0 0 160 160" xmlns="http://www.w3.org/2000/svg">
 <g transform="translate(0 -137)" style="stroke-linecap:round;fill:none">
  <path style="stroke:var(--secondary)" d="m38 251s16 21 42 21c26 0 42-10.5 42-33v-78" stroke-width="30"/>
  <path style="stroke:var(--primary)" d="m101 249a41 40 0 0 1-49-12 41 40 0 0 1-0-50 41 40 0 0 1 49-12" stroke-width="22"/>
  <path style="stroke:var(--contrast)" d="m95 192a21 22 0 0 1 8 30 21 22 0 0 1-29 8 21 22 0 0 1-8-30 21 22 0 0 1 29-8" stroke-width="20"/>
  <path style="stroke:var(--contrast)" d="m107 185v50" stroke-width="20"/>
 </g>
</svg>
)rawliteral";

/**
 * @brief Plus Icon
 *
 */
const char SvgPlus[] PROGMEM = R"rawliteral(
<svg viewBox="0 0 24 24" aria-hidden="true">
 <path d="M12 5v14M5 12h14"/></svg>
)rawliteral";

/**
 * @brief Trash Icon
 *
 */
const char SvgTrash[] PROGMEM = R"rawliteral(
<svg viewBox="0 0 24 24" aria-hidden="true">
 <path d="M3 6h18"/>
 <path d="M8 6V4h8v2"/>
 <path d="M19 6l-1 14H6L5 6"/>
 <path d="M10 10v7M14 10v7"/>
</svg>
)rawliteral";

#endif