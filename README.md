# 🧠 MedSlice 3D Viewer

An interactive Qt/C++ application for exploring 3D volumetric data through 2D cross-sections.

---

## 📖 Overview

**3D Slice Viewer** visualizes slices of synthetic 3D volumetric data (phantoms) generated programmatically.  
Users can rotate and translate a slicing plane to inspect internal structures in real time.

The app follows a **Model–View–Controller (MVC)** design pattern:

- **Model:** `Volume3D` — defines and stores the 3D voxel data (e.g., spherical phantoms with gradients).
- **View:** `SlicePlane` and rendering classes — display the resulting 2D slice image.
- **Controller:** `MainWindow` — provides user interaction through sliders and buttons.

A small **dark voxel (black dot)** is randomly placed inside the phantom as a gamification feature —  
your goal is to find the dot by exploring different slice orientations and positions!

---

## 🧩 Features

- Generate test 3D phantoms:
  - Radial, X-axis, Y-axis, and Z-axis gradients
- View 2D slices through a 3D volume interactively
- Real-time update of the slice image
- Full control of the slicing plane:
  - **Roll / Pitch / Yaw (°):** rotate around the X, Y, Z axes
  - **Point (x / y / z):** move the plane’s center
- **Reset** button to return to the default view
- **Help dialog** with a quick guide
- Random hidden **black dot** inside the phantom for exploration

---

## ⚙️ Architecture

| Layer | Files | Description |
|-------|--------|-------------|
| **Model** | `volume3d.h/.cpp` | Handles voxel data storage and 3D phantom generation |
| **View** | `sliceplane.h/.cpp`, `reslicer.h/.cpp` | Computes and renders 2D cross-sections |
| **Controller** | `mainwindow.h/.cpp` | Manages user input and updates the view |
| **UI** | `mainwindow.ui`, `HelpDialog.ui` | Graphical user interface built with Qt Designer |
| **Help Module** | `helpdialog.h/.cpp` | Provides an on-screen help window |

---

## 🧮 How It Works

1. **Generate Phantom**  
   Choose a gradient mode (Radial / X / Y / Z) → `Volume3D::makeSpherePhantom()` creates the volume.

2. **Adjust Plane Orientation**  
   Use **Roll**, **Pitch**, and **Yaw** sliders to rotate the slice plane.

3. **Move Through Volume**  
   Use **Point (x/y/z)** sliders to move the plane’s center across the 3D space.

4. **View Cross-Section**  
   The reslicer computes the 2D intersection in real time — brighter regions indicate higher voxel values.

5. **Explore the Volume**  
   Try to find the hidden black dot inside the phantom!

---

## 🕹️ Controls

| Control | Function |
|----------|-----------|
| **Generate Phantom** | Create a 3D test volume (choose gradient mode) |
| **Roll / Pitch / Yaw (°)** | Rotate the slice plane around X, Y, Z |
| **Point (x / y / z)** | Move the slice plane through the volume |
| **Reset** | Restore default position and orientation |
| **Help** | Open the Quick Guide dialog |

---

## 🧱 File Structure

FinalProject/
├── CMakeLists.txt
├── FinalProject/
│ ├── main.cpp
│ ├── mainwindow.h/.cpp/.ui
│ ├── volume3d.h/.cpp
│ ├── sliceplane.h/.cpp
│ ├── reslicer.h/.cpp
│ ├── helpdialog.h/.cpp/.ui
│ ├── rsc.qrc
└── CMake-Module/

---

## 🧩 Build Instructions

### Requirements
- **Qt 6+** (or Qt 5.15+)
- **CMake 3.15+**
- **C++17** compatible compiler (GCC, Clang, MSVC)

### Build (Linux / macOS)
```bash
mkdir build && cd build
cmake ..
make -j
./FinalProject
