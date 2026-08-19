# UniRTOS LVGL Demos

ST7796S (320x480) LVGL demo for EG800ZCN_LA.  The application owns only the
panel driver, pin mapping, display port and UI; `unirtos-lvgl` is pulled and
compiled as an independent UniRTOS external library.

## Requirements

- EG800ZCN_LA UniRTOS SDK `v1.0.4`.
- An EG800ZCN_LA `gccout.7z` with LCD support must replace the SDK's base
  firmware archive before building this demo. The validated archive is
  14,181,489 bytes with SHA-256
  `FED45CB25E18ECB7311524651D58C6EEE580A841C73A8ECEC70410F03EC19421`.
- ST7796S wired to LSPI: reset GPIO15, LSPI pins GPIO49/50/51/52/53.

## Build

```powershell
unirtos-cli env-setup
unirtos-cli build
```

The project declares `unirtos-lvgl` version `8.3.11` in `env_config.json`.
`env-setup` retrieves that library; normal projects that do not declare it do
not compile or link LVGL. The `v1.0.4-p1` directory used during initial board
bring-up was a local patched SDK copy and is deliberately not a public demo
dependency.

The first screen contains a title, status label, progress bar, button and RGB
blocks.  It uses a 320x40 RGB565 DMA-visible draw buffer and continuously
refreshes the progress value.
