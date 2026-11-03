# artsy-compiler Lambda

Handler for `POST /compile` (API Gateway HTTP API `artsy-api`, us-west-1, Lambda `artsy-compiler`, Node.js 22).
It writes the submitted code to `/tmp/<uuid>.txt`, runs the bundled `artsy` compiler binary, and returns the WAT output.

The deployment zip also contains the static `artsy` binary (built from the C compiler) and `node_modules`; they are not in git.

Security: `uuid` comes from the browser. It must match `^[A-Za-z0-9-]{1,64}$` and is passed to `execFile` as an
argument (no shell), with a 10 s timeout and a 100 KB code limit. An earlier version interpolated `uuid` into an
`exec` shell string, which allowed command injection. Fixed and deployed 2026-11-02.
