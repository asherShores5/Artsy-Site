        var codeInput = editor.getValue();

        // Convert the data into JSON
        var data = {
          code: codeInput
        };
        var jsonData = JSON.stringify(data);

        // Send an AJAX request to processArtsy.php and execute compiler
        $.ajax({
          url: "save_code.php",
          type: "POST",
          data: jsonData,
          contentType: 'application/json',
          success: function(response) {
            if (useMessage === true) {
              setSaveText("Your progress was saved!", "color:darkgreen");
            }
          },
          error: function(xhr, status, error) {
            if (useMessage === true) {
              setSaveText("Error: Your code was not saved.", "color:red");
            }
          }
        });

              async function runCode() {
        // Reset output of terminal
        output.innerHTML = "";

        // Display saveCode
        setSaveText("Processing your code...", "style:black");

        // Send an AJAX request to process_code.php and execute compiler
        $.ajax({
          url: "process_code.php",
          type: "POST",
          contentType: 'application/json',
          success: function(response) {
            displayCode(response);
          },
          error: function(xhr, status, error) {
            console.log("Error: " + error);
          }
        });
      }