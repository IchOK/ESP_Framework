function createView(DataElements, ViewType) {
  //Loop Elements
  let ViewElements = document.getElementById("elements");
  for (const [DataElementName, DataElementValue] of Object.entries(DataElements)) {
    if ((typeof DataElementValue === "object") && (DataElementValue !== null)) {
      if (ViewType in DataElementValue) {
        let DataTags = DataElementValue[ViewType];
        if (DataTags.length > 0) {
          let ViewElement = ViewElements.querySelector("article[name='" + DataElementName + "']");
          if (ViewElement === null) {
            ViewElement = createViewElement(ViewElements, DataElementName, DataElementValue);
          }
          DataTags.forEach((DataTag, DIndex) => {
            let ViewTag = ViewElement.querySelector("div[name='" + DataTag.name + "']");
            if (ViewTag === null) {
              ViewTag = createViewTag(ViewElement, DataTag, ViewType);
            }
          });
        }
      }
    }
  }
}

function updateView(DataElements) {
  //Loop Elements
  let ViewElements = document.getElementById("elements");
  for (const [DataElementName, DataElementValue] of Object.entries(DataElements)) {
    let ViewElement = ViewElements.querySelector("article[name='" + DataElementName + "']");
    if (ViewElement !== null) {
      for (const [DataTagName, DataTagValue] of Object.entries(DataElementValue)) {
        let ViewTag = ViewElement.querySelector("div[name='" + DataTagName + "']");
        if (ViewTag !== null) {
          updateViewTag(ViewTag, DataTagValue);
        }
      }
    }
  }
}

function createViewElement(ViewElements, DataElementName, DataElementValue) {
  let ViewElement = document.createElement("article");
  ViewElement.setAttribute("name", DataElementName);
  let ViewElementHeader;
  ViewElementHeader = document.createElement("header");
  ViewElementHeader.innerText = DataElementName;
  ViewElement.appendChild(ViewElementHeader);
  if ("comment" in DataElementValue) {
    let ViewElementFooter;
    ViewElementFooter = document.createElement("footer");
    ViewElementFooter.innerText = DataElementValue.comment;
    ViewElement.appendChild(ViewElementFooter);
  }
  ViewElements.appendChild(ViewElement);
  return ViewElement;
}


function createViewTag(ViewElement, DataTag) {
  // create Tag Row
  let ViewTag = document.createElement("div");
  ViewTag.setAttribute("name", DataTag.name);
  ViewTag.setAttribute("style", "align-items:baseline ;");
  ViewTag.classList.add("grid");
  // create Tag Name
  let TagName = document.createElement("p");
  TagName.id = "name";
  TagName.innerText = DataTag.text;
  if ("comment" in DataTag) {
    TagName.setAttribute("data-tooltip", DataTag.comment);
  }
  ViewTag.appendChild(TagName);
  // create Tag Value Block
  let ViewTagValue = document.createElement("div");
  ViewTagValue.setAttribute("style", "display:flex;align-items:baseline;");
  ViewTag.appendChild(ViewTagValue);
  switch (DataTag.type) {
    case 1:
      createViewTagInputBoolean(ViewTagValue, DataTag, false);
      break;
    case 54:
      createViewTagInputBoolean(ViewTagValue, DataTag, true);
      break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 51:
    case 52:
    case 53:
      createViewTagInputNumber(ViewTagValue, DataTag);
      break;

    case 9:
      createViewTagInputString(ViewTagValue, DataTag);
      break;

    default:
      createViewTagInputString(ViewTagValue, DataTag);
      break;
  }
  // add Tag to Element
  ViewElement.appendChild(ViewTag);
  return ViewTag;
}

function createViewTagInputNumber(ViewTagValue, DataTag) {
  // create Number-Input Field
  let ValueInput = document.createElement("input");
  let AddUnit = false;
  ValueInput.setAttribute("name", "value");
  ValueInput.setAttribute("style", "padding-right:54px;text-align:right;");
  ValueInput.setAttribute("step", "any");
  switch (DataTag.type) {
    case 51:
      ValueInput.type = "time";
      break;
    case 52:
      ValueInput.type = "datetime-local";
      break;
    case 53:
      ValueInput.type = "color";
      break;
    default:  
      ValueInput.type = "number";
      AddUnit = "unit" in DataTag;
      break;
  }
  ValueInput.disabled = DataTag.readOnly;
  if (DataTag.readOnly == false) {
    ValueInput.setAttribute("onchange", "onChange(this)");
  }
  ViewTagValue.appendChild(ValueInput);
  // create Unit Field
  if (AddUnit) {
    let ValueUnit = document.createElement("span");
    ValueUnit.setAttribute("name", "unit");
    ValueUnit.setAttribute("style", "margin-left:-50px;text-align:left;");
    ValueUnit.innerText = DataTag.unit;
    ViewTagValue.appendChild(ValueUnit);
  }
}

function createViewTagInputString(ViewTagValue, DataTag) {
  // - create String-Input Field
  let ValueInput = document.createElement("input");
  ValueInput.setAttribute("name", "value");
  ValueInput.setAttribute("style", "padding-right:54px;text-align:right;");
  ValueInput.type = "text";
  ValueInput.disabled = DataTag.readOnly;
  if (DataTag.readOnly == false) {
    ValueInput.setAttribute("onchange", "onChange(this)");
  }
  ViewTagValue.appendChild(ValueInput);
}

function createViewTagInputBoolean(ViewTagValue, DataTag, IsCommand) {
  // - create Button Field
  let ValueInput = document.createElement("input");
  ValueInput.type = "button";
  ValueInput.setAttribute("name", "value");
  ValueInput.setAttribute("style", "text-align:center;");
  ValueInput.setAttribute("cmd", IsCommand);
  if (IsCommand) {
    ValueInput.value = DataTag.off;
    ValueInput.setAttribute("textOff", DataTag.off);
    ValueInput.classList = "secondary";
  } else {
    ValueInput.setAttribute("textOn", DataTag.on);
    ValueInput.setAttribute("textOff", DataTag.off);
    ValueInput.classList = "secondary outline"
  }
  ValueInput.disabled = DataTag.readOnly;
  if (DataTag.readOnly == false) {
    ValueInput.setAttribute("onclick", "onClick(this)");
  }
  ViewTagValue.appendChild(ValueInput);
}

function timestampToDatetimeLocal(timestamp) {
  // Timestamp in Millisekunden umwandeln
  const date = new Date(timestamp * 1000);

  // Jahr, Monat, Tag, Stunden und Minuten extrahieren
  const year = date.getFullYear();
  const month = (date.getMonth() + 1).toString().padStart(2, '0'); // Monate von 0-11
  const day = date.getDate().toString().padStart(2, '0');
  const hours = date.getHours().toString().padStart(2, '0');
  const minutes = date.getMinutes().toString().padStart(2, '0');

  // Format für datetime-local: YYYY-MM-DDTHH:mm
  return `${year}-${month}-${day}T${hours}:${minutes}`;
}

function updateViewTag(ViewTag, DataTagValue) {
  let ValueInput = ViewTag.querySelector("input[name='value']");
  if (ValueInput.type === "button") {
    if (ValueInput.getAttribute("cmd") == "false") {
      if (DataTagValue) {
        ValueInput.value = ValueInput.getAttribute("textOn");
        ValueInput.className = "primary";
      } else {
        ValueInput.value = ValueInput.getAttribute("textOff");
        ValueInput.className = "primary outline";
      }
    }
  } else {
    if (ValueInput !== document.activeElement) {
      switch (ValueInput.type) {
        case "color":
          ValueInput.value = intToRgb(DataTagValue);
          break;
        case "time":
          const hours = Math.floor(DataTagValue / 3600).toString().padStart(2, '0');
          const minutes = Math.floor((DataTagValue % 3600) / 60).toString().padStart(2, '0');
          ValueInput.value = hours + ":" + minutes;
          break;
        case "datetime-local":
          ValueInput.value = timestampToDatetimeLocal(DataTagValue);
          break;
        default:
          ValueInput.value = DataTagValue;
          break;
      }
    }
  }
}

function getOnChangeObject (ValueInput) {
  var Value;
  switch (ValueInput.type) {
    case "color":
      Value = rgbToInt(ValueInput.value);
      break;
    case "time":
      const [hours, minutes] = ValueInput.value.split(":");
      Value = (parseInt(hours) * 60 + parseInt(minutes)) * 60;
      break;
    case "datetime-local":
      const date = new Date(ValueInput.value);
      Value = Math.floor(date.getTime() / 1000);
      break;
    default:
      Value = ValueInput.value;
      break;
  }
  let ViewTag = ValueInput.parentElement.parentElement;
  let ViewElement = ViewTag.parentElement;
  let DataElement = {};
  DataElement[ViewElement.getAttribute("name")] = {};
  DataElement[ViewElement.getAttribute("name")][ViewTag.getAttribute("name")] = Value;
  let DataElements = { "elements": DataElement };
  return DataElements;
}

function getOnClickObject(ValueInput) {
  let Value;
  if (ValueInput.getAttribute("cmd") == true) {
    Value = true;
  } else {
    let text = ValueInput.getAttribute("value");
    if (text === ValueInput.getAttribute("textOn")) {
      Value = false;
    } else if (text === ValueInput.getAttribute("textOff")) {
      Value = true;
    } else {
      return;
    }
  }
  let ViewTag = ValueInput.parentElement.parentElement;
  let ViewElement = ViewTag.parentElement;
  let DataElement = { };
  DataElement[ViewElement.getAttribute("name")] = {};
  DataElement[ViewElement.getAttribute("name")][ViewTag.getAttribute("name")] = Value;
  let DataElements = { "elements": DataElement };
  return DataElements;
}

function intToRgb(ValueInt) {
  let ValueRgbSwap = "#" + ('000000' + ((ValueInt) >>> 0).toString(16)).slice(-6);
  let ValueRgb = ValueRgbSwap.substring(0, 3) + ValueRgbSwap.substring(5, 7) + ValueRgbSwap.substring(3, 5);
  return ValueRgb;
}

function rgbToInt(ValueRgb) {
  let ValueRgbBlank = ValueRgb.replace("#","0x");
  let ValueRgbSwap = ValueRgbBlank.substring(0, 4) + ValueRgbBlank.substring(6, 8) + ValueRgbBlank.substring(4, 6);
  let ValueInt = parseInt(ValueRgbSwap)
  return ValueInt;
}