# Compatibility

*Any statements of compatibility do not imply endorsement by the vendor. Testing has not been performed by the vendor themselves.*

## Three Wire Interface (TWI)

### Key Fill Device (KFD)

#### Adapter Compatibility

A vendor specific adapter or cable is required for most radios.

**The following adapters or cables are tested compatible with KFDtool:**

| Adapter | Device |
| --- | --- |
| KFDtool AC100 + KFDtool AC103 | Motorola R2670A/B Communications System Analyzer |
| KFDtool AC100 + KFDtool AC104 | Kenwood NX-5700/5800/5900 Mobile |
| KFDtool AC100 + KFDtool AC106 | Kenwood NX-5200/5300/5400 Portable |
| KFDtool AC100 + KFDtool AC107 | Motorola XTS4000 Portable |
| KFDtool AC100 + KFDtool AC108 | Aeroflex/IFR 2975 Radio Test Set |
| KFDtool AC100 + Motorola NTN8613 | Motorola XTS Portable (excluding XTS4000) |
| KFDtool AC100 + Motorola HKN6182 | Motorola GCAI Mobile (O CH, M CH, CAN TIB) |
| KFDtool AC100 + Motorola NNTN7869 | Motorola APX Portable |
| KFDtool AC100 + Motorola TRN7414/5880219R01 | Motorola W CH, DB25 TIB |
| KFDtool AC100 + Motorola NTN5664D | Motorola ASTRO Saber Portable |

#### Radio Compatibility

**Tested Compatible (with a passive adapter that does not alter the protocol):**

Motorola:

* APX (MACE) Portable/Mobile
* ASTRO 25 (MACE UCM) Portable/Mobile
* ASTRO 25 (ARMOR UCM) Portable/Mobile
* ASTRO (UCM/EMC Firmware Version 3.XX, Portable Host Firmware Version R07+, Mobile Host Firmware Version R11+) Portable/Mobile

Harris:

* XG-100P Portable
* XG-100M Mobile
* XG-75M/M7300 Mobile

Kenwood:

* NX-5200/5300/5400 Portable
* NX-5700/5800/5900 Mobile

EF Johnson:

* VP6000 Portable
* VP600 Portable
* 5100 Portable
* VM7000 Mobile
* VM900 Mobile
* 5300 Mobile

BK/Relm:

* KNG2 Portable
* KNG Portable

Thales/Racal:

* Liberty/PRC7332 Portable
* 25/PRC6894 Portable

Vertex Standard:

* VX-P820 Portable

Aeroflex/IFR:

* 2975 Radio Test Set

General Dynamics (Motorola Test Equipment Products):

* R2670A/B Communications System Analyzer

**Untested Compatible (with a passive adapter that does not alter the protocol):**

Harris:

* XL-200P Portable
* XG-75P Portable
* XG-25P Portable
* P7300 Portable
* P5500 Portable
* P5400 Portable

Kenwood:

* TK-5210/5310/5410(D) Portable
* TX-5220/5320 Portable
* TK-5710/5810 Mobile
* TK-5720/5820 Mobile

Icom:

* F7000 Portable
* F7500 Mobile
* F70/F80 Portable
* F1721/F2821 Mobile
* F9011/F9021 Portable
* F9511/F9521 Mobile

EF Johnson:

* VP5000 Portable
* VP900 Portable
* VM6000 Mobile
* VM5000 Mobile

BK/Relm:

* BKR 9000 Portable
* KNG Mobile

GME:

* CM60 Mobile

Aeroflex/IFR:

* 3920(B) Radio Test Set

Christine Wireless:

* TKMD

**Untested Compatible (with an active adapter that translates P25 standard 3WI keyload to/from their proprietary protocol):**

Tait:

* TP9400 Portable
* TP9100 Portable
* TM8200/TM8100 Mobile

**Not Compatible:**

Motorola:

* APX1000 Portable (no encryption support)
* APX900 Portable (loaded over USB programming cable in DLI mode)

Harris:

* P7200 Portable (loaded over serial with proprietary software)
* P7100 Portable (loaded over serial with proprietary software)

Unication:

* G4/G5 Pager (loaded over USB with proprietary software)
* U3 Portable (loaded over USB with proprietary software)

Other:

* Radios not supporting encryption (duh)

### Mobile Radio (MR) Emulator

#### Adapter Compatibility

A vendor specific adapter or cable is required for most keyloaders.

**The following adapters or cables are tested compatible with KFDtool:**

| Adapter | Device |
| --- | --- |
| Motorola TKN8531 | Motorola KVL |

#### Keyloader Compatibility

**Tested Compatible (with a passive adapter that does not alter the protocol):**

Motorola:

* KVL5000
* KVL4000
* KVL3000+
* KVL3000

**Untested Compatible (with a passive adapter that does not alter the protocol):**

Icom:

* KVL125

Benelec:

* BKL5000

Christine Wireless:

* TKMD

**Untested Compatible (with an active adapter that translates P25 standard 3WI keyload to/from their proprietary protocol):**

EF Johnson:

* SMA (2nd generation)
* SMA (1st generation)

Tait:

* KFD (2nd generation)
* KFD (1st generation)

**Not Compatible:**

Motorola:

* T series keyloaders

## Data Layer Independent (DLI)

### Key Fill Device (KFD)

#### Radio Compatibility

**Tested Compatible**

Motorola:

* APX900 Portable
