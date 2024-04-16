# Tremor-Detector
Parkinson's disease is one of the most common conditions for elderly patients in the US and across the world. The symptoms usually start with a tremor followed by another such as slow movement and loss of balance. The aim of this project is to develop a wearable device that detects resting tremors, providing an accurate and early diagnosis to facilitate timely and effective treatment.

Resting tremors in Parkinson’s disease are primarily observed in the hand or wrist when the affected limb is at rest, with tremor frequencies ranging from 3 to 6 Hz. These tremors occur at a frequency low enough to be captured by the built-in gyroscope of an embedded system. Our objective is to distinguish resting tremors associated with Parkinson’s disease from normal movements using only the gyroscope, without additional hardware. This involves capturing time segments from the gyroscope, analyzing the data, and providing visual feedback on the presence and intensity of resting tremors.

## Hardware
STM32F429 Discovery Board with built in gyroscope

Power supply

(no additional hardware allowed as a constraint for the project)

## Environment
PlatformIO

C, C++
