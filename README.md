

# IPK Project 1 – OMEGA: L4 Scanner

## Popis

Tento program je jednoduchý síťový skener na 4. vrstvě (L4), který umožňuje skenovat zadané TCP a UDP porty cílové IP adresy nebo hostname. Podporuje jak IPv4, tak IPv6. Program umí určit, zda je port `open`, `closed`, nebo `filtered`, a funguje jak pro TCP, tak UDP provoz.

## Sestavení

Program lze zkompilovat pomocí příkazu:

```bash
make
```

Pro vyčištění sestavení (smazání `.o` souborů a binárky):

```bash
make clean
```

## Spuštění

```bash
./ipk-l4-scan [-i interface] [--pt tcp-porty] [--pu udp-porty] [-w timeout] [hostname|IP]
```

| Argument         | Popis                                                                 |
|------------------|------------------------------------------------------------------------|
| `-i`, `--interface` | Určí síťové rozhraní pro skenování                                 |
| `-t`, `--pt`         | TCP porty ke skenování (např. `22,80`, `20-25`)                    |
| `-u`, `--pu`         | UDP porty ke skenování                                              |
| `-w`, `--wait`       | Timeout v milisekundách (např. `3000`, výchozí je `5000`)          |
| `hostname` nebo `IP` | Cílové zařízení ke skenování                                       |

## Příklad použití

```bash
# Sken TCP portů 21 a 22 a UDP portů 53 a 67 na localhostu
./ipk-l4-scan -i lo -t 21,22 -u 53,67 localhost

# TCP scan portu 80 na webu fakulty
./ipk-l4-scan -i wlp3s0 -t 80 www.fit.vutbr.cz

# Rychlejší test s timeoutem 1 sekunda
./ipk-l4-scan -i lo -t 9999 -w 1000 127.0.0.1

# Výpis aktivních rozhraní
./ipk-l4-scan --interface
```

## Výstup

Každý řádek výstupu odpovídá jednomu portu a je ve formátu:

```text
IP PORT PROTOCOL STATE
```

Například:

```text
127.0.0.1 22 tcp open
127.0.0.1 53 udp closed
127.0.0.1 143 tcp filtered
```
```

