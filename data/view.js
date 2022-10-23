function updateView(DataElements, ViewType, ReadOnly) {
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
              ViewTag = createViewTag(ViewElement, DataTag, ReadOnly);
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


function createViewTag(ViewElement, DataTag, ReadOnly) {
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
  let TagValue = document.createElement("div");
  TagValue.setAttribute("style", "display:flex;align-items:baseline;");
  ViewTag.appendChild(TagValue);
  if (typeof DataTag.value === "number") {
    // create Number-Input Field
    let ValueInput = document.createElement("input");
    ValueInput.setAttribute("name", "value");
    ValueInput.setAttribute("style", "padding-right:54px;text-align:right;");
    ValueInput.setAttribute("step", "any");
    ValueInput.type = "number";
    ValueInput.disabled = ReadOnly;
    if (ReadOnly == false) {
      ValueInput.setAttribute("onchange", "onChange(this)");
    }
    TagValue.appendChild(ValueInput);
    // create Unit Field
    if ("unit" in DataTag) {
      let ValueUnit = document.createElement("span");
      ValueUnit.setAttribute("name", "unit");
      ValueUnit.setAttribute("style", "margin-left:-50px;text-align:left;");
      ValueUnit.innerText = DataTag.unit;
      TagValue.appendChild(ValueUnit);
    }
  }
  else if (typeof DataTag.value === "string") {
    // - create String-Input Field
    let ValueInput = document.createElement("input");
    ValueInput.setAttribute("name", "value");
    ValueInput.setAttribute("style", "padding-right:54px;text-align:right;");
    ValueInput.type = "text";
    ValueInput.disabled = ReadOnly;
    if (ReadOnly == false) {
      ValueInput.setAttribute("onchange", "onChange(this)");
    }
    TagValue.appendChild(ValueInput);
  }
  else if (typeof DataTag.value === "boolean") {
    // - create Button Field
    let ValueInput = document.createElement("input");
    ValueInput.type = "button";
    ValueInput.setAttribute("name", "value");
    ValueInput.setAttribute("style", "padding-right:54px;text-align:right;");
    ValueInput.setAttribute("stat", "undef");
    ValueInput.classList = "secondary outline"
    ValueInput.disabled = ReadOnly;
    if (ReadOnly == false) {
      ValueInput.setAttribute("onclick", "onClick(this)");
    }
    TagValue.appendChild(ValueInput);
  }
  // add Tag to Element
  ViewElement.appendChild(ViewTag);
  return ViewTag;
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