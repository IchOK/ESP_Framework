function createView(DataElements, ViewType) {
  //Loop Elements
  let ViewElements = document.getElementById("elements");
  DataElements.forEach ((DataElement, EIndex) => {
    if ((typeof DataElement === "object") && (DataElement !== null)) {
      if (ViewType in DataElement) {
        let DataTags = DataElement[ViewType];
        if (DataTags.length > 0) {
          let ViewElement = ViewElements.querySelector("article[name='" + DataElement.name + "']");
          if (ViewElement === null) {
            ViewElement = createViewElement(ViewElements, DataElement);
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
  });
}

function updateView(DataElements, ViewType) {
  //Loop Elements
  let ViewElements = document.getElementById("elements");
  for (const [DataElementName, DataElementValue] of Object.entries(DataElements)) {
    if (ViewType in DataElementValue) {
      let ViewElement = ViewElements.querySelector("article[name='" + DataElementName + "']");
      if (ViewElement !== null) {
        let DataTagValues = DataElementValue[ViewType];
        for (const [DataTagName, DataTagValue] of Object.entries(DataTagValues)) {
          let ViewTag = ViewElement.querySelector("div[name='" + DataTagName + "']");
          if (ViewTag !== null) {
            updateViewTag(ViewTag, DataTagValue);
          }
        }
      }
    }
  }
}

function createViewElement(ViewElements, DataElement) {
  let ViewElement = document.createElement("article");
  ViewElement.setAttribute("name", DataElement.name);
  let ViewElementHeader;
  ViewElementHeader = document.createElement("header");
  ViewElementHeader.innerText = DataElement.name;
  ViewElement.appendChild(ViewElementHeader);
  if ("comment" in DataElement) {
    let ViewElementFooter;
    ViewElementFooter = document.createElement("footer");
    ViewElementFooter.innerText = DataElement.comment;
    ViewElement.appendChild(ViewElementFooter);
  }
  ViewElements.appendChild(ViewElement);
  return ViewElement;
}


function createViewTag(ViewElement, DataTag, ViewType) {
  // create Tag Row
  let ViewTag = document.createElement("div");
  ViewTag.setAttribute("name", DataTag.name);
  ViewTag.setAttribute("style", "align-items:baseline ;");
  ViewTag.setAttribute("tagGroup", ViewType);
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
  if ("type" in DataTag) {
    if (DataTag.type == "bool") {
      createViewTagInputBoolean(ViewTagValue, DataTag, true);
    } else {
      createViewTagInputString(ViewTagValue, DataTag, true);
    }
  }
  else if (typeof DataTag.value === "number") {
    createViewTagInputNumber(ViewTagValue, DataTag, false);
  }
  else if (typeof DataTag.value === "string") {
    createViewTagInputString(ViewTagValue, DataTag, false);
  }
  else if (typeof DataTag.value === "boolean") {
    createViewTagInputBoolean(ViewTagValue, DataTag, false);
  }
  // add Tag to Element
  ViewElement.appendChild(ViewTag);
  return ViewTag;
}

function createViewTagInputNumber(ViewTagValue, DataTag, IsCommand) {
  // create Number-Input Field
  let ValueInput = document.createElement("input");
  ValueInput.setAttribute("name", "value");
  ValueInput.setAttribute("style", "padding-right:54px;text-align:right;");
  ValueInput.setAttribute("step", "any");
  ValueInput.type = "number";
  ValueInput.disabled = DataTag.readOnly;
  if (DataTag.readOnly == false) {
    ValueInput.setAttribute("onchange", "onChange(this)");
  }
  ViewTagValue.appendChild(ValueInput);
  // create Unit Field
  if ("unit" in DataTag) {
    if (DataTag.unit == "COLOR") {
      ValueInput.type = "color";
    } else {
      let ValueUnit = document.createElement("span");
      ValueUnit.setAttribute("name", "unit");
      ValueUnit.setAttribute("style", "margin-left:-50px;text-align:left;");
      ValueUnit.innerText = DataTag.unit;
      ViewTagValue.appendChild(ValueUnit);
    }
  }
}

function createViewTagInputString(ViewTagValue, DataTag, IsCommand) {
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
      if (ValueInput.type == "color") {
        ValueInput.value = intToRgb(DataTagValue);
      } else {
        ValueInput.value = DataTagValue;
      }
    }
  }
}

function getOnChangeObject (ValueInput) {
  var Value;
  if (ValueInput.type == "color") {
    Value = rgbToInt(ValueInput.value);
  } else {
    Value = ValueInput.value;
  }
  let ViewTag = ValueInput.parentElement.parentElement;
  let TagGrp = ViewTag.getAttribute("tagGroup");
  if (TagGrp == "cmdInfo") {
    TagGrp = "cmd";
  }
  let ViewElement = ViewTag.parentElement;
  let DataElement = {"name": ViewElement.getAttribute("name")};
  DataElement[TagGrp] = [{ "name": ViewTag.getAttribute("name"), "value": Value }];
  let DataElements = { "elements": []};
  DataElements.elements.push(DataElement);
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
  let TagGrp = ViewTag.getAttribute("tagGroup");
  if (TagGrp == "cmdInfo") {
    TagGrp = "cmd";
  }
  let ViewElement = ViewTag.parentElement;
  let DataElement = { "name": ViewElement.getAttribute("name") };
  DataElement[TagGrp] = [{ "name": ViewTag.getAttribute("name"), "value": Value }];
  let DataElements = { "elements": [] };
  DataElements.elements.push(DataElement);
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