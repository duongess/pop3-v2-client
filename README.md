# Network-System: Basic C++ POP3 V2 Client CLI

This project implements a cross-platform **POP3 (Post Office Protocol version 3)** client using C++ and TCP sockets. It is designed to connect to a POP3 server, authenticate users, and synchronize emails using a command-line interface.

## 🚀 Key Features

* **Core Function:** POP3 V2 Client.
* **Networking:** Implements fundamental **TCP/IP sockets** to communicate with mail servers.
* **Build System:** CMake (Modern and Cross-Platform).
* **Data Persistence:** Uses **SQLite** for local data management (storing user profiles and downloaded emails locally).
* **Cross-Platform Support:**
* **Windows:** Automatically links against `Ws2_32`.
* **Linux/macOS:** Fully supported via GCC/Clang with standard socket APIs.



---

## 📂 Project Structure

The project is organized into modules to separate core networking libraries, client application logic, and local state management:

```text
.
├── lib/               # Shared libraries & Core utilities (formerly 'common')
│   ├── include/       # Headers: socket.h, cli.h, utils.h...
│   └── src/           # Implementation: TCP client wrapper, CLI tools
│
├── client/            # Main Client Application
│   ├── include/       # Headers: Client, Account, DBConnection...
│   └── src/           # Client implementation
│       ├── state/     # Local Storage Layer (SQLite Wrappers for Account/Email)
│       ├── client.cpp # Network connection logic
│       └── pop3-v2-cli.cpp # Command processing logic
│
├── sqlite/            # SQLite source files (Vendor code)
├── types/             # Common data type definitions
└── CMakeLists.txt     # Main build configuration

```

---

## 🛠️ Setup and Build

### 1. Prerequisites

* **C++ Compiler:** Must support **C++17** or higher (GCC, Clang, or MSVC).
* **CMake:** Version 3.15 or higher.

### 2. Build Instructions

#### 🐧 Linux (Ubuntu/Debian) & 🍎 macOS

1. **Install Dependencies:**
* **Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential cmake

```


* **macOS (via Homebrew):**
```bash
brew install cmake

```




2. **Build:**
Navigate to the project root and run:
```bash
# Generate build files
cmake -S . -B build

# Compile the project
cmake --build build --config Release

```


3. **Run the Client:**
```bash
./build/client

```



#### 🪟 Windows

1. **Install CMake:**
Using Winget or downloading from the official site:
```powershell
winget install Kitware.CMake

```


2. **Build:**
Open PowerShell or CMD at the project root:
```powershell
# Generate build files (Visual Studio solution or MinGW makefiles)
cmake -S . -B build

# Compile the executable
cmake --build build --config Release

```


3. **Run the Client:**
```powershell
# Path may vary depending on the generator (Release/Debug folders)
.\build\Release\client.exe
# Or if using MinGW/Ninja:
.\build\client.exe

```



---

## 💻 Usage (CLI Commands)

Once the client is running, you can interact with the server using the following commands:

| Command | Usage | Description |
| --- | --- | --- |
| **LOGIN** | `login <host>:<port> <user> <pass>` | Connects to the server at `<IP>` and authenticates with `<user>` and `<pass>`. |
| **SYNC** | `sync` | Retrieves the mail list from the server, compares it with local storage, downloads new emails, and displays the count of new messages. |
| **QUIT** | `quit` | Ends the session and closes the application. |

**Example Session:**

```bash
> login localhost:21 myuser mypassword
[INFO] Server: POP3 server ready
[INFO] User logged in successfully.

> sync
[INFO] 2 new emails downloaded.

> quit

```

---

## 📝 License

This project is open-source. Please refer to the `LICENSE` file in the root directory for more information.