# Contribuir / Contributing

> 🇪🇸 Versión en español abajo · 🇬🇧 English version below.

---

## 🇪🇸 Español

¡Gracias por tu interés en mejorar el Calendario Perpetuo! Las contribuciones son bienvenidas: correcciones de errores, nuevas funciones, mejoras de documentación o traducciones.

### Cómo contribuir

1. Haz un **fork** del repositorio.
2. Crea una rama descriptiva: `git checkout -b fix/descripcion` o `feature/descripcion`.
3. Haz tus cambios con commits claros.
4. Abre un **Pull Request** hacia `main` explicando qué cambia y por qué.

### Compilar y probar

El firmware se puede compilar de dos formas (ver [README](README.md)):

```bash
# PlatformIO
pio run -e standard            # versión estándar
pio run -e color_coded_clock   # versión con reloj
pio run -e standard -t upload  # subir al dispositivo
```

O con Arduino IDE: placa **NodeMCU 1.0**, librería **FastLED**. Detalles en el [Manual Técnico](MANUAL.md#instalación-y-configuración-inicial).

Antes de enviar un PR, verifica que **ambas** versiones (`standard` y `color_coded_clock`) compilen.

### Convenciones de documentación

- El proyecto es **bilingüe**. Los documentos de contenido tienen versión española (`ARCHIVO.md`) e inglesa (`ARCHIVO.en.md`). Si editas uno, actualiza el otro.
- Mantén los enlaces de idioma (badges) al inicio de cada documento.
- Para cambios de comportamiento, actualiza el [CHANGELOG](CHANGELOG.md) y el [Manual Técnico](MANUAL.md) si aplica.
- Respeta el estilo del código existente (indentación, nombres, comentarios).

### Reportar problemas

Abre un **Issue** incluyendo:
- Versión del firmware (Standard / Color Coded Clock) y cómo lo instalaste.
- Placa ESP8266 usada.
- Pasos para reproducir y comportamiento esperado vs. real.
- Logs del Serial Monitor (115200 baud) si es posible.

Antes de abrir un issue, revisa la guía de [Resolución de Problemas](TROUBLESHOOTING.md).

### Licencia

Al contribuir, aceptas que tu trabajo se publique bajo la licencia del proyecto: **CC BY-NC-SA 4.0** (ver [LICENSE](LICENSE)).

---

## 🇬🇧 English

Thanks for your interest in improving the Perpetual Calendar! Contributions are welcome: bug fixes, new features, documentation improvements, or translations.

### How to contribute

1. **Fork** the repository.
2. Create a descriptive branch: `git checkout -b fix/description` or `feature/description`.
3. Make your changes with clear commits.
4. Open a **Pull Request** against `main` explaining what changes and why.

### Build and test

The firmware can be built two ways (see [README](README.en.md)):

```bash
# PlatformIO
pio run -e standard            # standard version
pio run -e color_coded_clock   # clock version
pio run -e standard -t upload  # upload to the device
```

Or with the Arduino IDE: board **NodeMCU 1.0**, library **FastLED**. Details in the [Technical Manual](MANUAL.en.md#installation-and-initial-setup).

Before submitting a PR, verify that **both** versions (`standard` and `color_coded_clock`) compile.

### Documentation conventions

- The project is **bilingual**. Content documents have a Spanish version (`FILE.md`) and an English one (`FILE.en.md`). If you edit one, update the other.
- Keep the language badges at the top of each document.
- For behavior changes, update the [CHANGELOG](CHANGELOG.md) and the [Technical Manual](MANUAL.en.md) if applicable.
- Follow the existing code style (indentation, naming, comments).

### Reporting issues

Open an **Issue** including:
- Firmware version (Standard / Color Coded Clock) and how you installed it.
- ESP8266 board used.
- Steps to reproduce, and expected vs. actual behavior.
- Serial Monitor logs (115200 baud) if possible.

Before opening an issue, check the [Troubleshooting](TROUBLESHOOTING.en.md) guide.

### License

By contributing, you agree that your work is published under the project's license: **CC BY-NC-SA 4.0** (see [LICENSE](LICENSE)).
