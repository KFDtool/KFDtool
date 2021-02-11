# [KFDtool](https://github.com/KFDtool/KFDtool)

Open Source P25 Key Fill Device

Compliant with P25 standards (TIA-102.AACD-A)

Purchase Hardware: [online store](https://kfdtool.com/store)

Download Software: [latest release](https://github.com/KFDtool/KFDtool/releases)

Release Notifications: [subscribe](https://kfdtool.com/newsletter)

Demonstration: [video](https://www.youtube.com/watch?v=Oioa3xTQoE0)

Software Manual: [view](doc/KFDtool_Manual.pdf)

Security Considerations: [view](doc/SECURITY_CONSIDERATIONS.md)

Features
--------

**Key Fill Device (KFD)**

The KFDtool software supports KFD features through the KFDtool hardware adapter (TWI/3WI/Three Wire Interface), as well as through a IP (UDP) connection (DLI/Data Link Independent interface).

Keys and groups of keys can be saved to an AES-256 encrypted key container file, which can then be selected and loaded into a target device in one operation.

Supported Manual Rekeying Features (TIA-102.AACD-A)

* 2.3.1 Keyload
* 2.3.2 Key Erase
* 2.3.3 Erase All Keys
* 2.3.4 View Key Info
* 2.3.5 View Individual RSI
* 2.3.6 Load Individual RSI
* 2.3.7 View KMF RSI
* 2.3.8 Load KMF RSI
* 2.3.9 View MNP
* 2.3.10 Load MNP
* 2.3.11 View Keyset Info
* 2.3.12 Activate Keyset

Motorola refers to the P25 standard 3 wire interface (3WI) keyload protocol as ASTRO 25 mode or CKR mode.

The legacy Motorola proprietary keyloading formats SECURENET and ASN (Advanced SECURENET) are **NOT** supported by KFDtool. PID mode is also used to refer to ASN mode.

Key validators/generators are available for the following algorithms:

* AES-256 (Algorithm ID 0x84)
* DES-OFB (Algorithm ID 0x81)
* DES-XL (Algorithm ID 0x9F)
* ADP/RC4 (Algorithm ID 0xAA)

**Mobile Radio (MR) Emulator**

The KFDtool software only supports MR Emulator features through the KFDtool hardware adapter (TWI/3WI/Three Wire Interface) at this time.

This mode allows another keyloader to be connected to the KFDtool, and the keys retrieved.

Supported Manual Rekeying Features (TIA-102.AACD-A)

* 2.3.1 Keyload

Radio Compatibility
-------------------

*Any statements of compatibility do not imply endorsement by the vendor. Testing has not been performed by the vendor themselves.*

**A detailed list of compatible radios and adapters is available [here](doc/RADIO_COMPATIBILITY.md).**

Radios that are compatible with Motorola KVL3000/KVL3000+/KVL4000/KVL5000 keyloaders in ASTRO 25 mode should be compatible with KFDtool.

Keyloading cables made for other radios with MX (Motorola KVL) connectors can be modified by soldering an AC101 or AC102 Hirose pigtail in parallel with the MX connector according to [these](doc/MX_CONN_MOD_NOTES.md) instructions.

Operations encapsulated with encryption (commonly referred to as FIPS mode) are not supported at this time for either the KFD or MR emulation modes.

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
| AC103 | Motorola R2670 compatible adapter (0.15 m / 6 in) |
| AC104 | Kenwood KPG-115 compatible adapter (0.15 m / 6 in) |
| AC105 | 4 way female jack passive Hirose splitter |
| AC106 | Kenwood KPG-93 compatible adapter (0.15 m / 6 in) |
| AC107 | Motorola XTS4000 compatible adapter (0.15 m / 6 in) |
| AC108 | Aeroflex/IFR 2975 compatible adapter (0.15 m / 6 in) |
| AC109 | Harris XG-100P/XL-150P/XL-185P/XL-200P compatible adapter |

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
* [Radio Compatibility](doc/RADIO_COMPATIBILITY.md)
* [TWI Cable Assembly Notes](doc/TWI_CABLE_ASSY_NOTES.md)
* [MX Connector Modification Notes](doc/MX_CONN_MOD_NOTES.md)
* [Developer Notes](doc/DEV_NOTES.md)
* [Security Considerations](doc/SECURITY_CONSIDERATIONS.md)

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
