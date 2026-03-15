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

## 🫂 Цільова аудиторія
```
WIP...
```

## 🎯 Основні сценарії використання (Use Cases)
```
WIP...
```

## 🚀 Quick Start
```
WIP...
```

## 📁 Project Structure
```
WIP...
```

## 🛠️ Technologies
```
C++ – core logic
WebAssembly – resource processing module
HTML / JavaScript – web interface
Electron – desktop wrapper
```

## ⚙️ Configuration
```
WIP...
```

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
