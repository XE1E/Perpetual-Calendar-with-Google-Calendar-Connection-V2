# Lista de Materiales (BOM)

[![English](https://img.shields.io/badge/lang-English-blue.svg)](BILL_OF_MATERIALS.en.md)
[![Español](https://img.shields.io/badge/lang-Español-green.svg)](BILL_OF_MATERIALS.md)

Lista completa de componentes, materiales, piezas impresas y herramientas necesarias para construir el Calendario Perpetuo.

> Para los pasos de montaje paso a paso, ver la [Guía de Ensamblaje](ASSEMBLY.md). Para la guía completa con imágenes, ver la [Guía Completa](https://xe1e.github.io/Perpetual-Calendar-with-Google-Calendar-Connection-V2/GUIA_COMPLETA.html).

---

## Componentes Electrónicos

| Componente | Cantidad | Especificación / Notas |
|------------|:--------:|------------------------|
| Microcontrolador ESP8266 | 1 | Wemos D1 Mini (recomendado), NodeMCU u otro compatible |
| Tira LED WS2811 / WS2812B | 75 LEDs | 60 LEDs/m, direccionable, 5V. Se corta en tiras (ver Ensamblaje) |
| Fuente de alimentación 5V | 1 | Mínimo **3 A** (75 LEDs × 60 mA máx ≈ 4.5 A en pico blanco; 3 A suficiente en uso normal) |
| Conector DC hembra | 1 | 5.5 mm / 2.5 mm, para la entrada de la fuente |
| Módulo regulador 5V → 3.3V | 1 | Solo si tu placa ESP8266 no integra regulador (el D1 Mini ya lo trae) |
| Resistencia 3 kΩ | 1 | **Obligatoria** — entre GPIO2 (pin de datos) y VCC. Evita arranques fallidos |
| Cables de colores | Varios | Para las conexiones de datos y alimentación |
| Estaño de soldadura | — | Para unir las tiras LED y los cables |

> ⚠️ **Resistencia de 3 kΩ:** Es necesaria para que el ESP8266 arranque de forma fiable con la tira LED conectada al pin de datos (GPIO2 / D4). Sin ella el módulo puede no iniciar.

> ⚡ **Alimentación:** NO alimentes los 75 LEDs desde el ESP8266. Usa una fuente externa de 5V/3A y une el **GND común** entre la fuente, la tira LED y el ESP8266.

---

## Materiales de Construcción

| Material | Especificación |
|----------|----------------|
| Piezas impresas en 3D | Marco frontal, soporte de LEDs, rejilla separadora y pata/tapa trasera (ver abajo) |
| Acrílico ahumado | 191 mm × 96 mm × 2 mm de grosor (difusor frontal) |
| Papel blanco impreso | Plantilla con los números de días y nombres de meses ([`plantilla_numeros.pdf`](docs/plantilla_numeros.pdf), imprimir en A4) |
| Pegamento caliente | Para fijar el ESP, el regulador y los componentes |
| Cinta adhesiva de espuma | 1 mm de grosor, para la pata trasera |

---

## Piezas Impresas en 3D

Todas las piezas vienen en un único archivo STL:

- 📥 [`docs/stl/3D_Printed_Perpetual_Calendar.stl`](docs/stl/3D_Printed_Perpetual_Calendar.stl) (~7.3 MB) — listo para imprimir
- ✏️ [Ver / editar en Tinkercad](https://www.tinkercad.com/things/52D6lIWCsAP-3d-printed-perpetual-calendar) — para modificar el diseño

| Pieza | Función |
|-------|---------|
| Soporte de LEDs | Base con relieves que posicionan las tiras LED a ~12 mm |
| Rejilla separadora | Aísla la luz de cada LED. Hay **dos versiones**: con y sin separaciones para los 6 LEDs del reloj |
| Marco frontal | Fija el conjunto y enmarca el difusor de acrílico |
| Pata / tapa trasera | Soporte para apoyar el calendario |

> 💡 **Versión del reloj:** Si vas a instalar el firmware *Color Coded Clock*, usa la rejilla **con** separaciones para los LEDs 62–67. Para la versión *Standard*, cualquiera de las dos sirve.

---

## Herramientas

- Soldador y estaño
- Pistola de pegamento caliente
- Lija (para redondear las esquinas del acrílico)
- Impresora 3D (o acceso a un servicio de impresión)
- Cable USB con datos (no solo de carga) para programar el ESP8266

---

## Resumen Rápido de Costos *(orientativo)*

Los precios varían por región y proveedor. Esto es solo una referencia aproximada de las piezas principales:

| Categoría | Ítems |
|-----------|-------|
| Electrónica | ESP8266, tira LED, fuente 5V/3A, conector DC, resistencia, cables |
| Impresión 3D | ~4 piezas (filamento PLA recomendado) |
| Difusor | Acrílico ahumado 2 mm |
| Consumibles | Papel, pegamento caliente, cinta de espuma, estaño |

---

*Lista de materiales del proyecto Perpetual Calendar with Google Calendar Connection v2.2*
