# [KFDtool](https://github.com/KFDtool/KFDtool)

Open Source P25 Key Fill Device

Compliant with P25 standards (TIA-102.AACD-A)

Purchase Hardware: [online store](https://kfdtool.com/store)

Download Software: [latest release](https://github.com/KFDtool/KFDtool/releases)

Demonstration: [video](https://www.youtube.com/watch?v=Oioa3xTQoE0)

Disclaimer
----------

The KFDtool as a computer peripheral has several important considerations to keep in mind when secure keyloading is required:

* The following points are only valid with unmodified software, firmware, and hardware. With physical access to the PC or adapter, the software, firmware, or hardware could be modified to covertly retain the plaintext keying material.

* Plaintext keying material is present in the PC's RAM, over the USB connection, in the adapter's RAM, and over the keyload connection. Therefore, you must trust the PC that the software is running on, or air gap it.

* It is possible that plaintext keying material in the PC's RAM is paged out to disk. It is also possible that Windows crash dumps may contain plaintext keying material. Therefore, it is recommended that the PC's hard drive is protected using full disk encryption such as BitLocker and powered off when unattended.

* After the KFDtool adapter has been disconnected from the USB port, any residual plaintext keying material present in the microcontroller's RAM will be lost.

* When certain non-default logging is enabled, plaintext keying material is written out to the log file on disk. This logging should only be enabled when diagnostic information needs to be collected, and only used with dummy keying material.

Features
--------

**Key Fill Device (KFD)**

Supported Manual Rekeying Features (TIA-102.AACD-A section 2.3)

* 2.3.1 Keyload
* 2.3.2 Key Erase
* 2.3.3 Erase All Keys
* 2.3.4 View Key Info

Motorola refers to the P25 standard 3 wire interface (3WI) keyload protocol as ASTRO 25 mode or CKR mode.

The legacy Motorola proprietary keyloading formats SECURENET and ASN (Advanced SECURENET) are **NOT** supported by KFDtool. PID mode is also used to refer to ASN mode.

Key validators/generators are available for the following algorithms:

* AES-256 (Algorithm ID 0x84)
* DES-OFB (Algorithm ID 0x81)
* DES-XL (Algorithm ID 0x9F)
* ADP/RC4 (Algorithm ID 0xAA)

**Mobile Radio (MR) Emulator**

Supported Manual Rekeying Features (TIA-102.AACD-A section 2.3)

* 2.3.1 Keyload

Operations encapsulated with encryption are not supported at this time.

Radio Compatibility
-------------------

*Any statements of compatibility do not imply endorsement by the vendor. Testing has not been performed by the vendor themselves.*

**A detailed list of compatible radios and adapters is available [here](doc/RADIO_COMPATIBILITY.md).**

Radios that are compatible with Motorola KVL3000/KVL3000+/KVL4000/KVL5000 keyloaders in ASTRO 25 mode should be compatible with KFDtool.

Keyloading cables made for other radios with MX (Motorola KVL) connectors can be modified by soldering an AC101 or AC102 Hirose pigtail in parallel with the MX connector according to [these](doc/MX_CONN_MOD_NOTES.md) instructions.

Hardware
--------

Assembled and tested KFDtool hardware is available from me directly. I can ship internationally. Please visit the [online store](https://kfdtool.com/store) to place an order.

**Proceeds from hardware sales enables me to further develop the software.**

| Part Number | Description |
| :---: | --- |
| KFD100 | Single Hirose port USB key fill device (includes 1 m / 3 ft USB A to USB B cable) |
| AC100 | 6 pin male plug Hirose to 6 pin male plug Hirose cable (0.5 m / 1.5 ft) |
| AC101 | 6 pin male plug Hirose pigtail for custom cables (0.5 m / 1.5 ft) |
| AC102 | 6 pin female jack Hirose pigtail for custom cables (0.5 m / 1.5 ft) |
| AC103 | Motorola R2670 compatible adapter, requires AC100 (0.15 m / 6 in) |
| AC104 | Kenwood KPG-115 NX-5700/5800/5900 compatible adapter, requires AC100 (0.15 m / 6 in) |
| AC105 | 4 way female jack passive Hirose splitter |
| AC106 | Kenwood KPG-93 NX-5200/5300/5400 compatible adapter, requires AC100 (0.15 m / 6 in) |

OS Compatibility
----------------

* KFDtool software supports 32-bit and 64-bit Windows 7, Windows 8.1, and Windows 10

* The .NET Framework 4.7.2 or later compatible must be installed

* **The use of a virtual machine with USB passthrough is NOT supported at this time**
    * Changing the USB controller from USB 2.0 mode to USB 3.0 mode has been reported to resolve the issue
    * Do not attempt to update the adapter firmware or initialize an adapter using USB passthrough

Documentation
-------------

* [Software Changelog](doc/SW_CHANGELOG.txt)
* [Firmware Changelog](doc/FW_CHANGELOG.txt)
* [Hardware Changelog](doc/HW_CHANGELOG.txt)
* [TWI Cable Assembly Notes (AC100, AC101, AC102, AC103, AC104, AC106)](doc/TWI_CABLE_ASSY_NOTES.md)
* [MX Connector Modification Notes (AC101, AC102)](doc/MX_CONN_MOD_NOTES.md)
* [Developer Notes](doc/DEV_NOTES.md)

Contributors
------------

* [Daniel Dugger](https://github.com/duggerd)
* [Matt Ames](https://github.com/mattames)

License / Legal
---------------

KFDtool software, firmware, and hardware is distributed under the MIT License (see [LICENSE.txt](LICENSE.txt)).

KFDtool is a trademark of Florida Computer and Networking, Inc.

All product names, trademarks, registered trademarks, logos, and brands are property of their respective owners. All company, product, and service names used are for identification purposes only. Use of these names, trademarks, logos, and brands does not imply endorsement.

Note about hardware:

I request that no one else manufactures identical or compatible units **and sells them to others while I am still doing so** - I have put quite a bit of my own money into developing this hardware. I am totally fine with someone making a unit for themselves or a couple of extras to give to their friends, just that they don't charge for them. Proceeds from hardware sales enables me to further develop the software.

Included open-source components:

Software (see [doc/SW_LICENSE.txt](doc/SW_LICENSE.txt)):

* [NLog](https://github.com/NLog/NLog) - MIT License
* [Mono.Options](https://github.com/mono/mono/blob/master/mcs/class/Mono.Options/Mono.Options/Options.cs) - MIT License
* [HidLibrary](https://github.com/mikeobrien/HidLibrary) - MIT License
* [Microsoft Reference Source](https://github.com/microsoft/referencesource) - MIT License
* [InnovasubBSL430](https://github.com/corentinaltepe/InnovasubBSL430) - GPL v3+ License
* Texas Instruments - BSD 3 Clause License

Firmware (see [doc/FW_LICENSE.txt](doc/FW_LICENSE.txt)):

* Texas Instruments - BSD 3 Clause License
