
<p align="center">
  <img src="Docs/header.png"/>
</p>

# 🎥 Omnia Camera System

An Extended and Modular Plugin based version of Lyra Camera for Unreal Engine 5, Blueprint Friendly And Can be used without GAS.  

## 💾 Install

To Install simply place the OmniaCamera Folder in your **"ProjectDirectory/Pluigns"** folder.  
Check if the Plugin is Enabled In your Project Plugin window:  
<p align="center">
  <img src="Docs/PluginsWindow.png"/>
</p>
  

## 🧩 Features

- Configurable Blending between Camera Modes.  
- Blueprint Spawnable Component.  
- Modular and Decoupled from GAS and Lyra related Classes.  
- Utility Functions to Clear and Swap out Camera Modes.  
- You can extend the **"OmniaCameraMode"** Class to make your Own Camera Modes Just by Following Any Lyra Camera Tutorial.  

## 🎞️ Camera Modes

Currently This Plugin comes with 3 Camera Modes:  
- First Person  
- Third Person  
- Death Camera  

There are two Vector Curves that control the Positions of Camera Veiw in Third Person and Death Camera.  

## 🖥️ Setup

- First Add the Omnia Camera Component to your Character or Pawn **(No Need to Add Spring Arm)**.  
<p align="center">
  <img src="Docs/OmniaCameraComp.png"/>
</p>  

- You can also set the default Camera Mode inside the Omnia Camera Component:  
<p align="center">
  <img src="Docs/DefaultCamera.png"/>
</p>  

- To Change the Camera Mode Simply Call this function using the Omnia Camera Component Reference:  
<p align="center">
  <img src="Docs/SetCameraMode.png"/>
</p>  
