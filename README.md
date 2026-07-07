# PlaceholderApi

A Minecraft Bedrock server mod for LeviLamina that helps developers support multilingualism in their mods. It intercepts outbound packets (like text messages, titles, toast requests, actor names, and forms) and replaces custom placeholders with locale-specific translations before the packets reach the clients.

## How it works

The API generates a unique placeholder key based on an identifier. You register translations for different locales under this key. When the server sends a packet containing the key, the mod detects the recipient's language, matches it with your registered translations, and sends the localized packet to that player.

## Usage (C++ API)

Include `<placeholder_api/Api.h>` in your project.

### 1. Persistent Placeholders

```cpp
#include <placeholder_api/Api.h>

// 1. Generate a key (e.g. "my_prefix:some_hash")
std::string key = placeholder::api::generatePlaceholder("my_mod.welcome_message");

// 2. Set translations for different locales
placeholder::api::setPlaceholder(key, "Welcome to the server!", "en_US");
placeholder::api::setPlaceholder(key, "Добро пожаловать на сервер!", "ru_RU");

// 3. Use the generated key in any packet (e.g. chat messages, titles, forms)
player.sendMessage(key);
```

### 2. Temporary Placeholders

Useful for temporary values (like UI status or short-lived text) that you want cleaned up automatically.

```cpp
std::string tempKey = placeholder::api::generateTemporaryPlaceholder();

placeholder::api::setTemporaryPlaceholder(tempKey, "Active", "en_US");
placeholder::api::setTemporaryPlaceholder(tempKey, "Активен", "ru_RU");
```

## Supported Packets

The mod automatically scans and replaces placeholders in:
- `TextPacket` (messages)
- `SetTitlePacket` (titles/subtitles)
- `ToastRequestPacket` (toast notifications)
- `ModalFormRequestPacket` (custom form JSONs)
- `AvailableCommandsPacket` (command descriptions)
- `AddPlayerPacket` & `AddActorPacket` & `SetActorDataPacket` (actor metadata/name tags)
