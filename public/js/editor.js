// Initialize ace editor
var editor = ace.edit("editor");
editor.setTheme("ace/theme/monokai");
editor.session.setMode("ace/mode/python");
editor.getSession().setUseWorker(false);
var output = document.getElementById("output");
var save_text = document.getElementById("save-text");

const exampleSelector = document.getElementById('example-selector');

async function loadExamplePrograms() {
  try {
    const response = await fetch('/example-programs/index.json');
    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }
    const examples = await response.json();
    
    examples.forEach(example => {
      const option = document.createElement('option');
      option.value = example.file;
      option.textContent = example.name;
      exampleSelector.appendChild(option);
    });
  } catch (error) {
    console.error("Error loading example programs:", error);
  }
}

async function loadSelectedExample() {
  const selectedFile = exampleSelector.value;
  if (selectedFile) {
    try {
      const response = await fetch(`/example-programs/${selectedFile}`);
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const code = await response.text();
      editor.setValue(code, -1);
      saveCode();
    } catch (error) {
      console.error("Error loading example:", error);
    }
  }
}

// Start code editor
startEditor();

let wabtModule;

async function initWabt() {
  try {
    wabtModule = await WabtModule();
    console.log('WABT initialized successfully');
  } catch (error) {
    console.error('Error initializing WABT:', error);
  }
}

initWabt();

async function wat2wasm(watCode) {
  if (!wabtModule) {
    throw new Error('WABT not initialized');
  }
  try {
    const module = wabtModule.parseWat('temp.wat', watCode);
    const { buffer } = module.toBinary({});
    return buffer;
  } catch (error) {
    console.error('Error in wat2wasm:', error);
    throw error;
  }
}

function updateWatSyntax(watCode) {
  return watCode
    .replace(/\(set_local /g, '(local.set ')
    .replace(/\(get_local /g, '(local.get ')
    .replace(/\(tee_local /g, '(local.tee ')
    .replace(/\(set_global /g, '(global.set ')
    .replace(/\(get_global /g, '(global.get ');
}

function generateUUID() {
  let uuid = '', i, random;
  for (i = 0; i < 32; i++) {
    random = Math.random() * 16 | 0;
    if (i === 8 || i === 12 || i === 16 || i === 20) {
      uuid += '-';
    }
    uuid += (i === 12 ? 4 : (i === 16 ? (random & 3 | 8) : random)).toString(16);
  }
  return uuid;
}

// Function to start up code interface
function startEditor() {
  // Set the UUID if it does not exist yet
  if (localStorage.getItem('uuid') === "" || localStorage.getItem('uuid') === null) {
    localStorage.setItem('uuid', generateUUID());
  }

  // Startup using the localStorage variable "codeInput", otherwise generate default starter code    
  if (localStorage.getItem('codeInput') === "" || localStorage.getItem('codeInput') === null) {
    editor.setValue("# Artsy Starter Program\nprint \"Hello Artsy\";", -1); // include hello world program by default
    saveCode();
  } else {
    console.log("Your progress was restored!");
    editor.setValue(localStorage.getItem('codeInput'));
  }
    // Load example programs
    loadExamplePrograms();

    // Add event listener for example selection
    exampleSelector.addEventListener('change', loadSelectedExample);
}

// Function to run code and display output in output box
async function displayCode(watCode) {
  try {
    // Update WAT syntax
    const updatedWatCode = updateWatSyntax(watCode);
    
    // Convert WAT to WASM
    const wasmBuffer = await wat2wasm(updatedWatCode);
    const wasmModule = await WebAssembly.compile(wasmBuffer);
    
    // Instantiate the WASM module
    const instance = await WebAssembly.instantiate(wasmModule, {
      env: {
        print_int: function printInt(wVar) {
          output.innerHTML += wVar;
        },
        print_float: function printFloat(wVar) {
          output.innerHTML += wVar;
        },
        print_string: function printString(wVar) {
          output.innerHTML += String.fromCharCode(wVar);
        },
        newline: function newline() {
          output.innerHTML += "\n";
        }
      }
    });

    // Run the start function if it exists
    if (instance.exports._start) {
      instance.exports._start();
    }

    setSaveText("Code processed!", "color:green;");
  } catch (error) {
    console.error("Error:", error);
    setSaveText("Processing failed.", "color:red;");
    output.innerHTML = "Error running compiled code: " + error.message;
  }
}

function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

async function setSaveText(response, style) {
  // Set save text as response
  save_text.innerHTML = response;
  save_text.style = style;

  // Clear output after 4 seconds
  await sleep(5000);
  save_text.innerHTML = "";
}

async function saveCode() {
  // Set as a variable in local storage
  var codeInput = editor.getValue();
  localStorage.setItem('codeInput', codeInput);
}

async function runCode() {
  if (!wabtModule) {
    console.log('WABT not initialized yet, waiting...');
    await new Promise(resolve => setTimeout(resolve, 1000)); // Wait for 1 second
    if (!wabtModule) {
      setSaveText("WABT initialization failed. Please refresh the page.", "color:red;");
      return;
    }
  }
  // Reset terminal output
  output.innerHTML = "";

  // Convert the data into JSON
  var data = {
    code: localStorage.getItem('codeInput'),
    uuid: localStorage.getItem('uuid')
  };
  var jsonData = JSON.stringify(data);

  // Display saveCode
  setSaveText("Processing your code...", "color:black;");

  try {
    const response = await fetch('https://5dpcfw46rj.execute-api.us-west-1.amazonaws.com/compile', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: jsonData,
    });

    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }

    const watCode = await response.text();
    await displayCode(watCode);
  } catch (error) {
    console.error("Error:", error);
    setSaveText("Processing failed.", "color:red;");
    output.innerHTML = "Error running Artsy compiler. Try again later.";
  }
}

// Register change event handler
editor.getSession().on('change', function() {
  saveCode(); // call the saveCode function whenever the editor content changes
});

$(document).keydown(function(event) {
  if (event.ctrlKey && event.keyCode === 13) {
    runCode();
  }
});

$('#run-button').click(function() {
  runCode();
});
