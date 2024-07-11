# The Artsy Project: Learn the art of programming

Created by Asher Shores, Andrew Esch, Albert Gonzales, and Karan Sharma

## Project Overview

Artsy is an innovative custom programming language and web-based development environment designed to facilitate learning the art of programming. This project showcases a blend of compiler design, web development, and cloud computing skills.

## Key Components and Technologies

1. **Artsy Compiler**
   - Language: C
   - Tools: Flex (lexical analyzer), Bison (parser generator)
   - Output: WebAssembly Text Format (WAT)

2. **Web Interface**
   - Frontend: HTML, CSS, JavaScript
   - Code Editor: Ace Editor
   - WebAssembly: For executing compiled Artsy code in the browser

3. **Cloud Infrastructure**
   - Hosting: Amazon S3 (static website)
   - Serverless Compute: AWS Lambda
   - API Management: Amazon API Gateway

4. **Development Tools**
   - Version Control: Git
   - Build Tools: Make (for compiler), npm (for web interface)
   - Cloud CLI: AWS CLI

## Technical Skills Demonstrated

- Compiler Design and Implementation
- Language Processing: Lexical Analysis and Parsing
- WebAssembly Generation and Execution
- Frontend Web Development
- Serverless Architecture Design
- Cloud Services Configuration and Deployment
- API Design and Implementation

## Development Environment

### Compiler Development (Ubuntu 20.04 or similar)

Required packages:
```bash
sudo apt-get update
sudo apt-get install make flex bison wabt build-essential gcc
```

## Web Interface Development
Prerequisites:

- Node.js (version 14.x or later)
- AWS CLI

### Deployment Process

1. <strong>Compiler Preparation</strong>

    - Cross-compile for Amazon Linux 2
    - Package with Lambda function code


2. <strong>Web Interface Deployment</strong>

    ```bash
    npm run build
    aws s3 sync ./build s3://artsy-bucket-name --delete
    ```

3. <strong>Lambda Function Update</strong>

    ```bash
    zip -r lambda-package.zip .
    aws lambda update-function-code --function-name artsy-compiler --zip-file fileb://lambda-package.zip
    ```

4. <strong>API Gateway Configuration</strong>

    - Ensure CORS settings are correctly configured
    - Deploy API changes if necessary



### Artsy Language Documentation
For detailed information on the Artsy programming language syntax and features, visit our documentation website.

### Project Status
This project is privately maintained and not open for public contributions. It serves as a demonstration of the team's skills in compiler design, web development, and cloud architecture.