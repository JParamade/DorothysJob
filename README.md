## About the Project

*Dorothy’s Job* is a frenetic and comedic isometric twin-stick shooter where players control Dorothy, a muscular maid tasked with cleaning enemy-infested levels using deadly mops, vacuums, and outrageous cleaning tools.

This repository exists to highlight my **technical contributions** to the project, with a focus on **User Interface systems**, **audio programming**, and their integration into gameplay.

---

## My Role & Responsibilities

**Role:** UI & Audio Programmer

### UI Programming & Navigation

- Designed and implemented a **custom C++ focus navigation system** compatible with both **linear and grid-based UI layouts**, featuring:
  - Focus wrapping and focus memory
  - Input-aware focus switching (keyboard/mouse vs. gamepad)
  - Support for multiple UMG widgets (buttons, sliders, checkboxes, etc.)
- Implemented **automatic cursor visibility management** based on the active input device
- Developed **responsive UMG layouts** adaptable to multiple screen resolutions and aspect ratios
- Implemented **C++ binding logic** to ensure seamless communication between UI and gameplay systems

Relevant code:
- [`GeneralFocusManager`](./Source/DorothysJob/UI/Focus/GeneralFocusManager.cpp) - Global focus navigation system
- [`SpecificFocusManager`](./Source/DorothysJob/UI/Focus/SpecificFocusManager.cpp) - Per-menu focus management
- [`PlayerController`](./Source/DorothysJob/Controller/BasePlayerController.cpp) - Input-aware UI control logic
- [`DialogueScreen`](./Source/DorothysJob/UI/Menus/Dialogue/DialogueScreen.cpp) - Representative UI implementation

---

### Audio Programming (FMOD Integration)

- Fully integrated **FMOD** into the project, including:
  - Event-driven audio logic
  - Audio settings management
- Designed and implemented a **custom Audio Manager subsystem**, providing:
  - Centralized and global access to FMOD functionality
  - Clean and scalable audio control across gameplay systems

Relevant code:
- [`AudioManager`](./Source/DorothysJob/Manager/GameInstanceSubsystem/AudioManager.cpp) - Global audio management subsystem
- [`BaseWeaponAudioComponent`](./Source/DorothysJob/Component/Weapon/BaseWeaponAudioComponent.cpp) - Weapon audio system
- [`AudioSettingsManager`](./Source/DorothysJob/Settings/AudioSettingsManager.cpp) - Audio configuration handling
- [`FMODAutoDestroyer`](./Source/DorothysJob/Audio/FMODAutoDestroyer.cpp) - FMOD instance handling system

---

### Gameplay Programming Contributions

- Designed and implemented a **modular consumable (power-up) system**, including:
  - Core architecture and integration with the player character
  - Scalable design for easy expansion with new consumable types
- Contributed to **weapon mechanics** and **enemy behavior scripting**, supporting combat design

Relevant code:
- [`Consumables`](./Source/DorothysJob/Consumables)

---

## Technologies Used

- **Unreal Engine 5** (C++ & Blueprint)
- **FMOD**

---

## Disclaimer

This repository is shared **strictly for educational and portfolio purposes**.

All intellectual property, assets, and rights related to *Dorothy’s Job* belong to **Bola 13 Studios** and **U-tad**.  
No commercial use is intended.