function updateView(DataObject, ViewType, ReadOnly) {
  //Get main View-Object
  let ViewElements = document.getElementById("elements");
  for (const [ElementName, ElementObject] of Object.entries(DataObject)) {
    if ((typeof ElementObject === "object" || typeof ElementObject === 'function') && (ElementObject !== null)) {
      if (ViewType in ElementObject) {
        let ViewElement = ViewElements.querySelector("article[name='" + ElementName + "']");
        if (ViewElement === null) {
          ViewElement = createViewElement(ViewElements, ElementName);
        }
        for (const [TagName, TagObject] of Object.entries(ElementObject[ViewType])) {
          let ViewTag = ViewElement.querySelector("div[name='" + TagName + "']");
          if (ViewTag === null) {
            ViewTag = createViewTag(ViewElement, TagName, TagObject, ReadOnly);
          }
          updateViewTag(ViewTag, TagObject);
        }
      }
    }
  }
}

function createViewElement(ViewElements, ElementName) {
  let ViewElement = document.createElement("article");
  ViewElement.setAttribute("name", ElementName);
  let x;
  x = document.createElement("header");
  x.innerText = ElementName;
  ViewElement.appendChild(x);
  ViewElements.appendChild(ViewElement);
  return ViewElement;
}

function createViewTag(ViewElement, TagName, TagObject, ReadOnly) {
  // create Tag Row
  let ViewTag = document.createElement("div");
  ViewTag.setAttribute("name", TagName);
  ViewTag.setAttribute("style", "align-items:baseline ;");
  ViewTag.classList.add("grid");
  // create Tag Name
  let TagName = document.createElement("p");
  TagName.id = "name";
  TagName.innerText = TagName;
  ViewTag.appendChild(TagName);
  // create Tag Value Block
  let TagValue = document.createElement("div");
  TagValue.setAttribute("style", "display:flex;align-items:baseline;");
  ViewTag.appendChild(TagValue);
  // - create Tag input Field
  let ValueInput = document.createElement("input");
  ValueInput.setAttribute("name", "value");
  ValueInput.setAttribute("style", "padding-right:54px;text-align:right;");
  ValueInput.type = "text";
  ValueInput.readOnly = ReadOnly;
  TagValue.appendChild(ValueInput);
  // - create Tag unit Field
  let ValueUnit = document.createElement("span");
  ValueUnit.setAttribute("name", "unit");
  ValueUnit.setAttribute("style", "margin-left:-50px;text-align:left;");
  if ("unit" in TagObject) {
    ValueUnit.innerText = TagObject.unit;
  }
  TagValue.appendChild(ValueUnit);
  // add Tag to Element
  ViewElement.appendChild(ViewTag);
  return ViewTag;
}

function updateViewTag(ViewTag, TagObject) {
  let ValueInput
  ValueInput = ViewTag.querySelector("input[name='value']");
  ValueInput.value = TagObject.value;
}