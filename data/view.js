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
    let ValueUnit = document.createElement("span");
    ValueUnit.setAttribute("name", "unit");
    ValueUnit.setAttribute("style", "margin-left:-50px;text-align:left;");
    ValueUnit.innerText = DataTag.unit;
    ViewTagValue.appendChild(ValueUnit);
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
    if (DataTagValue) {
      ValueInput.value = ValueInput.getAttribute("textOn");
      ValueInput.className = "primary";
    } else {
      ValueInput.value = ValueInput.getAttribute("textOff");
      ValueInput.className = "primary outline";
    }
  } else {
    if (ValueInput !== document.activeElement) {
        ValueInput.value = DataTagValue;
    }
  }
}

function getOnChangeObject (ValueInput) {
  let Value = ValueInput.value;
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
  let text = ValueInput.getAttribute("stat");
  let Value;
  if (text == ValueInput.getAttribute("textOn")) {
    Value = false;
  } else if (text == ValueInput.getAttribute("textOff")) {
    Value = true;
  } else {
    return;
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