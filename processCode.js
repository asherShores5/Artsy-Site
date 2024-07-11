const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');
const path = require('path');
const { exec } = require('child_process');

const app = express();
app.use(bodyParser.json());

app.post('/process_code', (req, res) => {
  const data = req.body;

  if (!data || !data.code || !data.uuid) {
    res.status(400).json({ status: 'error', message: 'Invalid input' });
    return;
  }

  const { code, uuid } = data;
  const dirIn = path.join(__dirname, 'src', 'in');
  const filePath = path.join(dirIn, `${uuid}.txt`);

  if (!fs.existsSync(dirIn)) {
    fs.mkdirSync(dirIn, { recursive: true });
  }

  fs.writeFile(filePath, code, (err) => {
    if (err) {
      res.status(500).json({ status: 'error', message: 'Cannot put code into file' });
      return;
    }

    exec(`make artsy ARGS="${uuid}"`, { cwd: path.join(__dirname, 'src') }, (err, stdout, stderr) => {
      if (err) {
        res.status(500).json({ status: 'error', message: 'Error running make' });
        return;
      }

      const wasmFile = path.join(dirIn, `${uuid}.wasm`);
      const errorFile = path.join(dirIn, `${uuid}_error.txt`);

      if (fs.existsSync(wasmFile)) {
        res.sendFile(wasmFile);
      } else if (fs.existsSync(errorFile)) {
        res.sendFile(errorFile);
      } else {
        res.status(404).json({ status: 'error', message: 'File not found' });
      }
    });
  });
});

app.listen(3000, () => {
  console.log('Server running on port 3000');
});
