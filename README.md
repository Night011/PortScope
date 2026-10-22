<div align="center">

# PortScope

**A lightweight network connection and port monitor for Windows**  
**Лёгкая утилита для мониторинга сетевых подключений и портов Windows**

**[English](#english) | [Русский](#russian)**

</div>

---

<a id="english"></a>

## English

> **Status: Early development.** PortScope is a learning-driven C++ project. The features described below are planned and may not be available yet.

### About

**PortScope** is an open-source *project in development* for inspecting local TCP connections, UDP endpoints, listening ports, and the processes that own them. Its goal is to make Windows networking information easier to understand through a fast, native desktop interface.

The first version will monitor **only the local Windows PC**. Remote Windows and Linux server monitoring are possible future additions, not current features.

### Planned features

- **Live network overview:** local TCP connections and UDP endpoints.
- **Process details:** process name, PID, and executable path when accessible.
- **Port details:** local/remote IP addresses and ports, plus TCP connection states.
- **Listening ports:** a dedicated view for listening TCP ports.
- **Search and filters:** filter by process, PID, port, protocol, and address.
- **Connection changes:** highlight new and recently disappeared entries.
- **Export:** save snapshots as CSV or JSON.
- **Read-only operation:** inspect connections without changing firewall or network settings.

> UDP endpoints do not have TCP connection states such as `LISTENING` or `ESTABLISHED`.

### Roadmap

| Version | Goal | Status |
| --- | --- | --- |
| v0.1 | Local Windows monitoring, processes, filters | Planned |
| v0.2 | Connection history, notifications, exports | Future |
| v0.3 | Remote Windows monitoring via authenticated agent | Future |
| v0.4 | Remote Linux server monitoring | Future |

### Technology

- **Language:** C++20
- **Operating system:** Windows 10/11 (x64)
- **Interface:** Win32 API / Windows Common Controls
- **Network information:** Windows IP Helper API
- **Build environment:** Microsoft Visual Studio with Desktop development with C++

### Build from source

These steps apply once project source files and a Visual Studio solution are available in the repository.

1. Install Visual Studio with the **Desktop development with C++** workload and a Windows SDK.
2. Clone or download this repository.
3. Open the included `.sln` or `.slnx` file.
4. Select **x64** and **Debug** or **Release**.
5. Build and run the application.

**There is no stable public release yet.**

### Limitations

- PortScope is **not** a packet sniffer and does not decrypt network traffic.
- Some process information may be restricted by Windows permissions.
- The initial version is intended for **local monitoring only**.
- Features and system requirements may change during development.

### Contributing

Issues, ideas, documentation improvements, and code reviews are welcome. For significant changes, please open an issue first.

### License

A license has not been selected yet. Until a license file is added, **all rights are reserved by default**; public source availability does not itself grant permission to reuse the code.

---

<a id="russian"></a>

## Русский

> **Статус: ранняя разработка.** PortScope — учебный проект на C++. Описанные ниже возможности запланированы и пока могут не работать.

### О проекте

**PortScope** — будущая утилита с открытым исходным кодом для просмотра локальных TCP-подключений, UDP-сокетов, прослушиваемых портов и процессов, которым они принадлежат. Цель проекта — показывать сетевую информацию Windows в понятном и быстром нативном интерфейсе.

Первая версия будет следить **только за текущим компьютером под Windows**. Мониторинг удалённых Windows-ПК и Linux-серверов планируется на более поздние версии.

### Планируемые функции

- **Мониторинг сети:** просмотр локальных TCP-подключений и UDP-сокетов в реальном времени.
- **Информация о процессах:** название, PID и путь к исполняемому файлу, если Windows разрешает доступ.
- **Сведения о портах:** локальные и удалённые IP-адреса, порты и состояния TCP.
- **Прослушиваемые порты:** отдельный список TCP-портов, ожидающих подключений.
- **Поиск и фильтры:** по процессу, PID, порту, протоколу и адресу.
- **Изменения подключений:** подсветка новых и недавно исчезнувших записей.
- **Экспорт:** сохранение снимков данных в CSV или JSON.
- **Режим только для чтения:** просмотр без изменения брандмауэра и сетевых настроек.

> UDP-сокеты не имеют таких состояний TCP, как `LISTENING` и `ESTABLISHED`.

### План развития

| Версия | Возможности | Статус |
| --- | --- | --- |
| v0.1 | Локальный мониторинг Windows, процессы, фильтры | Запланировано |
| v0.2 | История подключений, уведомления, экспорт | В будущем |
| v0.3 | Удалённый мониторинг Windows через агент с авторизацией | В будущем |
| v0.4 | Удалённый мониторинг Linux-серверов | В будущем |

### Технологии

- **Язык:** C++20
- **ОС:** Windows 10/11 (x64)
- **Интерфейс:** Win32 API / Windows Common Controls
- **Информация о сети:** Windows IP Helper API
- **Среда разработки:** Microsoft Visual Studio с компонентом «Разработка классических приложений на C++»

### Сборка из исходного кода

Инструкция станет актуальной, когда в репозитории появятся исходники проекта и файл решения Visual Studio.

1. Установите Visual Studio с компонентом **«Разработка классических приложений на C++»** и Windows SDK.
2. Клонируйте репозиторий или скачайте его архив.
3. Откройте файл решения `.sln` или `.slnx`.
4. Выберите конфигурацию **x64** и **Debug** либо **Release**.
5. Соберите и запустите приложение.

**Стабильного публичного релиза пока нет.**

### Ограничения

- PortScope **не перехватывает пакеты** и не расшифровывает сетевой трафик.
- Доступ к сведениям о некоторых процессах может ограничиваться правами Windows.
- Первая версия рассчитана **только на локальный мониторинг**.
- Функции и системные требования могут меняться по мере разработки.

### Участие в разработке

Можно предлагать идеи, сообщать об ошибках, помогать с документацией и проверкой кода. Перед крупными изменениями желательно сначала создать issue.

### Лицензия

Лицензия пока не выбрана. До появления файла лицензии **по умолчанию сохраняются все права автора**: публичная доступность исходников сама по себе не разрешает их свободное использование.

---

<div align="center">

**[Back to English](#english) · [Вернуться к русской версии](#russian)**

</div>
