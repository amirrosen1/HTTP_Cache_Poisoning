# 🚨 HTTP Cache Poisoning – Exercise 3

This repository contains the solution to **Exercise 3** from the Cybersecurity Lab (67607) at the Hebrew University.

---

## 🧠 Overview

The goal of this exercise is to perform two different cache poisoning attacks via a vulnerable proxy server:

1. **HTTP Request Smuggling**
2. **HTTP Response Splitting**

These attacks demonstrate how inconsistencies between proxies and servers in interpreting HTTP headers can lead to dangerous vulnerabilities.

---

## 📁 Files Included

| File               | Description                                       |
|--------------------|---------------------------------------------------|
| `ex3_splitting.c`  | Performs HTTP Response Splitting                  |
| `ex3_smuggling.c`  | Performs HTTP Request Smuggling                   |
| `explanation.txt`  | Technical explanation of both attacks            |
| `readme.txt`       | Submitter IDs: `207942285,336224076`             |
| `.gitignore`       | Ignores zip, pdf, idea files, and other extras   |

---

## 🧪 How the Attacks Work

### 🔹 HTTP Request Smuggling

- Exploits a mismatch between `Content-Length: 0` and `Transfer-Encoding: chunked`
- Injects a second request (`GET /poison.html`) inside a `POST` body
- Goal: poison the cache of `/page_to_poison.html`

### 🔸 HTTP Response Splitting

- Injects CRLF (`%0D%0A`) characters in headers
- Tricks the proxy into storing a fake HTTP response
- Goal: modify the response of `/67607.html` to reflect attacker-controlled HTML

---

## 🧾 How to Compile and Run

```bash
gcc -o smuggling ex3_smuggling.c
gcc -o splitting ex3_splitting.c

./smuggling
./splitting
