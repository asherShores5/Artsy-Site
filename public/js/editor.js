// Initialize ace editor
var editor = ace.edit("editor");
editor.setTheme("ace/theme/monokai");
editor.session.setMode("ace/mode/python");
editor.getSession().setUseWorker(false);
var output = document.getElementById("output");
var save_text = document.getElementById("save-text");

// Start code editor
startEditor();

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
}

// Function to run code and display output in output box
async function displayCode(response) {
  // Display Artsy within the output terminal
  var memory;

  // Check if the file name ends with a ".wasm" extension
  if (response.endsWith('.wasm')) {
    try {
      const module = await fetch(response);
      const { instance } = await WebAssembly.instantiateStreaming(module, {
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
      memory = instance.exports.pagememory;
      setSaveText("Code processed!", "color:green;");
    } catch (error) {
      setSaveText("Processing failed.", "color:red;");
      output.innerHTML = error;
    }
  } else if (response.endsWith('_error.txt')) {
    // Display the compiler error to the output terminal
    fetch(response)
      .then(response => response.text())
      .then(data => {
        output.innerHTML = data;
      })
      .catch(error => console.error(error));
    
    setSaveText("Processing failed.", "color:red;");
  } else {
    // Display a default error message to the output terminal
    setSaveText("Processing failed.", "color:red;");
    output.innerHTML = "Error running Artsy compiler. Try again later.";
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

  // Send an AJAX request to process_code.php and execute compiler
  $.ajax({
    url: "/process_code",
    type: "POST",
    contentType: 'application/json',
    data: jsonData,
    success: function(response) {
      displayCode(response);
    },
    error: function(xhr, status, error) {
      console.log("Error: " + error);
    }
  });
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
