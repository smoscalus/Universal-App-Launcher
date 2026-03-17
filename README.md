# 🎓 Universal-App-Launcher

## 📌 Description

Universal App Launcher & Manager — це локальний програмний застосунок для керування програмами, іграми та посиланнями на персональному комп’ютері.
Застосунок працює автономно та не потребує підключення до інтернету або використання сторонніх сервісів.

Проєкт дозволяє об’єднати всі часто використовувані додатки та ресурси в одному місці й керувати ними через зручний веб-інтерфейс, який відкривається у браузері після запуску локального сервера.

Застосунок надає можливість:
- додавати програми, ігри та посилання;
- редагувати та видаляти записи;
- запускати програми або відкривати посилання безпосередньо з інтерфейсу;
- сортувати та групувати елементи за типами.

Дані зберігаються локально у файлі, що дозволяє використовувати застосунок як персональний менеджер без залежності від хмарних сервісів.

## 🧠 Архітектурна особливість

Застосунок побудований на основі розділення відповідальностей:
- WebAssembly (C++) — бізнес-логіка та робота з даними
- Electron (Node.js) — взаємодія з операційною системою
- Frontend (HTML/JS) — інтерфейс користувача

Це дозволяє:
- ізолювати логіку від ОС
- забезпечити безпечну взаємодію
- легко тестувати core-частину

Застосунок надає можливість:
- додавати програми, ігри та посилання;
- редагувати та видаляти записи;
- запускати програми або відкривати посилання;
- сортувати та групувати ресурси;
- запускати групи програм (presets).

Дані зберігаються локально у власному форматі бази даних.

## 🫂 Цільова аудиторія

- 👨‍💻 розробники (швидкий запуск dev-оточення)
- 🎮 геймери (організація ігор та лаунчерів)
- 🧑‍💼 офісні користувачі (робочі пресети)
- 🧠 power users (автоматизація повсякденних задач)

## 🎯 Основні сценарії використання (Use Cases)

### 1. 🚀 Швидкий запуск програм
Користувач відкриває застосунок і запускає програму одним кліком.

### 2. 📦 Використання preset'ів
Користувач створює набір програм:
Work preset:
- VSCode
- Browser
- Slack
→ запускає все одразу

### 3. 🗂 Організація ресурсів
- групування по категоріях
- фільтрація по тегах

## 4. 🔗 Централізація доступу
Всі:
- програми
- файли
- сайти
→ в одному місці


## 🚀 Quick Start

### 📦 Вимоги
- Node.js
- CMake
- Emscripten
- C++ compiler

### 🔧 Збірка WASM
```
cd wasm
emcmake cmake .
cmake --build .
```

### ⚡ Запуск Electron
```
cd electron
npm install
npm start
```

## 🧠 Що відбувається при запуску
- Electron стартує UI
- Завантажується WebAssembly модуль
- Ініціалізується база даних
- UI взаємодіє з WASM через JS bridge

## 📁 Project Structure

```
/project
│
├── /domain                # Бізнес-сутності (Resource, Category, etc.)
├── /application           # Use cases (сервіси)
├── /infrastructure        # Робота з HELL NAH DB
│
├── /wasm                  # Збірка WebAssembly (C++)
│
├── /electron
│   ├── main/              # Electron main process (OS доступ)
│   ├── preload/           # bridge (IPC)
│   └── renderer/          # UI (HTML/JS)
│
├── /web-ui                # фронтенд
├── /hellnah               # база даних
│
└── CMakeLists.txt
```

## 🛠️ Technologies

```
C++ – core logic
WebAssembly – resource processing module
HTML / JavaScript – web interface
Electron – desktop wrapper
```

### Загальна схема

```
[ UI (HTML/JS) ]
        ↓
[ WASM (C++ Core) ]
        ↓
[ JS Bridge ]
        ↓
[ Electron Main ]
        ↓
[ OS ]
```

## 🧩 Ролі компонентів

### 🟦 WebAssembly (C++)
- бізнес-логіка
- управління ресурсами
- робота з базою даних

### 🟨 Electron (Node.js)
- запуск програм (.exe, .app)
- доступ до файлової системи
- системні виклики

### 🟩 Frontend
- UI
- виклик use cases через WASM

# 📊 Data Models

## User
```
{
  "id": "int",
  "name": "string",
  "avatarUrl": "string",
  "createdAt": "time"
}
```
Користувач застосунку.
У поточній версії застосунок передбачає одного локального користувача.

## Resource
```
{
  "id": "int",
  "name": "string",
  "description": "string",
  "path": "string",
  "userId": "int",
  "categoryId": "int"
}
```
Описує ресурс, який може бути запущений або відкритий через лаунчер.
Ресурсом може бути:
- програма
- гра
- веб-посилання
- файл

## Category
```
{
  "id": "int",
  "name": "string",
  "description": "string",
  "userId": "int"
}
```
Категорія використовується для групування ресурсів.
Приклади:
- Games
- Development
- Work
- Browsers
Один ресурс належить одній категорії.

## Tag
```
{
  "id": "int",
  "name": "string",
  "description": "string"
}
```
Теги використовуються для гнучкої класифікації ресурсів.
Приклади тегів:
- game
- launcher
- dev
- communication
- browser

## Preset
```
{
  "id": "int",
  "name": "string",
  "description": "string",
  "userId": "int"
}
```
Preset — це набір ресурсів, які можуть запускатися разом.
Приклади:
Work preset:
- VSCode
- Chrome
- Slack
Gaming preset:
- Steam
- Discord
- Spotify

## ResourceTag
```
{
  "id": "int",
  "resourceId": "int",
  "tagId": "int"
}
```
ResourceTag є службовою таблицею, яка використовується для реалізації
зв'язку many-to-many між Resource та Tag.

## PresetResource
```
{
  "id": "int",
  "presetId": "int",
  "resourceId": "int"
}
```
PresetResource використовується для зв'язку між Preset та Resource,
що дозволяє одному пресету містити декілька ресурсів.

## Relationships
```
User
 ├── Resource
 ├── Category
 └── Preset

Resource
 ├── Category (many-to-one)
 ├── ResourceTag (many-to-many)
 └── PresetResource (many-to-many)

Tag
 └── ResourceTag

Preset
 └── PresetResource
```
