## Dorothy’s Job — Showcase Repository

This repository is a **showcase fork** of *Dorothy’s Job*, created for portfolio and evaluation purposes.

*Dorothy’s Job* is a humorous game developed by **Bola 13 Studios** and **U-tad**, published on **Steam on October 24th, 2024**.

---

## About the Project

*Dorothy’s Job* is a frenetic and comedic isometric twin-stick shooter where players control Dorothy, a muscular maid tasked with cleaning enemy-infested levels using deadly mops, vacuums, and outrageous cleaning tools.

This repository exists to highlight my **technical contributions** to the project, with a focus on **User Interface systems**, **Audio programming**, and their **integration into gameplay**.

---

## My Role & Responsibilities

**Role:** Audio Programmer & User Interface Programmer

### UI Programming & Navigation

- Designed and implemented a **custom C++ focus navigation system** compatible with both **linear and grid-based UI layouts**, featuring:
  - Focus wrapping and focus memory
  - Input-aware focus switching (keyboard/mouse vs. gamepad)
  - Support for multiple UMG widgets (buttons, sliders, checkboxes, etc.)
- Implemented **automatic cursor visibility management** based on the active input device
- Developed **responsive UMG layouts** adaptable to multiple screen resolutions and aspect ratios
- Implemented **C++ binding logic** to ensure seamless communication between UI and gameplay systems

Relevant code:
- [`/Source/.../UI/FocusNavigation`](./Source) — Core focus navigation system  
- [`/Source/.../UI/Widgets`](./Source) — Custom UMG widgets and bindings  

> Folder names may be simplified or curated for showcase purposes.

---

### Audio Programming (FMOD Integration)

- Fully integrated **FMOD** into the project, including:
  - Event-driven audio logic
  - Snapshots
  - Audio settings management
- Designed and implemented a **custom Audio Manager subsystem**, providing:
  - Centralized and global access to FMOD functionality
  - Clean and scalable audio control across gameplay systems

Relevant code:
- [`/Source/.../Audio/AudioManager`](./Source) — Global audio management subsystem  
- [`/Source/.../Audio/FMOD`](./Source) — FMOD integration and helpers  

---

### Gameplay Programming Contributions

- Designed and implemented a **modular consumable (power-up) system**, including:
  - Core architecture and integration with the player character
  - A scalable design allowing easy expansion with new consumable types
- Contributed to **weapon mechanics** and **enemy behavior scripting**, supporting the overall combat design

Relevant code:
- [`/Source/.../Gameplay/Consumables`](./Source)  
- [`/Source/.../Gameplay/Weapons`](./Source)  

---

## Technologies Used

- **Unreal Engine 5** (C++ & Blueprint)
- **FMOD**

---

## Disclaimer

This repository is shared **strictly for educational and portfolio purposes**.

All intellectual property, assets, and rights related to *Dorothy’s Job* belong to **Bola 13 Studios** and **U-tad**.  
No commercial use is intended.