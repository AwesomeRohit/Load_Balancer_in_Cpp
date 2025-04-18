# ⚖️ C++ Load Balancer

A lightweight and efficient Load Balancer built in C++ with support for:
- Round-robin load distribution
- Health checks
- Configurable backends via JSON
- Thread-safe operations using mutex
- Automatic config reload

---

## 📋 Features

✅ Load balancing using **Round-Robin algorithm**  
✅ Health checks for backend servers (with failure/success thresholds)  
✅ Automatic **JSON config reloading** without restarting the service  
✅ Thread-safe backend server handling using **mutex**  
✅ Concurrent client handling using **std::thread**  
✅ Forwarding data between client and backend bi-directionally  

---

## 📁 Project Structure

```
.
├── main.cpp                  # Entry point
├── servers.hpp              # Backend server structure
├── HealthCheck.hpp          # Health check logic
├── forwarding.hpp           # Client ↔ Backend forwarding
├── configreader.hpp         # JSON config parsing
├── config.json              # Backend server configuration
├── README.md                # Project documentation
```

---

## 🛠️ Prerequisites

- **Windows OS** (uses Winsock2)
- **Visual Studio** with C++ support
- **vcpkg** (for installing dependencies like nlohmann/json)

### 🔗 Dependencies

Install [nlohmann/json](https://github.com/nlohmann/json) via `vcpkg`:

```bash
vcpkg install nlohmann-json
```

Include it in your project via:

```cpp
#include <nlohmann/json.hpp>
using json = nlohmann::json;
```

---

## ⚙️ Configuration (`config.json`)

```json
[
  { "ip": "127.0.0.1", "port": 4000 },
  { "ip": "127.0.0.1", "port": 4001 },
  { "ip": "127.0.0.1", "port": 4002 }
]
```

- Add your backend server IPs and ports here.
- The config file is **automatically reloaded** when modified.

---

## 🚀 Getting Started

1. Clone the repo:

```bash
git clone https://github.com/yourusername/cpp-load-balancer.git
cd cpp-load-balancer
```

2. Open the project in Visual Studio.

3. Make sure your backend servers (on ports like `4000`, `4001`, etc.) are up.

4. Build and run `main.cpp`. The load balancer will start listening on port `8080`.

---

## 📦 Future Improvements

- Logging to file (with log levels)
- Monitoring dashboard (stats, health)
- Retry logic & connection pooling
- Support for HTTPS and SSL

---

## 📄 License

MIT License. Free to use and modify.

---

## 🙋‍♂️ Author

**Rohit Suthar**  
_Developer | Backend Enthusiast | MCA Student_  
📧 [Email](mailto:rohit.suthar.1935@gmail.com)  
🔗 [LinkedIn](https://www.linkedin.com/in/rohit-suthar-8a3449214/)
