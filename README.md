# [KFDtool](https://github.com/duggerd/KFDtool)

Open Source P25 Key Fill Device

Compliant with P25 standards (TIA-102.AACD-A)

Download: [latest release](https://github.com/duggerd/KFDtool/releases)

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

**Key Fill Device**

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

Hardware Pricing
----------------

Assembled and tested KFDtool hardware is available from me directly. I can ship internationally. Please email contact@kfdtool.com to place an order.

**Proceeds from hardware sales enables me to further develop the software.**

* KFD100 - single Hirose port USB key fill device (includes 1 m / 3 ft USB A to USB B cable) - $200 USD + shipping
* AC100 - 6 pin male Hirose to 6 pin male Hirose cable (0.5 m / 1.5 ft) - $50 USD + shipping
* AC101 - 6 pin male Hirose pigtail for custom cables (0.5 m / 1.5 ft) - $25 USD + shipping

Radio Compatibility
-------------------

*Any statements of compatibility do not imply endorsement by the vendor. Testing has not been performed by the vendor themselves.*

Radios that are compatible with Motorola KVL3000/KVL3000+/KVL4000/KVL5000 keyloaders in ASTRO 25 mode should be compatible with KFDtool.

A vendor specific adapter or cable is required for most radios. The following adapters or cables are compatible with KFDtool.

* AC100 + Motorola NTN8613 - XTS Portable
* AC100 + Motorola HKN6182 - GCAI Mobile (O CH, M CH, CAN TIB)
* AC100 + Motorola NNTN7869 - APX Portable
* AC100 + Motorola TRN7414 - W CH, DB25 TIB

Keyloading cables made for other radios with MX (Motorola KVL) connectors can be modified by soldering a Hirose connector in parallel with the MX connector. A pigtail with a 6 pin male Hirose connector is available from me as part number AC101.

**Tested Compatible (with a passive adapter that does not alter the protocol):**

Motorola:

* APX (MACE) Portable/Mobile
* ASTRO 25 (MACE UCM) Portable/Mobile
* ASTRO 25 (ARMOR UCM) Portable/Mobile
* ASTRO (UCM SW 3.XX) Portable/Mobile

**Untested Compatible (with a passive adapter that does not alter the protocol):**

Harris:

* XL-200P Portable
* XG-100P Portable
* XG-75P Portable
* XG-25P Portable
* P7300 Portable
* XG-100M Mobile
* XG-75M/M7300 Mobile
* P5500 Portable
* P5400 Portable

Kenwood:

* NX-5x10 Portable
* TK-5x10 Portable
* TX-5x20 Portable
* NX-5x00(B) Mobile
* TK-5x10(G) Mobile
* TK-5x20 Mobile

Icom:

* F7000 Portable
* F7500 Mobile
* F70/F80 Portable
* F1721/F2821 Mobile
* F9011/F9021 Portable
* F9511/F9521 Mobile

EF Johnson:

* VP900 Portable
* VP600 Portable
* VP5000 Portable
* VP6000 Portable
* 5100 Portable

BK/Relm:

* BKR 9000 Portable
* KNG2 Portable
* KNG Portable
* KNG Mobile

GME:

* CM60 Mobile

**Untested Compatible (with an active adapter that translates P25 standard 3WI keyload to/from their proprietary protocol):**

Tait:

* TP9400 Portable
* TP9100 Portable
* TM8200/TM8100 Mobile

**Not Compatible:**

Motorola:

* APX1000 Portable (no encryption support)
* APX900 Portable (software AES loaded over USB with KVL4000/KVL5000)

Harris:

* P7200 Portable (loaded over serial with proprietary software)
* P7100 Portable (loaded over serial with proprietary software)

Unication:

* G4/G5 Pager (loaded over USB with proprietary software)
* U3 Portable (loaded over USB with proprietary software)

Other:

* Radios not supporting encryption (duh)

Contributors
------------

* [Daniel Dugger](https://github.com/duggerd)
* [Matt Ames](https://github.com/mattames)

License / Legal
---------------

KFDtool(TM) software, firmware, and hardware is distributed under the MIT License (see LICENSE.txt).

KFDtool is a trademark of Florida Computer and Networking, Inc.

All product names, trademarks, registered trademarks, logos, and brands are property of their respective owners. All company, product and service names used are for identification purposes only. Use of these names, trademarks, logos, and brands does not imply endorsement.

Note about hardware:

I request that no one else manufactures identical or compatible units **and sells them to others while I am still doing so** - I have put quite a bit of my own money into developing this hardware. I am totally fine with someone making a unit for themselves or a couple of extras to give to their friends, just that they don't charge for them. Proceeds from hardware sales enables me to further develop the software.

Included open-source components:

Software (see doc/SW-LICENSE.txt):

* [NLog](https://github.com/NLog/NLog) - MIT License
* [Mono.Options](https://github.com/mono/mono/blob/master/mcs/class/Mono.Options/Mono.Options/Options.cs) - MIT License
* [HidLibrary](https://github.com/mikeobrien/HidLibrary) - MIT License
* [Microsoft Reference Source](https://github.com/microsoft/referencesource) - MIT License
* [InnovasubBSL430](https://github.com/corentinaltepe/InnovasubBSL430) - GPL v3+ License

Firmware (see doc/FW-LICENSE.txt):

* Texas Instruments - BSD 3 Clause License
