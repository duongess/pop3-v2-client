# 📧 POP3 V2 Echo Client CLI (pop3-v2-nhom8)

**A cross-platform POP3 client built with C++ and distributed as an npm CLI.**

The POP3 Client CLI allows users to establish a TCP connection and interact with a POP3 server, demonstrating basic network communication and protocol handling.

---

## 🚀 Key Features

* **Core Function:** POP3 V2 Echo Client (designed to test communication by echoing data).
* **Networking:** Implements fundamental **TCP/IP sockets** for connection to the mail server.
* **Data Persistence:** Uses **SQLite** for local data management (e.g., user profiles and mail storage).
* **Distribution:** Publicly available and installable via **npm** (`pop3-v2-nhom8`).
* **Cross-Platform:** Built using **CMake** for seamless support across Windows, Linux, and macOS.
* **Server DNS:** Configured to connect to `interchange.proxy.rlwy.net:16785`.

---

## 🛠️ Setup and Build

This project uses **CMake** as its build system to ensure cross-platform compatibility.

### 1. Prerequisites

You must have the following tools installed:

* **g++ 15+** (or compatible C++ compiler)
* **CMake** (version 3.10 or higher)

> **Windows Note:** You can install CMake easily using `winget`:
> ```bash
> winget install Kitware.CMake
> ```

### 2. Build Process

Execute the following commands from the project root directory to build the client application:

1.  **Configure Build:** Generate the necessary build files (Makefiles, Visual Studio projects, etc.) in the dedicated `build/` directory.

    ```bash
    cmake -S . -B build
    ```

2.  **Compile Source:** Compile the source code to create the **`client.exe`** executable.

    ```bash
    cmake --build build --config Release
    ```

---

## 💻 Usage

### 1. Running the Compiled Client (Local Testing)

After a successful build, you can run the executable directly from the command line:

```bash
build/client.exe
```
### 2. Running via npm CLI (Public Distribution)
If you have published the package to npm, users can access it instantly using `npx` (Node Package eXecutor):
```bash
npx pop3-v2-nhom8
```

Alternatively, for global installation:
```bash
npm install -g pop3-v2-nhom8
pop3-v2-nhom8
```

### 2. Commands (CLI Interaction)
| Command | Description |
| :--- | :--- |
| `LOGIN <server IP> <username> <password>` | Establishes a connection with the server and sends authentication commands according to the POP3 protocol specification (USER, PASS). |
| `SYNC` | The Client sends a request to the server to retrieve the mail list. The server returns the mail list, including the ID and subject of each email. The Client compares this list with locally stored emails. For new emails, the Client sends a download request, the server returns the mail content and deletes the email, and the Client saves the content to a local file. The Client then displays the number of newly updated emails. |
| `QUIT` | Ends the current session and closes the network connection. |

-----
## 📂 Project Structure
The client-focused project is organized for modularity and clear separation of concerns:

```bash
Client
│
├───client             # Core Client Application Logic
│   ├───include        # Header Files (.h) containing class and function declarations for the Client
│   │       ├── account.h       # Structure/Class declaration for managing user account information and login status
│   │       ├── cli.h           # Declarations for command-line interface handling and parsing
│   │       ├── client.h        # Class/Functions for managing the network connection (TCP Socket)
│   │       ├── db.h            # Basic interface for database interactions
│   │       ├── dbConnection.h  # Declaration of classes/functions managing the physical database connection
│   │       ├── pop3-v2-cli.h   # Core class declaration connecting the POP3 business logic to the CLI
│   │       ├── table.h         # Declarations for data structures mapped to database tables
│   │       └── utils.h         # Utility function declarations specific to the Client module
│   │
│   └───src            # Source Files (.cpp) containing function and class definitions for the Client
│       ├── cli.cpp           # Detailed implementation of the command-line interface logic
│       ├── client.cpp        # Detailed implementation of the network connection and data transfer
│       ├── main.cpp          # Main function, the application's entry point
│       ├── pop3-v2-cli.cpp   # Implementation of the POP3 command execution logic (LOGIN, SYNC, QUIT)
│       ├── utils.cpp         # Implementation of Client-specific utility functions
│       │
│       └───state      # Local Data and State Management Logic
│           ├── account.cpp       # Implementation for storing and retrieving account data
│           ├── db.cpp            # Implementation of basic CRUD operations on the database
│           ├── dbConnection.cpp  # Implementation of database connection setup and management
│           └── table.cpp         # Implementation for mapping and manipulating data objects from/to DB tables
│
├───common             # Shared Code (Used by multiple parts of the project)
│       ├── console.h       # Declaration/Definition for console I/O functions
│       ├── protocol.cpp    # Implementation for handling the POP3 protocol (parsing, message construction)
│       ├── protocol.h      # Declarations for protocol-related constants and structures
│       ├── utils.cpp       # Implementation of general utility functions
│       └── utils.h         # Declaration of general utility functions
│
├───sqlite             # Contains source files or necessary libraries for SQLite integration
│
└───types
```