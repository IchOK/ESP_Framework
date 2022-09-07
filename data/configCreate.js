function createNewConfigSlave(that) {
  console.log(that);
  let btnFunc = that.getAttribute("onclick");
  let thatParent = that.parentElement;
  that.remove();
  createConfigSlave(thatParent, {tags:[]});
  createNewButton(thatParent, btnFunc, "New Slave", "secondary");
}

function createNewButton(root, func, btnText, btnClass) {
  let btn = document.createElement("button");
  btn.setAttribute("onclick", func);
  btn.classList.add(btnClass);
  btn.innerText = btnText;
  root.appendChild(btn);
}

function createConfigSlave(slaves, config) {
  let slave = document.createElement("article");
  // Create Header as Slave Config
  let x = document.createElement("header");
  let data = document.createElement("div");
  data.classList.add("grid");
  createConfigText(data, config, "name", "Name");
  createConfigText(data, config, "id", "Adr.");
  x.appendChild(data);
  slave.appendChild(x);
  // Create Tag-Creation-Button
  createNewButton(slave, "this.parentElement.remove()", "Delete Slave", "attention");
  slaves.appendChild(slave);
}

function createConfigLabel(tag, tagName, tagText) {
  let y = document.createElement("label");
  y.setAttribute("for", tagName);
  y.innerText = tagText;
  tag.appendChild(y);
  return y;
}

function createConfigText(tag, config, typeStr, typeText) {
  let y = createConfigLabel(tag, typeStr, typeText);
  let x = document.createElement("input");
  x.id = typeStr;
  x.setAttribute("name", typeStr);
  x.type = "text";
  if (typeStr in config) {
    x.value = config[typeStr];
  }
  y.appendChild(x);
}

