import { promises as fs } from 'fs';
import path from 'path';
import { execFile } from 'child_process';
import { promisify } from 'util';

const execPromise = promisify(execFile);
const UUID_RE = /^[A-Za-z0-9-]{1,64}$/;
const MAX_CODE = 100000;

export const handler = async (event) => {
  let code, uuid;
  try { ({ code, uuid } = JSON.parse(event.body || '{}')); } catch { return { statusCode: 400, body: 'Invalid JSON' }; }
  if (typeof uuid !== 'string' || !UUID_RE.test(uuid) || typeof code !== 'string' || code.length > MAX_CODE) {
    return { statusCode: 400, headers: { 'Content-Type': 'text/plain' }, body: 'Invalid request' };
  }

  const tmpDir = '/tmp';
  const filePath = path.join(tmpDir, `${uuid}.txt`);

  try {
    // Write the code to a file in /tmp
    await fs.writeFile(filePath, code);

    // Execute the Artsy compiler
    const { stdout, stderr } = await execPromise('/var/task/artsy', [uuid], { cwd: tmpDir, timeout: 10000 });
    
    const watFile = path.join(tmpDir, `${uuid}_WATcode.wat`);
    const errorFile = path.join(tmpDir, `${uuid}_error.txt`);

    if (await fs.stat(watFile).catch(() => false)) {
      const watContent = await fs.readFile(watFile, 'utf-8');
      return {
        statusCode: 200,
        headers: {
          'Content-Type': 'text/plain',
          'Content-Disposition': `attachment; filename="${uuid}_WATcode.wat"`
        },
        body: watContent
      };
    } else if (await fs.stat(errorFile).catch(() => false)) {
      const errorContent = await fs.readFile(errorFile, 'utf-8');
      return {
        statusCode: 400,
        headers: { 'Content-Type': 'text/plain' },
        body: errorContent
      };
    } else {
      return {
        statusCode: 500,
        body: JSON.stringify({ status: 'error', message: 'Compilation failed' })
      };
    }
  } catch (error) {
    console.error('Error:', error);
    return {
      statusCode: 500,
      body: JSON.stringify({ status: 'error', message: 'Internal server error' })
    };
  } finally {
    // Clean up temporary files
    await fs.unlink(filePath).catch(() => {});
    await fs.unlink(path.join(tmpDir, `${uuid}_WATcode.wat`)).catch(() => {});
    await fs.unlink(path.join(tmpDir, `${uuid}_error.txt`)).catch(() => {});
  }
};