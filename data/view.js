function createViewSlave(elements, key, values) {
  let element = document.createElement("article");
  element.setAttribute("name", key);
  let x;
  x = document.createElement("header");
  x.innerText = key;
  element.appendChild(x);
  
  Object.keys(values).forEach(key => {
    createViewTag(element, key, values[key]);
  });
  elements.appendChild(element);
}

function createViewTag(element, key, value) {
  let tag = document.createElement("div");
  tag.setAttribute("name", key);
  tag.setAttribute("style", "align-items:baseline ;");
  tag.classList.add("grid");
  let x;
  x = document.createElement("p");
  x.id = "name";
  x.innerText = key;
  tag.appendChild(x);
  
  let y = document.createElement("div");
  y.setAttribute("style", "display:flex;align-items:baseline ;");
  
  x = document.createElement("input");
  x.setAttribute("name", "value");
  x.setAttribute("style", "padding-right:54px; text-align:right;");
  x.type = "text";
  y.appendChild(x);
  
  tag.appendChild(y);
  
  element.appendChild(tag);
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
  x.value = data;
}