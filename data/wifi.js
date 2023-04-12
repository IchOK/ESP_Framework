function createWiFiTagInputPass(ViewTagValue) {
  let ValueInput = document.createElement("input");
  ValueInput.setAttribute("name", "value");
  ValueInput.setAttribute("style", "padding-right:54px;text-align:right;");
  ValueInput.type = "password";
  ViewTagValue.appendChild(ValueInput);
}

function createWiFiTagInputText(ViewTagValue) {
  let ValueInput = document.createElement("input");
  ValueInput.setAttribute("name", "value");
  ValueInput.setAttribute("style", "padding-right:54px;text-align:right;");
  ValueInput.type = "text";
  ViewTagValue.appendChild(ValueInput);
}

function createWiFiTagInputBool(ViewTagValue) {
  let ValueInput = document.createElement("input");
  ValueInput.type = "button";
  ValueInput.setAttribute("name", "value");
  ValueInput.setAttribute("style", "text-align:center;");
  ValueInput.setAttribute("textOn", "On");
  ValueInput.setAttribute("textOff", "Off");
  ValueInput.classList = "secondary outline"
  ValueInput.setAttribute("onclick", "onToggle(this)");
  ViewTagValue.appendChild(ValueInput);
}

function createWifiTag(ViewElement, ViewName, TagName, TagType) {
  // create Tag Row
  let ViewTag = document.createElement("div");
  ViewTag.setAttribute("name", TagName);
  ViewTag.setAttribute("style", "align-items:baseline ;");
  ViewTag.classList.add("grid");
  // create Tag Name
  let ViewTagName = document.createElement("p");
  ViewTagName.id = "name";
  ViewTagName.innerText = ViewName;
  ViewTag.appendChild(ViewTagName);
  // create Tag Value Block
  let ViewTagValue = document.createElement("div");
  ViewTagValue.setAttribute("style", "display:flex;align-items:baseline;");
  ViewTag.appendChild(ViewTagValue);
  if (TagType === "pass") {
    createWiFiTagInputPass(ViewTagValue);
  }
  else if (TagType === "text") {
    createWiFiTagInputText(ViewTagValue);
  }
  else if (TagType === "bool") {
    createWiFiTagInputBool(ViewTagValue);
  }
  // add Tag to Element
  ViewElement.appendChild(ViewTag);
  return ViewTag;
}

function createWiFiElement(ViewElements) {
  let ViewElement = document.createElement("article");
  let ViewElementHeader;
  ViewElementHeader = document.createElement("header");
  ViewElementHeader.innerText = "WiFi Config";
  ViewElement.appendChild(ViewElementHeader);
  createWifiTag(ViewElement, "SSID", "ssid", "text");
  createWifiTag(ViewElement, "Password", "pass", "pass");
  createWifiTag(ViewElement, "DHCP", "dhcp", "bool");
  createWifiTag(ViewElement, "Ststic IP", "ip", "text");
  createWifiTag(ViewElement, "Subnet-Mask", "subnet", "text");
  createWifiTag(ViewElement, "Gateway", "gateway", "text");
  let ViewElementDelete = document.createElement("input");
  ViewElementDelete.type = "button";
  ViewElementDelete.classList = "secondary";
  ViewElementDelete.value = "Delete";
  ViewElementDelete.setAttribute("onclick", "onDelete(this)");
  ViewElement.appendChild(ViewElementDelete);
  ViewElements.appendChild(ViewElement);
  return ViewElement;
}

function updateWiFiTag(ViewElement, TagName, Value) {
  let ViewTag = ViewElement.querySelector("div[name='" + TagName + "']");
  if (ViewTag !== null) {
    let ValueInput = ViewTag.querySelector("input[name='value']");
    if (ValueInput.type === "button") {
      if (Value) {
        ValueInput.value = ValueInput.getAttribute("textOn");
        ValueInput.className = "primary";
      } else {
        ValueInput.value = ValueInput.getAttribute("textOff");
        ValueInput.className = "primary outline";
      }
    } else {
      ValueInput.value = Value;
    }
  }
}

function setWiFiElement(ViewElement, DataElement) {
  for (const Property in DataElement) {
    updateWiFiTag(ViewElement, Property, DataElement[Property]);
  }
}

function onToggle(ValueInput) {
  if (ValueInput.value !== ValueInput.getAttribute("textOn")) {
    ValueInput.value = ValueInput.getAttribute("textOn");
    ValueInput.className = "primary";
  } else {
    ValueInput.value = ValueInput.getAttribute("textOff");
    ValueInput.className = "primary outline";
  }
}

function getWiFiObject(ViewElement) {
  let WiFiObject = {};
  let ViewTags
  //Auslesen aller Textfelder
  ViewTags = ViewElement.querySelectorAll("input[type='text'], input[type='password']");
  ViewTags.forEach((ViewTag, TIndex) => {
    let TagName = ViewTag.parentNode.parentNode.getAttribute("name");
    if (TagName !== null) {
      WiFiObject[TagName] = ViewTag.value;
    }
  });
  //Auslesen aller Taster
  ViewTags = ViewElement.querySelectorAll("input[type='button']");
  ViewTags.forEach((ViewTag, TIndex) => {
    let TagName = ViewTag.parentNode.parentNode.getAttribute("name");
    if (TagName !== null) {
      WiFiObject[TagName] = ViewTag.value === ViewTag.getAttribute("texton");
    }
  });
  return WiFiObject;
}

