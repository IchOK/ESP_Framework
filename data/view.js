function updateView(DataElements, ViewType) {
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
          //for (const DataTag in DataElement[ViewType]) {
          DataTags.forEach((DataTag, DIndex) => {
            let ViewTag = ViewElement.querySelector("div[name='" + DataTag.name + "']");
            if (ViewTag === null) {
              ViewTag = createViewTag(ViewElement, DataTag, ViewType);
            }
            updateViewTag(ViewTag, DataTag);
          });
        }
      }
    }
  });
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
    ValueInput.setAttribute("stat", "off");
    ValueInput.value = DataTag.off;
    ValueInput.classList = "secondary";
  } else {
    ValueInput.setAttribute("stat", "undef");
    ValueInput.classList = "secondary outline"
  }
  ValueInput.disabled = DataTag.readOnly;
  if (DataTag.readOnly == false) {
    ValueInput.setAttribute("onclick", "onClick(this)");
  }
  ViewTagValue.appendChild(ValueInput);
}

function updateViewTag(ViewTag, DataTag) {
  if (typeof DataTag.value === "number" || typeof DataTag.value === "string") {
    let ValueInput = ViewTag.querySelector("input[name='value']");
    if (document.activeElement !== ValueInput) {
      ValueInput.value = DataTag.value
    }
  }
  else if (typeof DataTag.value === "boolean") {
    let ValueInput = ViewTag.querySelector("input[name='value']");
    if (DataTag.value) {
      ValueInput.setAttribute("stat", "on");
      ValueInput.value = DataTag.on;
      ValueInput.className = "primary"
    } else {
      ValueInput.setAttribute("stat", "off");
      ValueInput.value = DataTag.off;
      ValueInput.className = "primary outline"
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
  let stat = ValueInput.getAttribute("stat");
  let Value;
  if (stat == "on") {
    Value = false;
  } else if (stat == "off") {
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