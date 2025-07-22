# Console Notepad 📓

A feature-rich terminal notepad written in C++ for Windows, supporting file I/O, undo/redo, word & sentence completion, live search, and a trie-based autocomplete engine.
This project was made from scratch, all classes and data structures were made manually to get a good grip on data structures concepts. 

---

## Features

- **Create / Load / Save** plain-text files via a simple menu
- **Real-time editing** on a dynamically-rendered console grid with cursor movement (arrow keys)
- **Undo / Redo** up to 5 operations (Ctrl+Z / Ctrl+Y) using custom stacks
- **Word Completion** (`@`) powered by an N‑ary trie that learns as you type
- **Sentence Completion** (`Tab`) via a second graph structure for phrase suggestions
- **Live Find** (Ctrl+F) to locate and highlight occurrences with line numbers
- **Graceful word‑wrap** and automatic line‑shifting when rows fill up

---

## Prerequisites

- **Windows 10+** with console supporting `<Windows.h>`
- **C++17** (or later) compiler (MSVC recommended)
- Standard C++ library and `<filesystem>` support

---

## Building

```bash
cl /std:c++17 Notepad.cpp /Fe:Notepad.exe
```

---

## Running

```bash
Notepad.exe
```

1. **Main Menu**  
   - **1**: Create a new file  
   - **2**: Save current file  
   - **3**: Load existing file  
   - **4**: Exit  

2. **Editing Mode**  
   - **Arrow Keys**: Move cursor  
   - **Typing**: Insert characters (auto‑wraps & reflows)  
   - **Backspace**: Delete (with undo batching)
   - **Enter**: Newline

3. **Shortcuts & Special Commands**  
   - **Ctrl+Z**: Undo last edit  
   - **Ctrl+Y**: Redo last undo  
   - **Ctrl+F**: Find mode (type term + Enter)  
   - **@**: Trigger word completion on current token  
   - **Tab**: Trigger sentence/phrase suggestions  
   - **Esc**: Return to Main Menu  

---

## Internal Data Structures

- **`Node` grid**: Doubly‑linked nodes per character for 2D text layout  
- **`stack<Entry>`**: Custom undo/redo entries storing insertion/deletion batches  
- **`NAryTree`**: 26‑branch trie storing pointers back to `Node` locations for live autocomplete  
- **`Graph`**: Linked‑tree for sentence‑level suggestions based on previous words  

---

## Contributing

1. Fork the repo  
2. Submit issues or pull requests  
3. Follow coding standards and document new features  

---
