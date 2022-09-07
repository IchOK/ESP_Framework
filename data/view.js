function createViewSlave(slaves, config) {
  let slave = document.createElement("article");
  slave.setAttribute("name", config.name);
  let x;
  x = document.createElement("header");
  x.innerText = config.name;
  slave.appendChild(x);
  
  config.tags.forEach((_tagConfig) => {
    createViewTag(slave, _tagConfig);
  });
  slaves.appendChild(slave);
}

function createViewTag(slave, config) {
  let tag = document.createElement("div");
  tag.setAttribute("name", config.name);
  tag.setAttribute("style", "align-items:baseline ;");
  tag.classList.add("grid");
  let x;
  x = document.createElement("p");
  x.id = "name";
  x.innerText = config.name;
  tag.appendChild(x);
  
  let y = document.createElement("div");
  y.setAttribute("style", "display:flex;align-items:baseline ;");
  
  x = document.createElement("input");
  x.setAttribute("name", "value");
  x.setAttribute("style", "padding-right:54px; text-align:right;");
  x.type = "text";
  y.appendChild(x);
  
  x = document.createElement("span");
  x.setAttribute("name", "unit");
  x.setAttribute("style", "margin-left:-50px;text-align:left;");
  if ("unit" in config) {
    x.innerText = config.unit;
  }
  y.appendChild(x);
  tag.appendChild(y);
  
  slave.appendChild(tag);
}

function getViewSlave(slaves, name) {
  let slave = slaves.querySelector("article[name='" + name + "']");
  return slave;
}

function getViewTag(slave, name) {
  let tag = slave.querySelector("div[name='" + name + "']");
  return tag;
}

function updateViewTag(tag, data) {
  let x
  x = tag.querySelector("input[name='value']");
  x.value = data.value;
}