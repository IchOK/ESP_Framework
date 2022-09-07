function saveConfigSlave(jSlaves, slave) {
  // Create epmty Slave config
  let jSlave = {};
  // Slave Config
  let dElements;
  dElements = slave.querySelector("header div").getElementsByTagName("label");
  for (let i = 0; i < dElements.length; i++) {
    let dInput = dElements[i].querySelector("input");
    jSlave[dInput.name] = dInput.value;
  }
  jSlaves.push(jSlave);
}